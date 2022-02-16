/*
 *实现目标:
 *    设计一种融合的内存分配策略
 *    它应该在申请特定大小范围内的内存块时
 *    采取不同的内存分配策略,用以提升其分配策略的效率
 *    降低碎片产生
 */

#define MEMORY_MANAGE_SIMPLIFY
#ifdef  MEMORY_MANAGE_SIMPLIFY
 
#define NULL (void *)0

#define BOOL  unsigned char
#define FALSE 0
#define TRUE  1

#endif

#define MEMORY_MANAGE_STRATEGY
#ifdef  MEMORY_MANAGE_STRATEGY

//块管理器
typedef struct block_set {
    unsigned char *first_block;    //块链表头集
    unsigned char *last_block;     //块链表尾集
} struct_block;

//slab分配单元
typedef struct slab_distributor {
    struct slab_distributor *next; //下一个分配单元
    unsigned char *memory_start;   //管理区起始地址
    unsigned char *memory_end;     //管理区结束地址
    unsigned int block_size;       //块大小
    unsigned int block_number;     //使用块数量
} struct_slab;

//buddy分配单元
typedef struct buddy_distributor {
    struct buddy_distributor *next; //下一个分配单元
    unsigned char *memory_start;    //管理区起始地址
    unsigned char *memory_end;      //管理区结束地址
    unsigned char *block_offset;    //基地址偏移
    unsigned int block_size;        //最小块大小
    unsigned int block_index;       //倍频因子
    unsigned int block_number;      //使用块数量
} struct_buddy;

//heap分配单元
typedef struct heap_distributor {
    struct heap_distributor *next; //下一个分配单元
    unsigned char *memory_start;   //管理区起始地址
    unsigned char *memory_end;     //管理区结束地址
    unsigned char *heap_index;     //堆分配起始(地址对齐)
    unsigned int block_size;       //最小块大小
} struct_heap;

//
static struct_slab  *slab_list  = NULL;
static struct_buddy *buddy_list = NULL;
static struct_heap  *heap_list  = NULL;

#endif

#define MEMORY_MANAGE_BLOCK_DISTRIBUTOR
#ifdef  MEMORY_MANAGE_BLOCK_DISTRIBUTOR

//小型对象式分配策略:
#define MEMORY_MANAGE_SLAB_DISTRIBUTOR
#ifdef  MEMORY_MANAGE_SLAB_DISTRIBUTOR

//将其注入为slab分配器
static BOOL slab_register(unsigned char *memory_start,
                          unsigned char *memory_end,
                          unsigned int   block_size)
{
    if ((memory_start == NULL || memory_end == NULL) ||
        (block_size & sizeof(void *) != 0))
        return FALSE;
    //1.内存区起始位置存放分配单元的头部区域
    struct_slab  *slab_zone  = (struct_slab *)memory_start;
    struct_block *block_zone = (struct_block *)(slab_zone + 1);
    //2.手动调节空闲链表区域(让其地址对齐,且至少保证有一块空间够用)
    memory_start += sizeof(struct_slab);
    memory_start += sizeof(struct_block) - 1;
    while ((unsigned char)(++memory_start) & (sizeof(void *) - 1));
    if (memory_start + block_size - 1 > memory_end)
        return FALSE;
    //3.初始化该缓冲区
    unsigned char *first_block = NULL;
    unsigned char *last_block  = NULL;
    unsigned char *pointer1 = memory_start;
    unsigned char *pointer2 = pointer1 + block_size;
    unsigned char *pointer3 = pointer2 + block_size - 1;

    first_block = pointer1;
    while (pointer2 != block_size) {
        //1.下一个块存在性检查
        pointer2 = (pointer3 > memory_end) ? NULL : pointer2;
        //2.当前块指向下一个块
        *((unsigned char **)pointer1) = pointer2;
        //3.迭代到下一个块
        pointer1 += block_size;
        pointer2 += block_size;
        pointer3 += block_size;
    }
    last_block = pointer1 - block_size;
    //4.初始化基本信息(以及空闲链表)
    slab_zone->next         = NULL;
    slab_zone->memory_start = memory_start;
    slab_zone->memory_end   = memory_end;
    slab_zone->block_size   = block_size;
    slab_zone->block_number = 0;
    //5.初始化块空闲链表
    block_zone->first_block = first_block;
    block_zone->last_block  = NULL;//last_block;
    //6.将其加入到slab分配器链表集中(经典二段式)
    if (slab_list == NULL || 
        slab_list->block_size >= block_size) {
        slab_zone->next = slab_list;
        slab_list = slab_zone;
        return TRUE;
    }
    
    struct_slab *iterator = slab_list;
    while (iterator->next != NULL &&
           iterator->next->block_size < block_size)
           iterator = iterator->next;
    slab_zone->next = iterator->next;
    iterator->next  = slab_zone;
    return TRUE;
}

