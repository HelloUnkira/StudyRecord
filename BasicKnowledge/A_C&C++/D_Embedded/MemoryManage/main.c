//C std lib
#include <stdio.h>
//
#include "MemoryManageInterface.h"

//复写打印对接接口
void CheckPrint(const char *title, uint32_t content)
{
    printf(title, content);
}

//测试slab分配器
void test_slab_distributor(void)
{
    /* 获取初始化内存布局简要 */
    MemoryManageSlabCheck(CheckPrint);
    
#define SLAB_NUMBER  5
#define SLAB_SIZE    8

    int32_t i = 0, j = 0, k = 0;
    
    uint8_t *test0_start = NULL, *test0_end = NULL;
    uint8_t *test1_start = NULL, *test1_end = NULL;
    uint8_t *test2_start = NULL, *test2_end = NULL;
    
    //创建一个大数组在分配和释放时用于保存地址
    uint8_t *temp[100] = {0};
    
    MemoryManageSlabMem(0, &test0_start, &test0_end);
    MemoryManageSlabMem(1, &test1_start, &test1_end);
    MemoryManageSlabMem(2, &test2_start, &test2_end);

#define MEMORY_RPINTF(test_x, number, size)                                                    \
    printf("--------------------------------------------------------------\n");  \
    for (j = 0; j < number; j++) {                                               \
        printf("addr:%16x >-< ", test_x + j * size);                             \
        for (k = size - 1; k >= 0; k--)                                          \
            printf("%2x ", *(test_x + j * size + k));                            \
        printf("\n");                                                            \
    }
    
    /* 打印内存布局 */
    printf("memory:-------------------------------------------------------\n");
    MEMORY_RPINTF(test0_start, SLAB_NUMBER, SLAB_SIZE);
    MEMORY_RPINTF(test1_start, SLAB_NUMBER, SLAB_SIZE);
    MEMORY_RPINTF(test2_start, SLAB_NUMBER, SLAB_SIZE);
    printf("\n\n");

    printf("--------------------------------------------------------------\n");
    for (i = 0; i < 15; i++) {
        printf("count: %d allocate: %d\n",i, MemoryManageSlabTake(SLAB_SIZE, &(temp[i])));
        if (temp[i] == NULL)
            continue;

        for (k = 0; k < SLAB_SIZE; k++)
            *(temp[i] + k) = 1;

        MEMORY_RPINTF(test0_start, SLAB_NUMBER, SLAB_SIZE);
        MEMORY_RPINTF(test1_start, SLAB_NUMBER, SLAB_SIZE);
        MEMORY_RPINTF(test2_start, SLAB_NUMBER, SLAB_SIZE);
        printf("\n\n");
    }
    
    /* 获取初始化内存布局简要 */
    MemoryManageSlabCheck(CheckPrint);
    
    printf("--------------------------------------------------------------\n");
    char a1[15] = {0,8,12,2,1,14,4,10,3,13,6,5,11,7,9};
    for (i = 0; i < 15; i++) {
        printf("count:%d free: %d", i,MemoryManageSlabGive(SLAB_SIZE, temp[a1[i]]));
        if (temp[a1[i]] == NULL)
            continue;
        
        MEMORY_RPINTF(test0_start, SLAB_NUMBER, SLAB_SIZE);
        MEMORY_RPINTF(test1_start, SLAB_NUMBER, SLAB_SIZE);
        MEMORY_RPINTF(test2_start, SLAB_NUMBER, SLAB_SIZE);
        printf("\n\n");
    }
    
    /* 获取初始化内存布局简要 */
    MemoryManageSlabCheck(CheckPrint);
}

//测试buddy分配器
void test_buddy_distributor(void)
{
    /* 获取初始化内存布局简要 */
    MemoryManageBuddyCheck(CheckPrint);
    
#define BUDDY_NUMBER    5
#define BUDDY_MIN_INDEX 3
#define BUDDY_MAX_INDEX 5

    int32_t i = 0, j = 0, k = 0, t = 0;
    
    uint8_t *test0_start = NULL, *test0_end = NULL;
    
    //创建一个大数组在分配和释放时用于保存地址
    uint8_t *temp[100] = {0};
    
    MemoryManageBuddyMem(0, &test0_start, &test0_end);
    
#define MEMORY_RPINTF(test_x)                                                           \
    printf("--------------------------------------------------------------\n");         \
    for (i = 0; i < BUDDY_NUMBER; i++) {                                                \
        printf("addr: %x >-< ", test_x + i * (1 << BUDDY_MAX_INDEX));                   \
        for (j = 0; j < (1 << BUDDY_MAX_INDEX); j += sizeof(void *))                    \
            printf("%6x\t", *(uint64_t *)(test_x + i * (1 << BUDDY_MAX_INDEX) + j));    \
        printf("\n");                                                                   \
    }

    /* 打印内存布局 */
    printf("memory:-------------------------------------------------------\n");
    MEMORY_RPINTF(test0_start);
    printf("\n\n");
    
    printf("--------------------------------------------------------------\n");
    for (k = 0; k < 15; k++) {
        printf("count: %d allocate: %d\n", k,
            MemoryManageBuddyTake((1 << BUDDY_MIN_INDEX), &(temp[k])));
        if (temp[k] == NULL)
            continue;

        for (t = 0; t < (1 << BUDDY_MIN_INDEX); t += sizeof(void *))
            *((uint32_t *)(temp[k] + t)) = 0x1;

        MEMORY_RPINTF(test0_start);
        printf("\n\n");
    }
    
    /* 获取初始化内存布局简要 */
    MemoryManageBuddyCheck(CheckPrint);
    
    printf("--------------------------------------------------------------\n");
    char a1[15] = {0,8,12,2,1,14,4,10,3,13,6,5,11,7,9};
    for (k = 0; k < 15; k++) {
        printf("count:%d free: %d", k,
            MemoryManageBuddyGive((1 << BUDDY_MIN_INDEX), temp[a1[k]]));
        if (temp[a1[k]] == NULL)
            continue;
        
        MEMORY_RPINTF(test0_start);
        printf("\n\n");
    }
    
    /* 获取初始化内存布局简要 */
    MemoryManageBuddyCheck(CheckPrint);
}

