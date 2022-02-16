
#define MEMORY_MANAGE_HEAP_SIMPLIFY
#ifdef  MEMORY_MANAGE_HEAP_SIMPLIFY
 
#define NULL (void *)0

#define BOOL  unsigned char
#define FALSE 0
#define TRUE  1

#endif

#define MEMORY_MANAGE_HEAP_STRATEGY
#ifdef  MEMORY_MANAGE_HEAP_STRATEGY


#endif

#define MEMORY_MANAGE_HEAP_DISTRIBUTOR
#ifdef  MEMORY_MANAGE_HEAP_DISTRIBUTOR

static BOOL heap_register(unsigned char *memory_start,
                          unsigned char *memory_end,
                          unsigned int   block_size)
{
    if (memory_start == NULL || memory_end == NULL)
        return FALSE;
    //1.堆的最小空间检查
    if (block_size < sizeof(unsigned char *) + sizeof(unsigned int))
        block_size = sizeof(unsigned char *) + sizeof(unsigned int);
    //2.内存区起始位置存放分配单元的头部区域
    struct_heap  *heap_zone  = (struct_heap *)memory_start;
    struct_block *block_zone = (struct_block *)(heap_zone + 1);
    //3.手动调节空闲链表区域(让其地址对齐,且至少保证有一块空间够用)
    memory_start += sizeof(struct_heap);
    memory_start += sizeof(struct_block) - 1;
    while ((unsigned char)(++memory_start) & (sizeof(void *) - 1));
    if (memory_start + block_size - 1 > memory_end)
        return FALSE;
    //4.初始化基本信息(以及空闲链表)
    heap_zone->next         = NULL;
    heap_zone->memory_start = memory_start;
    heap_zone->memory_end   = memory_end;
    heap_zone->heap_index   = memory_start;
    block_zone->first_block = NULL;
    block_zone->last_block  = NULL;
    //5.将其加入到heap分配器链表集中(经典头插式)
    heap_zone->next = heap_list;
    heap_list       = heap_zone;
    return TRUE;
}

static BOOL heap_unregister(unsigned char *memory_start)
{
    struct_heap *heap_zone = (struct_heap *)memory_start;

    if (heap_list == NULL)
        return FALSE;
    if (heap_zone == NULL)
        return FALSE;
        
    if (heap_list == heap_zone) {
        heap_list =  heap_zone->next;
        return TRUE;
    }
    
    struct_heap *iterator = heap_list;
    while (iterator->next != NULL &&
           iterator->next != heap_zone)
           iterator = iterator->next;

    if (iterator->next == NULL)
        return FALSE;
    iterator->next = heap_zone->next;
    return TRUE;
}