//将其剥离出slab分配器
static BOOL slab_unregister(unsigned char *memory_start)
{
    struct_slab *slab_zone = (struct_slab *)memory_start;

    if (slab_list == NULL)
        return FALSE;
    if (slab_zone == NULL || slab_zone->block_number != 0)
        return FALSE;
        
    if (slab_list == slab_zone) {
        slab_list  = slab_zone->next;
        return TRUE;
    }
    
    struct_slab *iterator = slab_list;
    while (iterator->next != NULL &&
           iterator->next != slab_zone)
           iterator = iterator->next;

    if (iterator->next == NULL)
        return FALSE;
    iterator->next = slab_zone->next;
    return TRUE;
}

//从SLAB分配器获得块
static BOOL slab_allocate(unsigned char **block, unsigned int block_size)
{
    if(block == NULL)
        return FALSE;
    
    //查找最小可分配的slab分配器
    struct_slab *slab_zone = slab_list;
    while (slab_zone != NULL && slab_zone->block_size < block_size)
        slab_zone = slab_zone->next;
    //限制slab的使用,不可分配大块slab单元
    while (slab_zone != NULL && slab_zone->block_size == block_size) {
        struct_block *block_zone = (struct_block *)(slab_zone + 1);
        //如果空闲链表不为空,直接分配即可,否则检查下一个块
        if (block_zone->first_block != NULL) {
            *block = block_zone->first_block;
            //更新块的尾指针到前一项
            block_zone->first_block = *((unsigned char **)(*block));
            slab_zone->block_number++;
            return TRUE;  
        }
        slab_zone = slab_zone->next;
    }
    return FALSE;
}

//释放块到SLAB分配器
static BOOL slab_free(unsigned char *block)
{
    if (block == NULL)
        return FALSE;
    
    //确认该块属于哪一个slab分配器中
    struct_slab *slab_zone = slab_list;
    while (slab_zone != NULL &&
          (slab_zone->memory_start > block ||
           slab_zone->memory_end < block))
           slab_zone = slab_zone->next;
    //如果没有找到分配器,退出
    if (slab_zone == NULL)
        return FALSE;
    struct_block *block_zone = (struct_block *)(slab_zone + 1);
    //1.更新新来的项为空闲链表的后一项
    *((unsigned char**)block) = block_zone->first_block;
    //2.更新空闲链表尾部索引
    block_zone->first_block = block;
    slab_zone->block_number--;
    return TRUE;
}

#endif

//大型块式分配策略:
#define MEMORY_MANAGE_BUDDY_DISTRIBUTOR
#ifdef  MEMORY_MANAGE_BUDDY_DISTRIBUTOR