//测试heapf分配器
void test_heap_distributor(void)
{
    /* 获取初始化内存布局简要 */
    MemoryManageHeapCheck(CheckPrint);
    
    uint8_t *test0_start = NULL, *test0_end = NULL;
    
    //创建一个大数组在分配和释放时用于保存地址
    uint8_t *temp[100] = {0};
    
    MemoryManageHeapMem(0, &test0_start, &test0_end);
    
    //获取四块内存区分别为(512, 256, 1024, 128)
    printf("count: 0 allocate: %d\n", MemoryManageHeapTake(512,  &(temp[0])));
    MemoryManageHeapCheck(CheckPrint);
    printf("count: 1 allocate: %d\n", MemoryManageHeapTake(256,  &(temp[1])));
    MemoryManageHeapCheck(CheckPrint);
    printf("count: 2 allocate: %d\n", MemoryManageHeapTake(1024, &(temp[2])));
    MemoryManageHeapCheck(CheckPrint);
    printf("count: 3 allocate: %d\n", MemoryManageHeapTake(128,  &(temp[3])));
    MemoryManageHeapCheck(CheckPrint);
    //释放四块内存区
    printf("count: 0 free: %d", MemoryManageHeapGive(512,  temp[0]));
    MemoryManageHeapCheck(CheckPrint);
    printf("count: 1 free: %d", MemoryManageHeapGive(1024, temp[2]));
    MemoryManageHeapCheck(CheckPrint);
    printf("count: 2 free: %d", MemoryManageHeapGive(256,  temp[1]));
    MemoryManageHeapCheck(CheckPrint);
    printf("count: 3 free: %d", MemoryManageHeapGive(128,  temp[3]));
    MemoryManageHeapCheck(CheckPrint);
    
}

//融合策略提供
/*1.目前因为设计原因
 *  Buddy分配器释放时需要归还申请空间的大小
 *  所以每次申请前应需额外申请4个字节的空间
 *  保存在首部记录申请时的大小,用户从后四个字节开始去使用内存空间
 *  Slab和Heap内部提前预制了策略完成了该项问题,回收时size可以任意设置
 *  用户可以根据各个分配器的特性为其定制额外的融合分配策略
 *  特性:
 *      Slab分配和释放速度快:O(1),过于固定化,适合分配小内存
 *      Buddy可以弥补Slab过于固定的策略,分配相对Slab较慢
 *      Heap可以动态分配指定大小空间,但面对小碎片显得力不从心
 */
 
/* 例:(Slab不能分配时Buddy去分配,Buddy不能分配时Heap去分配,直到失败返回空)
 * 在: 8 ~ 512字节时使用Slab快速分配小碎片内存
 * 在: 512~ 4K字节时使用Buddy分配可融合内存
 * 在: 4K及以上时使用Heap分配大块内存
 * 注意:这需要静态重配Configure的内容
 * 这只是一个简单的距离,是否有用未知
 */
 
uint32_t CalculateTouch2Pow(uint32_t size)
{
    /* 求size最近2**x */
    uint32_t temp = size - 1;
    
    temp |= temp >> 1;
    temp |= temp >> 2;
    temp |= temp >> 4;
    temp |= temp >> 8;
    temp |= temp >> 16;
    
    temp  = temp < size ? temp << 1 : temp;
    
    return temp;
}
 
void * FusionStrategyAllocation(uint32_t size)
{
    bool result = false;
    uint8_t *block = NULL;

    if (size < 512) {
        
        /* 最小为8 */
        size = size < 8 ? 8 : size;
        
        result = MemoryManageSlabTake(CalculateTouch2Pow(size), &block);
        
        if (result == true)
            return block;
    }

    if (size < 4 * 1024) {
        
        /* 最小为512 */
        size = size < 512 ? 512 : size;
        
        if (CalculateTouch2Pow(size) < size + 4)
            size += 4;
        
        result = MemoryManageSlabTake(CalculateTouch2Pow(size), &block);
        
        if (result == true)
        {
            *(uint32_t *)block = CalculateTouch2Pow(size);
            
            return block + 4;
        }
    }
    
    if (size >= 4 * 1024) {
        result = MemoryManageHeapTake(size, *block);
        
        if (result == false)
            return NULL;
        if (result == true)
            return block;
    }
}

void FusionStrategyFree(void *pointer)
{
    if (true == MemoryManageSlabGive(0, (uint8_t *)pointer))
        return;
    if (true == MemoryManageHeapGive(0, (uint8_t *)pointer))
        return;
    if (true == MemoryManageBuddyGive(((uint32_t*)pointer)[-1], (uint8_t *)pointer - 4));
        return;
}

int main(int argc, char *argv[])
{
    MemoryManageConfigure();
    
    //test_slab_distributor();
    //test_buddy_distributor();
    //test_heap_distributor();
    
    return 0;
}



