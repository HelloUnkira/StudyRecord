/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "HeapStrategy.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 块的布局规则(堆内存):
 * MMHS_MemoryAddressStart  (sizeof(uintptr_t) Alignment) ~
 * MMHS_MemoryAddressEnd    (sizeof(uintptr_t) Alignment)
 * 单元块对齐的内存空间被切分成全部数量的单元块密集排列:
 * MemoryZoneLayout:        [uintptr_t * Number] (Number = (End - Start) / uintptr_t)
 * 块资源的描述:
 * [uintptr_t * (1或2) == [MMHS_ChunkField:0,1,2,3]][uintptr_t * (N - (1或2))]
 * 块属性集合描述在块头:Chunk[0]
 * [MMHS_Chunk == uintptr_t * X][Buckets * Y]   (X + Y == Number)
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_RoundHigh(uintptr_t Memory, uintptr_t Alignment)
{
    return ((uintptr_t)Memory + (Alignment - 1)) & ~(Alignment - 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_RoundLow(uintptr_t Memory, uintptr_t Alignment)
{
    return ((uintptr_t)Memory) & ~(Alignment - 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef enum MMHS_ChunkFieldDistributor {
    MMHS_ChunkFieldSizeAndUsed = 0, /* 压缩:最后一位作为Chunk使用标志位 */
    MMHS_ChunkFieldPrevFree    = 1,
    MMHS_ChunkFieldNextFree    = 2,
    MMHS_ChunkFieldNearSize    = 3, /* 因为数组特性,这里默认指LeftSize */
    MMHS_ChunkFieldNumber,
} MMHS_ChunkField;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MMHS_ChunkBitsDeclare(Bits)                 \
typedef struct MMHS_Chunk##Bits##Distributor {      \
    uint##Bits##_t Field[MMHS_ChunkFieldNumber];    \
} MMHS_Chunk##Bits;                                 \
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MMHS_ChunkBitsDeclare(8);
MMHS_ChunkBitsDeclare(16);
MMHS_ChunkBitsDeclare(32);
MMHS_ChunkBitsDeclare(64);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MMHS_ChunkSetDistributor {   /* 块集合描述符 */
    uintptr_t ChunkNumber;                  /* 单元块数量 */
    uintptr_t BucketsAvail;                 /* 块空闲链表是否有块(位标记) */
    uintptr_t Buckets[0];                   /* 块空闲链表(以2的指数次方分割) */
} MMHS_ChunkSet;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MMHS_ChunkUnit          uint64_t    /* Chunk的单元量,默认固定取最大 */
#define MMHS_ChunkSize          sizeof(MMHS_ChunkUnit)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MMHS_ChunkAllocLoops    3
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通过块的数量信息判断以确定块域的大小 */
static inline uintptr_t MMHS_ChunkTypeGetSize(MMHS_ChunkSet *Set)
{   /* 根据ChunkNumer的大小确认Chunk类型,它随MMHS_ChunkUnit的变化而变化 */
    if (Set->ChunkNumber < 0x7F)
        return sizeof(MMHS_Chunk8);
    if (Set->ChunkNumber < 0x7FFF)
        return sizeof(MMHS_Chunk16);
    if (Set->ChunkNumber < 0x7FFFFFFF)
        return sizeof(MMHS_Chunk32);
    if (Set->ChunkNumber < 0x7FFFFFFFFFFFFFFF)
        return sizeof(MMHS_Chunk64);
    return 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_ChunkTypeGetAlignSize(MMHS_ChunkSet *Set)
{   /* Size需要向上取整到uintptr_t */
    return MMHS_RoundHigh(MMHS_ChunkTypeGetSize(Set), MMHS_ChunkSize);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline MMHS_ChunkUnit * MMHS_ChunkUnitGet(MMHS_ChunkSet *Set)
{   /* 注意:Set本身仅是第0个Chunk的数据段起始点,需要偏移到Chunk0的段起始点 */
    /* 因为Chunk本身的索引是完整映射到MMHS_ChunkUnit上,所以它可以无缝转换到该类型 */
    return (MMHS_ChunkUnit *)((uint8_t *)Set - MMHS_ChunkTypeGetAlignSize(Set));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 块域属性基础语义:设置域参数和获取域参数 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void MMHS_ChunkSetField(MMHS_ChunkSet  *Set,   uintptr_t Chunk,
                                      MMHS_ChunkField Index, uintptr_t Value)
{   /* 找到块中指定域映射的地址 */
    void *Memory = &MMHS_ChunkUnitGet(Set)[Chunk];
    /* 检查资源堆堆类型,从基准偏移量取得对应域 */
    MMHS_Check(Chunk > Set->ChunkNumber);
    switch (MMHS_ChunkTypeGetSize(Set)) {
    case sizeof(MMHS_Chunk8):
        (( MMHS_Chunk8 *)Memory)->Field[Index] = Value;
        MMHS_Check(Value !=  (uint8_t)Value);
        return;
    case sizeof(MMHS_Chunk16):
        ((MMHS_Chunk16 *)Memory)->Field[Index] = Value;
        MMHS_Check(Value != (uint16_t)Value);
        return;
    case sizeof(MMHS_Chunk32):
        ((MMHS_Chunk32 *)Memory)->Field[Index] = Value;
        MMHS_Check(Value != (uint32_t)Value);
        return;
    case sizeof(MMHS_Chunk64):
        ((MMHS_Chunk64 *)Memory)->Field[Index] = Value;
        MMHS_Check(Value != (uint64_t)Value);
        return;
    default :MMHS_Check(true);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_ChunkGetField(MMHS_ChunkSet  *Set, uintptr_t Chunk,
                                           MMHS_ChunkField Index)
{   /* 找到块中指定域映射的地址 */
    void *Memory = &MMHS_ChunkUnitGet(Set)[Chunk];
    /* 检查资源堆堆类型,从基准偏移量取得对应域 */
    switch (MMHS_ChunkTypeGetSize(Set)) {
    case sizeof(MMHS_Chunk8):  return (( MMHS_Chunk8 *)Memory)->Field[Index];
    case sizeof(MMHS_Chunk16): return ((MMHS_Chunk16 *)Memory)->Field[Index];
    case sizeof(MMHS_Chunk32): return ((MMHS_Chunk32 *)Memory)->Field[Index];
    case sizeof(MMHS_Chunk64): return ((MMHS_Chunk64 *)Memory)->Field[Index];
    default :MMHS_Check(true); return 0;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void MMHS_ChunkSetPrevFree(MMHS_ChunkSet *Set, uintptr_t Chunk, uintptr_t Free)
{
    MMHS_ChunkSetField(Set, Chunk, MMHS_ChunkFieldPrevFree, Free);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void MMHS_ChunkSetNextFree(MMHS_ChunkSet *Set, uintptr_t Chunk, uintptr_t Free)
{
    MMHS_ChunkSetField(Set, Chunk, MMHS_ChunkFieldNextFree, Free);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void MMHS_ChunkSetSize(MMHS_ChunkSet *Set, uintptr_t Chunk, uintptr_t Size)
{
    /* 注意: 这里不需要保留已使用的位,因为当修改块的大小时它永远不会被使用 */
    /*       并在MMHS_ChunkSetSize之后,总是会潜在的调用MMHS_ChunkSetUsed */
    MMHS_ChunkSetField(Set, Chunk, MMHS_ChunkFieldSizeAndUsed, Size << 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void MMHS_ChunkSetUsed(MMHS_ChunkSet *Set, uintptr_t Chunk, bool Used)
{
    uintptr_t Value = MMHS_ChunkGetField(Set, Chunk, MMHS_ChunkFieldSizeAndUsed);
    MMHS_ChunkSetField(Set, Chunk, MMHS_ChunkFieldSizeAndUsed, Used ? Value | 1U : Value & ~1U);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void MMHS_ChunkSetNearSize(MMHS_ChunkSet *Set, uintptr_t Chunk, uintptr_t Size)
{
    MMHS_ChunkSetField(Set, Chunk, MMHS_ChunkFieldNearSize, Size);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_ChunkGetPrevFree(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    return MMHS_ChunkGetField(Set, Chunk, MMHS_ChunkFieldPrevFree);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_ChunkGetNextFree(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    return MMHS_ChunkGetField(Set, Chunk, MMHS_ChunkFieldNextFree);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool MMHS_ChunkGetUsed(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    return MMHS_ChunkGetField(Set, Chunk, MMHS_ChunkFieldSizeAndUsed) & 1U;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_ChunkGetSize(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    return MMHS_ChunkGetField(Set, Chunk, MMHS_ChunkFieldSizeAndUsed) >> 1;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_ChunkGetLeft(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    return Chunk - MMHS_ChunkGetField(Set, Chunk, MMHS_ChunkFieldNearSize);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_ChunkGetRight(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    return Chunk + MMHS_ChunkGetSize(Set, Chunk);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 块空间布局占用 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t MMHS_ChunkByteToBucketIndex(MMHS_ChunkSet *Set, uintptr_t Size)
{
    for (uint32_t Index = 0; Index < sizeof(uintptr_t) * 8; Index++)
        if ((1 << (sizeof(uintptr_t) * 8 - 1 - Index)) & (Size + 1))
            return sizeof(uintptr_t) * 8 - 1 - Index;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 块基础属性逻辑 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void MMHS_FreeListRemoveBucketIndex(MMHS_ChunkSet *Set, uintptr_t Chunk, uintptr_t Index)
{
    uintptr_t *Bucket = &Set->Buckets[Index];
    MMHS_Check(MMHS_ChunkGetUsed(Set, Chunk));
    MMHS_Check(*Bucket == 0);
    MMHS_Check((Set->BucketsAvail & (1 << Index)) == 0);
    /* 循环链表中如果只剩下一项Chunk时 */
    if (MMHS_ChunkGetNextFree(Set, Chunk) == Chunk) {
        Set->BucketsAvail &= ~(1 << Index);
        *Bucket = 0;
    } else {
        /* 更新Chunk的前向索引以及后项索引 */
        uintptr_t First  = MMHS_ChunkGetPrevFree(Set, Chunk);
        uintptr_t Second = MMHS_ChunkGetNextFree(Set, Chunk);
        MMHS_ChunkSetNextFree(Set, First, Second);
        MMHS_ChunkSetPrevFree(Set, Second, First);
        *Bucket = Second;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void MMHS_FreeListRemove(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    uintptr_t Index = MMHS_ChunkByteToBucketIndex(Set, MMHS_ChunkGetSize(Set, Chunk));
    MMHS_FreeListRemoveBucketIndex(Set, Chunk, Index);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void MMHS_FreeListAddBucketIndex(MMHS_ChunkSet *Set, uintptr_t Chunk, uintptr_t Index)
{
    uintptr_t *Bucket = &Set->Buckets[Index];
    /* 循环链表中如果一项都没有时 */
    if (*Bucket == 0U) {
        MMHS_Check((Set->BucketsAvail & (1 << Index)) != 0);
        *Bucket = Chunk;
        Set->BucketsAvail |= (1 << Index);
        MMHS_ChunkSetPrevFree(Set, Chunk, Chunk);
        MMHS_ChunkSetNextFree(Set, Chunk, Chunk);
    } else {
        MMHS_Check((Set->BucketsAvail & (1 << Index)) == 0);
        /* 插入节点在Near索引之前 */
        uintptr_t Second = *Bucket;
        uintptr_t First  = MMHS_ChunkGetPrevFree(Set, Second);
        MMHS_ChunkSetPrevFree(Set, Chunk,  First);
        MMHS_ChunkSetNextFree(Set, Chunk,  Second);
        MMHS_ChunkSetNextFree(Set, First,  Chunk);
        MMHS_ChunkSetPrevFree(Set, Second, Chunk);        
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void MMHS_FreeListAdd(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    uintptr_t Index = MMHS_ChunkByteToBucketIndex(Set, MMHS_ChunkGetSize(Set, Chunk));
    MMHS_FreeListAddBucketIndex(Set, Chunk, Index);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void MMHS_ChunkSplit(MMHS_ChunkSet *Set, uintptr_t Chunk1, uintptr_t Chunk2)
{
    /* 计算新Chunk1和Chunk2的大小 */
    uintptr_t Chunk3 = 0;
    uintptr_t Size0 = MMHS_ChunkGetSize(Set, Chunk1);
    uintptr_t Size1 = Chunk2 - Chunk1;
    uintptr_t Size2 = Size0  - Size1;
    /* 检查Chunk1和Chunk2 */
    MMHS_Check(Chunk2 <= Chunk1);
    MMHS_Check(Chunk2  - Chunk1 >= Size0);
    /* 拆分Chunk1 */
    MMHS_ChunkSetSize(Set, Chunk1, Size1);
    MMHS_ChunkSetSize(Set, Chunk2, Size2);
    Chunk3 = MMHS_ChunkGetRight(Set, Chunk2);
    MMHS_ChunkSetNearSize(Set, Chunk2, Size1);
    MMHS_ChunkSetNearSize(Set, Chunk3, Size2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void MMHS_ChunkMerge(MMHS_ChunkSet *Set, uintptr_t Chunk1, uintptr_t Chunk2)
{
    uintptr_t Chunk3 = 0;
    uintptr_t Size = MMHS_ChunkGetSize(Set, Chunk1) + MMHS_ChunkGetSize(Set, Chunk2);
    MMHS_ChunkSetSize(Set, Chunk1, Size);
    Chunk3 = MMHS_ChunkGetRight(Set, Chunk2);
    MMHS_ChunkSetNearSize(Set, Chunk3, Size);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void MMHS_ChunkFree(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    /* 先尝试性的向右合并 */
    uintptr_t Right = MMHS_ChunkGetRight(Set, Chunk);
    if (!MMHS_ChunkGetUsed(Set, Right)) {
        MMHS_FreeListRemove(Set, Right);
        MMHS_ChunkMerge(Set, Chunk, Right);
    }
    /* 后尝试性的向左合并 */
    uintptr_t Left  = MMHS_ChunkGetLeft(Set, Chunk);
    if (!MMHS_ChunkGetUsed(Set, Left)) {
        MMHS_FreeListRemove(Set, Left);
        MMHS_ChunkMerge(Set, Left, Chunk);
        Chunk = Left;
    }
    /* 进行最后一次释放,将其加入到循环链表 */
    MMHS_FreeListAdd(Set, Chunk);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static uintptr_t MMHS_ChunkAlloc(MMHS_ChunkSet *Set, uintptr_t Size)
{
    uintptr_t  Index  = MMHS_ChunkByteToBucketIndex(Set, Size);
    uintptr_t *Bucket = &Set->Buckets[Index];
    MMHS_Check(Index > MMHS_ChunkByteToBucketIndex(Set, Set->ChunkNumber));
    /* 尝试性的在循环链表中命中一个 */
    if (*Bucket != 0) {
        uintptr_t Record = *Bucket;
        uintptr_t Count =  MMHS_ChunkAllocLoops;
        do {
            uintptr_t Chunk = *Bucket;
            if (MMHS_ChunkGetSize(Set, Chunk) >= Size) {
                MMHS_FreeListRemoveBucketIndex(Set, Chunk, Index);
                return Chunk;
            }
            /* 让这个循环链表转起来 */
            *Bucket = MMHS_ChunkGetNextFree(Set, Chunk);
            MMHS_Check(*Bucket == 0);
        } while (--Count != 0 && *Bucket != Record);
    }
    /* 从最小的一个单元内提取一个非空的块,并拆碎它 */
    uintptr_t BitsMask = Set->BucketsAvail & ~((1 << (Index + 1)) - 1);
    if (BitsMask != 0) {
        for (uint32_t Offset = 0; Offset < sizeof(uintptr_t) * 8; Offset++)
            if ((1 <<  (sizeof(uintptr_t) * 8 - 1 - Offset)) & BitsMask) {
                Index = sizeof(uintptr_t) * 8 - 1 - Offset;
                break;
            }
        Bucket = &Set->Buckets[Index];
        uintptr_t Chunk = *Bucket;
        MMHS_FreeListRemoveBucketIndex(Set, Chunk, Index);
        MMHS_Check(Size > MMHS_ChunkGetSize(Set, Chunk));
        return Chunk;
    }
    /* 找不到一个有效的块 */
    return 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 内存分配接口 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static uintptr_t MMHS_MemoryToChunk(MMHS_ChunkSet *Set, void *Memory)
{
    uintptr_t Chunk = (uintptr_t)Memory - (uintptr_t)MMHS_ChunkUnitGet(Set);
    Chunk -= MMHS_ChunkTypeGetAlignSize(Set);
    Chunk /= MMHS_ChunkSize;
    return Chunk;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void * MMHS_ChunkToMemory(MMHS_ChunkSet *Set, uintptr_t Chunk)
{
    uint8_t *Memory = (uint8_t *)&MMHS_ChunkUnitGet(Set)[Chunk];
    Memory += MMHS_ChunkTypeGetAlignSize(Set);
    MMHS_Check((((uintptr_t)Memory) & (MMHS_ChunkSize - 1)) != 0);
    return (void *)Memory;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static uintptr_t MMHS_ByteToChunkSize(MMHS_ChunkSet *Set, uintptr_t Bytes)
{
    /* 将目标空间对齐到MMHS_ChunkUnit上并加上头的长度 */
    Bytes  = MMHS_RoundHigh(Bytes, MMHS_ChunkSize);
    Bytes += MMHS_ChunkTypeGetAlignSize(Set);
    MMHS_Check(((Bytes) & (MMHS_ChunkSize - 1)) != 0);
    return Bytes / MMHS_ChunkSize;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
int8_t MMHS_Free(HeapStrategy *Instance, void *Memory)
{
    if ((uintptr_t)Memory == 0)
        return -1;
    MMHS_ChunkSet *Set = (MMHS_ChunkSet *)Instance->Manage;
    uintptr_t Chunk = MMHS_MemoryToChunk(Set, Memory);
    if (!MMHS_ChunkGetUsed(Set, Chunk))
        return -2;  //意外的堆状态(二次释放?)
    if (MMHS_ChunkGetLeft(Set, MMHS_ChunkGetRight(Set, Chunk)) != Chunk ||
        MMHS_ChunkGetRight(Set, MMHS_ChunkGetLeft(Set, Chunk)) != Chunk)
        return -3;  //损坏的堆边界(缓冲溢出?)
    MMHS_ChunkSetUsed(Set, Chunk, false);
    MMHS_ChunkFree(Set, Chunk);
    return 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * MMHS_Alloc(HeapStrategy *Instance, uintptr_t Bytes)
{
    MMHS_ChunkSet *Set = (MMHS_ChunkSet *)Instance->Manage;
    /* 分配长度容错检查 */
    if (Bytes == 0 || Bytes / MMHS_ChunkSize >= Set->ChunkNumber)
        return NULL;
    uintptr_t Size = MMHS_ByteToChunkSize(Set, Bytes);
    uintptr_t Chunk = MMHS_ChunkAlloc(Set, Size);
    if (Chunk == 0)
        return NULL;
    /* 将剩余的部分切开(保证剩下的空间至少成一个头) */
    if (MMHS_ChunkGetSize(Set, Chunk) > Size + MMHS_ChunkTypeGetAlignSize(Set)) {
        MMHS_ChunkSplit(Set, Chunk, Chunk + Size);
        MMHS_FreeListAdd(Set, Chunk + Size);
    }
    MMHS_ChunkSetUsed(Set, Chunk, true);
    return MMHS_ChunkToMemory(Set, Chunk);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
int8_t MMHS_Init(HeapStrategy *Instance, void *Memory, uintptr_t Bytes)
{
    /* 堆太大检查 */
    if (Bytes / MMHS_ChunkSize > ~(1 << (sizeof(uintptr_t) * 8 - 1)))
        return -1;
    /* 堆太小检查 */
    if (Bytes / MMHS_ChunkSize < sizeof(MMHS_Chunk8) * 4 * 2)
        return -2;
    /* 地址空间转化 */
    uintptr_t SetSize  = MMHS_RoundHigh(sizeof(MMHS_ChunkSet),     MMHS_ChunkSize);
    uintptr_t AddrHdr  = MMHS_RoundHigh((uintptr_t)Memory,         MMHS_ChunkSize);
    uintptr_t AddrEnd  = MMHS_RoundLow(( uintptr_t)Memory + Bytes, MMHS_ChunkSize);
    uintptr_t OldSize  = (AddrEnd - AddrHdr) / MMHS_ChunkSize;
    MMHS_Check(AddrHdr >= AddrEnd);
    MMHS_ChunkSet Old  = {.ChunkNumber = OldSize,};
    uintptr_t TypeSize = MMHS_ChunkTypeGetAlignSize(&Old);
    /* 计算切除头尾Chunk头和Set的大小后的Size */
    uintptr_t NewSize  = OldSize - (2 * TypeSize + SetSize) / MMHS_ChunkSize;
    MMHS_ChunkSet *Set = (MMHS_ChunkSet *)(AddrHdr + TypeSize);
    Instance->Manage   = (uintptr_t)Set;
    Instance->Memory   = AddrHdr;
    Instance->Bytes    = OldSize * MMHS_ChunkSize;
    Set->ChunkNumber   = OldSize;
    Set->BucketsAvail  = 0;
    uintptr_t BktIndex = MMHS_ChunkByteToBucketIndex(Set, NewSize) + 1;
    uintptr_t BktSize  = MMHS_RoundHigh(BktIndex * sizeof(uintptr_t), MMHS_ChunkSize);
    /* 初始化循环空闲链表 */
    for (uintptr_t Index = 0; Index < BktIndex; Set->Buckets[Index] = 0, Index++);
    /* 将该空间切碎成三块,头,中,尾 */
    uintptr_t Chunk0   = 0;
    uintptr_t Size0    = (TypeSize + SetSize + BktSize) / MMHS_ChunkSize;
    MMHS_ChunkSetSize(Set, Chunk0, Size0);
    MMHS_ChunkSetNearSize(Set, Chunk0, 0);
    MMHS_ChunkSetUsed(Set, Chunk0, true);
    uintptr_t Chunk1   = Chunk0 + Size0;
    uintptr_t Size1    = OldSize - Size0 - TypeSize / MMHS_ChunkSize;
    MMHS_ChunkSetSize(Set, Chunk1, Size1);
    MMHS_ChunkSetNearSize(Set, Chunk1, Size0);
    MMHS_ChunkSetUsed(Set, Chunk1, false);
    uintptr_t Chunk2   = Chunk1 + Size1;
    uintptr_t Size2    = TypeSize / MMHS_ChunkSize;
    MMHS_ChunkSetSize(Set, Chunk2, Size2);
    MMHS_ChunkSetNearSize(Set, Chunk2, Size1);
    MMHS_ChunkSetUsed(Set, Chunk2, true);
    /* 将中间的大块加入到循环链表中 */
    MMHS_FreeListAdd(Set, Chunk1);
    return 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MMHS_LayoutCheck(HeapStrategy *Instance)
{
    MMHS_ChunkSet *Set = (MMHS_ChunkSet *)Instance->Manage;
    
    MMHS_Visit("HeapStrategy:\n");
    MMHS_Visit("Manage:%p\n",       Instance->Manage);
    MMHS_Visit("Memory:%x\n",       Instance->Memory);
    MMHS_Visit("Bytes:%d\n",        Instance->Bytes);
    
    MMHS_Visit("ChunkSet:\n");
    MMHS_Visit("ChunkNumber:%u\n",  Set->ChunkNumber);
    MMHS_Visit("BucketsAvail:%x\n", Set->BucketsAvail);
    MMHS_Visit("ChunkType:%u\n",    MMHS_ChunkTypeGetAlignSize(&Set));
    
    for (uintptr_t Chunk = 0; Chunk < Set->ChunkNumber; ) {
        MMHS_Visit("\tChunk:%u\n",  Chunk);
        MMHS_Visit("\tSize:%u\n",   MMHS_ChunkGetSize(Set, Chunk));
        MMHS_Visit("\tUsed:%u\n",   MMHS_ChunkGetUsed(Set, Chunk));
        MMHS_Visit("\tPrev:%u\n",   MMHS_ChunkGetPrevFree(Set, Chunk));
        MMHS_Visit("\tNext:%u\n",   MMHS_ChunkGetNextFree(Set, Chunk));
        MMHS_Visit("\tLeft:%u\n",   MMHS_ChunkGetLeft(Set, Chunk));
        MMHS_Visit("\tRight:%u\n",  MMHS_ChunkGetRight(Set, Chunk));
        Chunk += MMHS_ChunkGetSize(Set, Chunk);
    }
    
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