//将其注入为buddy分配器
static BOOL buddy_register(unsigned char *memory_start,
                           unsigned char *memory_end,
                           unsigned int   maximum_size,
                           unsigned int   minimum_size)
{
    //块分配大小对齐检查,块大小与分裂因子检查!-->必须2**k次方
    if ((memory_start == NULL || memory_end == NULL) ||
        (maximum_size & sizeof(void *) != 0) ||
        (minimum_size & sizeof(void *) != 0) ||
        (maximum_size & (maximum_size - 1) != 0) ||
        (minimum_size & (minimum_size - 1) != 0) ||
        (maximum_size / minimum_size < 2))
        return FALSE;
    //最小块检查!

    //1.内存区起始位置存放分配单元的头部区域(初始化时资源存放最大块中)
    struct_buddy *buddy_zone = (struct_buddy *)memory_start;
    struct_block *block_zone = (struct_block *)(buddy_zone + 1);
    unsigned int  block_size = minimum_size;
    //2.确认伙伴集合长度
    unsigned int block_index = 0;
    while ((minimum_size << (++block_index)) != maximum_size);
    //3.手动调节空闲链表区域(让其地址对齐,且至少保证有一块空间够用)
    memory_start += sizeof(struct_buddy);
    memory_start += sizeof(struct_block) * (block_index + 1) - 1;
    while ((unsigned char)(++memory_start) & (sizeof(void *) - 1));
    if (memory_start + block_size - 1 > memory_end)
        return FALSE;
    //4.初始化块区域
    unsigned char *first_block = NULL;
    unsigned char *last_block  = NULL;
    unsigned char *pointer1 = memory_start;
    unsigned char *pointer2 = pointer1 + maximum_size;
    unsigned char *pointer3 = pointer2 + maximum_size - 1;

    first_block = pointer1;
    while (pointer2 != maximum_size) {
        //1.下一个块存在性检查
        pointer2 = (pointer3 > memory_end) ? NULL : pointer2;
        //2.当前块指向下一个块
        *((unsigned char **)pointer1) = pointer2;
        //3.迭代到下一个块
        pointer1 += maximum_size;
        pointer2 += maximum_size;
        pointer3 += maximum_size;
    }
    last_block = pointer1 - maximum_size;
    //5.初始化基本信息
    buddy_zone->next         = NULL;
    buddy_zone->memory_start = memory_start;
    buddy_zone->memory_end   = memory_end;
    buddy_zone->block_offset = memory_start;
    buddy_zone->block_size   = block_size;
    buddy_zone->block_index  = block_index;
    buddy_zone->block_number = 0;
    //6.绑定块区域
    block_zone[block_index].first_block = first_block;
    block_zone[block_index].last_block  = NULL;//last_block;
    while (--block_index >= 0) {
        block_zone[block_index].first_block = NULL;
        block_zone[block_index].last_block  = NULL;
        if (block_index == 0)
            break;
    }
    //7.将其加入到slab分配器链表集中(经典二段式)
    if (buddy_list == NULL || 
        buddy_list->block_size >= maximum_size) {
        buddy_zone->next = buddy_list;
        buddy_list = buddy_zone;
        return TRUE;
    }
    
    struct_buddy *iterator = buddy_list;
    while (iterator->next != NULL &&
           iterator->next->block_size < maximum_size)
           iterator  = iterator->next;
    buddy_zone->next = iterator->next;
    iterator->next   = buddy_zone;
    return TRUE;
}

//将其剥离出buddy分配器
static BOOL buddy_unregister(unsigned char *memory_start)
{
    struct_buddy *buddy_zone = (struct_buddy *)memory_start;

    if (buddy_list == NULL)
        return FALSE;
    if (buddy_zone == NULL || buddy_zone->block_number != 0)
        return FALSE;
        
    if (buddy_list == buddy_zone) {
        buddy_list  = buddy_zone->next;
        return TRUE;
    }
    
    struct_buddy *iterator = buddy_list;
    while (iterator->next != NULL &&
           iterator->next != buddy_zone)
           iterator = iterator->next;

    if (iterator->next == NULL)
        return FALSE;
    iterator->next = buddy_zone->next;
    return TRUE;
}

//从BUDDY分配器获得块
static BOOL buddy_allocate(unsigned char **block, unsigned int block_size)
{
    struct_buddy *buddy_zone = NULL;
    for (buddy_zone = buddy_list; 
         buddy_zone != NULL && block != NULL;
         buddy_zone = buddy_zone->next) {
        //获取最大最小链表尺寸
        unsigned int minimum_size = (buddy_zone->block_size);
        unsigned int maximum_size = (buddy_zone->block_size);
        maximum_size <<= (buddy_zone->block_index);
        //1.查找可分配的buddy分配器(仅可分配范围内的内存区)
        if (block_size > maximum_size || block_size < minimum_size)
            continue;
        //2.获得伙伴集合,以及必要成员
        minimum_size = buddy_zone->block_size;
        struct_block *block_zone = (struct_block *)(buddy_zone + 1);
        unsigned int block_index = buddy_zone->block_index;
        //3.定位到合适伙伴集内部
        unsigned char *buddy = NULL;
        unsigned int offset = 0, i = 0, j = 0;
        for (offset = 0; (minimum_size << offset) < block_size; offset++);
        if ((minimum_size << offset) != block_size)
            return FALSE;
        //4.刚好存在块可分配,直接分配,结束
        if (block_zone[offset].first_block != NULL) {
            *block = block_zone[offset].first_block;
            block_zone[offset].first_block = *((unsigned char **)(*block));
            buddy_zone->block_number++;
            return TRUE;
        }
        //5.该块不存在元素,向上寻找,找到"最小最近"一个块,将其迭代分裂
        for (i = offset; i <= block_index; i++)
            if (block_zone[i].first_block != NULL)
                break;
        if (i > block_index)
            continue;
        //6.将该块解除出关联中,分裂
        *block = block_zone[i].first_block;
        block_zone[i].first_block = *((unsigned char **)(*block));
        //7.反复掐掉其右伙伴加入集合,直到回退
        for (j = i - 1; j < i && j >= offset; j--) {
            buddy = *block + (minimum_size << j);
            //将后半部分加入到次级集合中,前半部分继续迭代分裂
            *((unsigned char **)(buddy)) = block_zone[j].first_block;
            block_zone[j].first_block = buddy;
        }
        buddy_zone->block_number++;
        return TRUE;
    }
    return FALSE;
}