static BOOL heap_allocate(unsigned char **block, unsigned int block_size)
{
    if(block == NULL)
        return FALSE;
    
    //为了释放所用,额外准备一个小的空间存放块长度信息
    if (block_size < sizeof(unsigned char *))
        block_size = sizeof(unsigned char *);
    block_size += sizeof(unsigned int) - 1;
    //保证分配出去的块是完整的字节对齐块
    while(++block_size != sizeof(void *));
    
    //查找最小可分配的heap分配器
    struct_heap *heap_zone = heap_list;
    while (heap_zone != NULL) {
        unsigned int size = 0;
        unsigned char *next = NULL;
        unsigned char *iterator = NULL;
        struct_block *block_zone = (struct_block *)(heap_zone + 1);
        //1.查阅free_list是否有空间用以提供(严格限制:
        //  优先寻找差不多的块分配,剩余部分不能独立成块,不要拆碎)
        //查单链表经典二段式
        iterator = block_zone->first_block;
        if (iterator != NULL) {
            size = *((unsigned int *)(iterator + sizeof(unsigned char *)));
            if (size > block_size &&
                size < block_size + heap_zone->block_size) {
                //将其从空闲链表集中取出
                block_zone->first_block = *((unsigned char **)iterator);
                *block = iterator;
                //调整size的存储位置到前面
                *((unsigned int *)(*block)) = size;
                *block += sizeof(unsigned int);
                return TRUE;
            }
        }

        while (*((unsigned char **)iterator) != NULL) {
            next = *((unsigned char **)iterator);
            size = *((unsigned int *)(next + sizeof(unsigned char *)));
            if (size > block_size &&
                size < block_size + heap_zone->block_size) {
                //将其从空闲链表集中取出
                *((unsigned char **)iterator) = *((unsigned char **)next);
                *block = next;
                //调整size的存储位置到前面
                *((unsigned int *)(*block)) = size;
                *block += sizeof(unsigned int);
                return TRUE;
            }
            iterator = *((unsigned char **)iterator);
        }
        
        //2.如果堆中有空间用以提供
        if ((heap_zone->heap_index != NULL)
            (heap_zone->heap_index + block_size < heap_zone->memory_end)) {
            //1.直接取出该块
            *block = heap_zone->heap_index;
            *((unsigned int *)(*block)) = block_size;
            *block += sizeof(unsigned int);
            //2.更新堆的首地址
            heap_zone->heap_index += block_size;
            if (heap_zone->heap_index > heap_zone->memory_end)
                heap_zone->heap_index = NULL;
            return TRUE;
        }
        
        //3.再次查阅free_list是否有空间用以提供(宽松限制:
        //  分配出一个块剩余部分还能独立成块,拆碎去分配它)
        //查单链表经典二段式
        iterator = block_zone->first_block;
        if (iterator != NULL) {
            size = *((unsigned int *)(iterator + sizeof(unsigned char *)));
            if (size > block_size + heap_zone->block_size) {
                //将其从空闲链表集中取出
                block_zone->first_block += block_size;
                *block = iterator;
                //调整size的存储位置到前面
                *((unsigned int *)(*block)) = block_size;
                *block += sizeof(unsigned int);
                return TRUE;
            }
        }

        while (*((unsigned char **)iterator) != NULL) {
            next = *((unsigned char **)iterator);
            size = *((unsigned int *)(next + sizeof(unsigned char *)));
            if (size > block_size + heap_zone->block_size) {
                //将其从空闲链表集中取出
                *((unsigned char **)iterator) = next + block_size;
                iterator = *((unsigned char **)iterator);
                *((unsigned char **)iterator) = *((unsigned char **)next);
                *block = next;
                //调整size的存储位置到前面
                *((unsigned int *)(*block)) = size;
                *block += sizeof(unsigned int);
                return TRUE;
            }
            iterator = *((unsigned char **)iterator);
        }
    }
    
    return FALSE;
}

static BOOL heap_free(unsigned char *block)
{
    if (block == NULL)
        return FALSE;
    
    //确认该块属于哪一个heap分配器中
    struct_heap *heap_zone = heap_list;
    while (heap_zone != NULL &&
          (heap_zone->memory_start > block ||
           heap_zone->memory_end < block))
           heap_zone = heap_zone->next;
    //如果没有找到分配器,退出
    if (heap_zone == NULL)
        return FALSE;
    struct_block *block_zone = (struct_block *)(heap_zone + 1);
    
    unsigned int size = 0, flag = FALSE;
    unsigned int block_size = 0;
    unsigned char *iterator = block;
    iterator -= sizeof(unsigned int);
    block_size = *((unsigned int *)iterator);
    iterator += sizeof(unsigned int);
    //将块的信息记录进行逆转
    block += sizeof(unsigned int);
    *((unsigned int *)block) = block_size;
    block -= sizeof(unsigned int);
    *((unsigned char **)block) = NULL;
    //1.将其递归合并到free_list中
    if (block_zone->first_block == NULL && flag == FALSE) {
        block_zone->first_block  = block;
        flag = TRUE;
    }
    
    if (block_zone->first_block != NULL && flag == FALSE) {
        iterator = block_zone->first_block;
        iterator += sizeof(unsigned char **);
        size = *((unsigned int *)iterator);
        iterator -= sizeof(unsigned char **);
        //修单链表经典二段式
        if (iterator + size == block && flag == FALSE) {
            iterator += sizeof(unsigned char **);
            *((unsigned int *)iterator) += block_size;
            iterator -= sizeof(unsigned char **);
            flag = TRUE;
        }
        
        if (block > iterator && flag == FALSE) {
            *((unsigned char **)block) = iterator;
            block_zone->first_block    = block;
            flag = TRUE;
        }

        while (*((unsigned char **)iterator) != NULL && flag == FALSE) {
            //出现地址连续的情况
            if (*((unsigned char **)iterator) + size == block) {
                //可以与前一个块合并,合并至一起
                iterator += sizeof(unsigned char **);
                *((unsigned int *)iterator) += block_size;
                iterator -= sizeof(unsigned char **);
                flag = TRUE;
                break;
            }
        }
    }
    
    
   

    //2.检查free_list中释放
    
}

#endif