//释放块到BUDDY分配器
static BOOL buddy_free(unsigned char *block, unsigned int block_size)
{
    struct_buddy *buddy_zone = NULL;
    for (buddy_zone = buddy_list; 
         buddy_zone != NULL && block != NULL;
         buddy_zone = buddy_zone->next) {
        //1.查找块隶属的buddy分配器(仅可释放到范围内的内存区)
        if (buddy_zone->memory_start > block ||
            buddy_zone->memory_end < block)
            continue;

        //2.获得伙伴集合,以及必要成员
        struct_block *block_zone = (struct_block *)(buddy_zone + 1);
        unsigned int minimum_size = buddy_zone->block_size;
        unsigned int block_index = buddy_zone->block_index;
        //3.定位到合适伙伴集内部
        unsigned char *buddy = NULL;
        unsigned char *address_offset = NULL;
        unsigned char *iterator = NULL;
        unsigned int offset = 0, i = 0;
        for (offset = 0; (minimum_size << offset) < block_size; offset++);
        if ((minimum_size << offset) != block_size)
            return FALSE;
        //4.寻找到当前块的归属,插入,且确认它伙伴
        for (i = offset; i <= block_index; i++) {
            //备注:p % 2**(k+1) == 0时,p 和 p + 2**k 互为伙伴
            address_offset = block - buddy_zone->block_offset;
            if ((unsigned int)address_offset % (minimum_size << (i + 1)) == 0)
                buddy = block + (minimum_size << i);
            if ((unsigned int)address_offset % (minimum_size << (i + 1)) != 0)
                buddy = block - (minimum_size << i);

            //1.检查伙伴块是否在集合中,如果在则将其取出集合
            if (block_zone[i].first_block != NULL && block_index != i) {
                if (block_zone[i].first_block == buddy) {
                    block_zone[i].first_block = *((unsigned char **)(buddy));
                    //伙伴在链表集中,与伙伴合并,进入下一阶段
                    block = buddy < block ? buddy : block;
                    continue;
                }
                
                for (iterator = block_zone[i].first_block;
                     *((unsigned char **)(iterator)) != NULL &&
                     *((unsigned char **)(iterator)) < buddy;
                     iterator = *((unsigned char **)(iterator)));

                if (*((unsigned char **)(iterator)) == buddy) {
                    *((unsigned char **)(iterator)) =
                    *((unsigned char **)(buddy));
                    *((unsigned char **)(buddy)) = NULL;
                    //伙伴在链表集中,与伙伴合并,进入下一阶段
                    block = buddy < block ? buddy : block;
                    continue;
                }
            }
        
            //2.不可继续迭代合并,此处退出,假伙伴
            //3.伙伴不在链表集中,自己加入到集合中
            if (block_zone[i].first_block == NULL || 
                block_zone[i].first_block >= block) {
                *((unsigned char **)(block)) = block_zone[i].first_block;
                block_zone[i].first_block = block;
                buddy_zone->block_number--;
                return TRUE;
            }
            
            for (iterator = block_zone[i].first_block;
                 *((unsigned char **)(iterator)) != NULL &&
                 *((unsigned char **)(iterator)) < block;
                 iterator = *((unsigned char **)(iterator)));
                 
            *((unsigned char **)(block)) = *((unsigned char **)(iterator));
            *((unsigned char **)(iterator)) = block;
            buddy_zone->block_number--;
            return TRUE;
        }
    }
    return FALSE;
}

#endif

#endif

#define MEMORY_MANAGE_C_TEST
#ifdef  MEMORY_MANAGE_C_TEST

#include <stdio.h>

void test_memory_allocate_slab(void)
{
#define SLAB_NUMBER  5
#define SLAB_SIZE    8
#define SLAB_LENGTH (sizeof(struct_slab) +  \
                     sizeof(struct_block) + \
                     SLAB_SIZE * SLAB_NUMBER)
                     
    unsigned int i = 0, k = 0, j = 0;
    unsigned char test1[SLAB_LENGTH] = {0};
    unsigned char test2[SLAB_LENGTH] = {0};
    unsigned char test3[SLAB_LENGTH] = {0};
    unsigned char *temp[100] = {0};
    
    printf("slab_register:%d\n",
            slab_register(test1, test1 + SLAB_LENGTH - 1, SLAB_SIZE));
    printf("slab_register:%d\n",
            slab_register(test2, test2 + SLAB_LENGTH - 1, SLAB_SIZE));
    printf("slab_register:%d\n",
            slab_register(test3, test3 + SLAB_LENGTH - 1, SLAB_SIZE));

#define PRINTF_SLAB_DISTRIBUTOR(name, id)        \
    printf("slab_distributor %d:\n"              \
           "next: %x\n"                          \
           "memory_start: %x\n"                  \
           "memory_end: %x\n"                    \
           "block_size: %u\n"                    \
           "block_number: %u\n" , id,            \
           ((struct_slab *)name)->next,          \
           ((struct_slab *)name)->memory_start,  \
           ((struct_slab *)name)->memory_end,    \
           ((struct_slab *)name)->block_size,    \
           ((struct_slab *)name)->block_number); \
    printf("first_block[%u]: %x --- "            \
           "last_block[%u]: %x\n" ,              \
           id,                                   \
           ((struct_block *)(name +              \
            sizeof(struct_slab)))->first_block,  \
           id,                                   \
           ((struct_block *)(name +              \
            sizeof(struct_slab)))->last_block);  \
    
    PRINTF_SLAB_DISTRIBUTOR(test1, 1);
    PRINTF_SLAB_DISTRIBUTOR(test2, 2);
    PRINTF_SLAB_DISTRIBUTOR(test3, 3);

    printf("slab_unregister:%d\n",
            slab_unregister(test3));

#define MEMORY_RPINTF(test_x)                                                    \
    printf("--------------------------------------------------------------\n");  \
    for (j = 0; j < SLAB_NUMBER; j++) {                                          \
        printf("addr:%16x >-< ",                                                 \
                test_x +                                                         \
                sizeof(struct_slab) +                                            \
                sizeof(struct_block) +                                           \
                j * SLAB_SIZE);                                                  \
        for (k = 0; k < SLAB_SIZE; k++)                                          \
            printf("%2x ",                                                       \
                 *(test_x +                                                      \
                 sizeof(struct_slab) +                                           \
                 sizeof(struct_block) +                                          \
                 j * SLAB_SIZE + k));                                            \
        printf("\n");                                                            \
    }

    printf("memory:-------------------------------------------------------\n");
    MEMORY_RPINTF(test1);
    MEMORY_RPINTF(test2);
    MEMORY_RPINTF(test3);
    printf("\n\n");
    
    printf("--------------------------------------------------------------\n");
    for (i = 0; i < 10; i++) {
        printf("count: %d allocate: %d\n",i,slab_allocate(&(temp[i]), SLAB_SIZE));
        if (temp[i] == NULL)
            continue;

        for (k = 0; k < SLAB_SIZE; k++)
            *(temp[i] + k) = 1;

        MEMORY_RPINTF(test1);
        MEMORY_RPINTF(test2);
        MEMORY_RPINTF(test3);
        printf("\n\n");
    }
    
    printf("--------------------------------------------------------------\n");
    char a1[10] = {0,8,2,1,4,3,6,5,7,9};
    for (i = 0; i < 10; i++) {
        printf("count:%d free: %d", i,slab_free(temp[a1[i]]));
        if (temp[a1[i]] == NULL)
            continue;
        
        MEMORY_RPINTF(test1);
        MEMORY_RPINTF(test2);
        MEMORY_RPINTF(test3);
        printf("\n\n");
    }
}

void test_memory_allocate_buddy(void)
{
#define BUDDY_NUMBER    5
#define BUDDY_MIN_INDEX 3
#define BUDDY_MAX_INDEX 5
#define BUDDY_EXTERN (BUDDY_MAX_INDEX - BUDDY_MIN_INDEX + 1)
#define BUDDY_LENGTH (BUDDY_NUMBER * (1 << BUDDY_MAX_INDEX) + \
                      sizeof(void *) + \
                      sizeof(struct_buddy) + \
                      sizeof(struct_block) * (BUDDY_EXTERN))
    unsigned int i = 0, j = 0, k = 0, t = 0;
    unsigned char test1[BUDDY_LENGTH] = {0};
    unsigned char *temp[100] = {0};
    
    printf("buddy_register:%d\n",
            buddy_register(test1, test1 + BUDDY_LENGTH - 1,
                           1 << BUDDY_MAX_INDEX,
                           1 << BUDDY_MIN_INDEX));

#define PRINTF_BUDDY_DISTRIBUTOR(name, id)              \
    printf("buddy_distributor: %d\n"                    \
           "next: %x\n"                                 \
           "memory_start: %x\n"                         \
           "memory_end: %x\n"                           \
           "block_size: %u\n"                           \
           "block_index: %u\n"                          \
           "block_number: %u\n", id,                    \
           ((struct_buddy *)name)->next,                \
           ((struct_buddy *)name)->memory_start,        \
           ((struct_buddy *)name)->memory_end,          \
           ((struct_buddy *)name)->block_size,          \
           ((struct_buddy *)name)->block_index,         \
           ((struct_buddy *)name)->block_number);       \
           i = 0;                                       \
    while (i <= ((struct_buddy *)name)->block_index) {  \
        printf("first_block[%u]: %16x --- "             \
               "last_block[%u]: %16x\n",                \
               i, ((struct_block *)                     \
                   (name+sizeof(struct_buddy))          \
                   )[i].first_block,                    \
               i, ((struct_block *)                     \
                   (name+sizeof(struct_buddy))          \
                   )[i].last_block);                    \
           i++;                                         \
    }
    
    PRINTF_BUDDY_DISTRIBUTOR(test1, 1);
    

#define MEMORY_RPINTF(test_x)                                                    \
    printf("--------------------------------------------------------------\n");  \
    for (i = 0; i < BUDDY_NUMBER; i++) {                                         \
        printf("addr: %x >-< ",                                                  \
                test_x +                                                         \
                sizeof(struct_buddy) +                                           \
                sizeof(struct_block) * BUDDY_EXTERN +                            \
                i * (1 << BUDDY_MAX_INDEX));                                     \
        for (j = 0; j < (1 << BUDDY_MAX_INDEX); j += sizeof(void *))             \
            printf("%6x\t",                                                      \
                 *(unsigned long long *)(test_x +                                \
                  sizeof(struct_buddy) +                                         \
                  sizeof(struct_block) * BUDDY_EXTERN +                          \
                  i * (1 << BUDDY_MAX_INDEX) + j));                              \
        printf("\n");                                                            \
    }
    
    printf("memory:-------------------------------------------------------\n");
    MEMORY_RPINTF(test1);
    printf("\n\n");
    
    printf("--------------------------------------------------------------\n");
    for (k = 0; k < 10; k++) {
        printf("count: %d allocate: %d\n", k,
                buddy_allocate(&(temp[k]), (1 << BUDDY_MIN_INDEX)));
        if (temp[k] == NULL)
            continue;

        for (t = 0; t < (1 << BUDDY_MIN_INDEX); t += sizeof(void *))
            *((unsigned int *)(temp[k] + t)) = 0x1;

        MEMORY_RPINTF(test1);
        //MEMORY_RPINTF(test2);
        //MEMORY_RPINTF(test3);
        printf("\n\n");
    }
    
    printf("--------------------------------------------------------------\n");
    char a1[10] = {0,8,2,1,4,3,6,5,7,9};
    for (k = 0; k < 10; k++) {
        printf("count:%d free: %d", k,
                buddy_free(temp[a1[k]], (1 << BUDDY_MIN_INDEX)));
        if (temp[a1[k]] == NULL)
            continue;
        
        MEMORY_RPINTF(test1);
        PRINTF_BUDDY_DISTRIBUTOR(test1, 1);
        //MEMORY_RPINTF(test2);
        //MEMORY_RPINTF(test3);
        printf("\n\n");
    }
}

#endif










