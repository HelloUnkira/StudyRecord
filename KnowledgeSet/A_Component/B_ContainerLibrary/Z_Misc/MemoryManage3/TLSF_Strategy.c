/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "TLSF_Strategy.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define TLSF_MAX(X, Y)  ((X) > (Y) ? (X) : (Y))
#define TLSF_MIN(X, Y)  ((X) < (Y) ? (X) : (Y))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t TLSF_RoundHigh(uintptr_t Memory, uintptr_t Alignment)
{
    return ((uintptr_t)Memory + (Alignment - 1)) & ~(Alignment - 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t TLSF_RoundLow(uintptr_t Memory, uintptr_t Alignment)
{
    return ((uintptr_t)Memory) & ~(Alignment - 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t TLSF_FFS(uintptr_t Word)
{   /* __bulitin_ffs:从低位起,第一个1的索引+1,为0则返回0 */
    /* 此处的语义有变化,因为明确参数不为0,这里索引不+1 */
    TLSF_Check(Word != 0);
    
    for (uintptr_t Idx = 0; Idx < sizeof(uintptr_t) * 8; Idx++)
        if ((Word & (1 << Idx)) != 0)
            return Idx;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t TLSF_FLS(uintptr_t Word)
{   /* __bulitin_fls:从高位起,第一个1的位置+1 */
    /* 此处的语义有变化,因为明确参数不为0,这里索引不+1 */
    TLSF_Check(Word != 0);
    
    for (uintptr_t Idx = 0; Idx < sizeof(uintptr_t) * 8; Idx++)
        if ((Word & (1 << (sizeof(uintptr_t) * 8 - 1 - Idx))) != 0)
            return sizeof(uintptr_t) * 8 - 1 - Idx;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 公共常量:可以修改 */
typedef enum TLSF_PublicDistributor {
    /* 二级块长度线性细分值的Log2,4~5是典型值 */
    TLSF_SL_Log2 = 5,
    /*  */
} TLSF_Public;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 私有常量:不可修改 */
typedef enum TLSF_PrivateDistributor {
    /* 字节对齐大小 */
    TLSF_AlignSize = sizeof(void *),
    /* 字节对齐,只能支持32位或者64位平台 */
    TLSF_AlignByte = TLSF_AlignSize == 8 ? 3 :
                     TLSF_AlignSize == 4 ? 2 : 0,
    /* 一级块数量最大值Log2,修改它以支持更大的块尺寸 */
    TLSF_FL_Index  = TLSF_AlignSize == 8 ? 32 :
                     TLSF_AlignSize == 4 ? 30 : 0,
    /* 一级块偏移量 */
    TLSF_FL_Shift = TLSF_AlignByte + TLSF_SL_Log2,
    /* 一级块数量 */
    TLSF_FL_Count = TLSF_FL_Index - TLSF_FL_Shift + 1,
    /* 二级块数量 */
    TLSF_SL_Count = 1 << TLSF_SL_Log2,
    /* 一级块大小(最小) */
    TLSF_FL_Lower = 1 << TLSF_FL_Shift,
    /*  */
} TLSF_Private;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//静态断言, 编译器优化
//#error "can't supported, only 4 byte align or 8 byte align"
static uint8_t TLSF_StaticAssert_AlignSize[(TLSF_AlignSize == 8 || TLSF_AlignSize == 4) ? 1 : -1] = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 块关联结构 */
typedef struct TLSF_BlockDistributor {
    /* 该字段前一块空闲时有效 */
    /* 并且该字段有效时嵌入到前一个块的末尾 */
    struct TLSF_BlockDistributor *PrevTail;
    /* 包括本身以及后面跟随的整体块的大小 */
    /* 因为至少四字节对齐,最低位二次复用 */
    /* 最低位:当前块是忙碌还是空闲态 */
    /* 次低位:前一块是忙碌还是空闲态 */
    uintptr_t Size;
    /* 下一块与前一块(低俩位可复用:未使用) */
    struct TLSF_BlockDistributor *Prev;
    struct TLSF_BlockDistributor *Next;
} TLSF_Block;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 有效块:因为TLSK_Block.PrevTail不在当前块,需要扣除 */
static const uintptr_t TLSF_BlockMin = sizeof(TLSF_Block) - sizeof(TLSF_Block *);
static const uintptr_t TLSF_BlockMax = (uintptr_t)1 << TLSF_FL_Index;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 内部块:分给用户时要扣除这部分,在头部进行偏移 */
static const uintptr_t TLSF_BlockOccupy = sizeof(uintptr_t);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 用户块:分配给用户时进行的地址偏移量 */
static inline uintptr_t TLSF_BlockOffset(void)
{
    return (uintptr_t)&(((TLSF_Block *)0)->Size) + sizeof(uintptr_t);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void TLSF_BlockSetSize(TLSF_Block *Block, uintptr_t Size)
{   /* 块占用实际大小设置 */
    Block->Size = (Block->Size & ((1 << 0) | (1 << 1))) | (Size & ~((1 << 0) | (1 << 1)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void TLSF_BlockSetCurrFree(TLSF_Block *Block, bool Free)
{   /* 设置当前块的空闲状态 */
    Block->Size = Free ? Block->Size | (1 << 0) : Block->Size & ~(1 << 0);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void TLSF_BlockSetPrevFree(TLSF_Block *Block, bool Free)
{   /* 设置前一块的空闲状态 */
    Block->Size = Free ? Block->Size | (1 << 1) : Block->Size & ~(1 << 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t TLSF_BlockGetSize(TLSF_Block *Block)
{   /* 块占用实际大小获取 */
    return Block->Size & ~((1 << 0) | (1 << 1));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool TLSF_BlockGetCurrFree(TLSF_Block *Block)
{   /* 获取当前块的空闲状态 */
    return (Block->Size & (1 << 0)) != 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool TLSF_BlockGetPrevFree(TLSF_Block *Block)
{   /* 获取前一块的空闲状态 */
    return (Block->Size & (1 << 1)) != 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 块控制结构 */
typedef struct TLSF_ControlDistributor {
    /* 空列表指向这个块以表示它们是空闲的 */
    TLSF_Block BlockNull;
    /* 空闲列表使用的位图 */
    uintptr_t FL_Bitmap;
    uintptr_t SL_Bitmap[TLSF_FL_Count];
    /* 空闲列表 */
    TLSF_Block * Blocks[TLSF_FL_Count][TLSF_SL_Count];
} TLSF_Control;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline TLSF_Block * TLSF_BlockFromPointer(void *Pointer)
{   /* 从用户块模式切换到内部块(以块描述符顶为基准) */
    return (TLSF_Block *)((uintptr_t)Pointer - TLSF_BlockOffset());
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * TLSF_BlockToPointer(TLSF_Block *Block)
{   /* 从内部块模式切换到用户块(以块描述符顶为基准) */
    return (TLSF_Block *)((uintptr_t)Block + TLSF_BlockOffset());
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline TLSF_Block * TLSF_BlockByPointer(void *Pointer, uintptr_t Size)
{   /* 从用户块模式切换到下一内部块 */
    return (TLSF_Block *)((uintptr_t)Pointer + Size);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline TLSF_Block * TLSF_BlockGetPrev(TLSF_Block *Block)
{   /* 获得前一块 */
    TLSF_Check(TLSF_BlockGetPrevFree(Block));
    return Block->PrevTail;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline TLSF_Block * TLSF_BlockGetNext(TLSF_Block *Block)
{   /* 获得后一块 */
    TLSF_Check(TLSF_BlockGetSize(Block) != 0);
    /* 地址偏移计算1: */
    return (TLSF_Block *)((uintptr_t)&Block->Size + TLSF_BlockGetSize(Block));
    /* 地址偏移计算2: */
    return TLSF_BlockByPointer(TLSF_BlockToPointer(Block),
                               TLSF_BlockGetSize(Block) - TLSF_BlockOccupy);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline TLSF_Block * TLSF_BlockLinkNext(TLSF_Block *Block)
{   /* 当前块关联下一个块 */
    TLSF_Block *Next = TLSF_BlockGetNext(Block);
    Next->PrevTail = Block;
    return Next;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline TLSF_Block * TLSF_BlockMark(TLSF_Block *Block, bool Free)
{   /* 标记块空闲或者使用状态 */
    TLSF_Block *Next = Free ? TLSF_BlockLinkNext(Block) : TLSF_BlockGetNext(Block);
    TLSF_BlockSetPrevFree(Next,  Free);
    TLSF_BlockSetCurrFree(Block, Free);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static uintptr_t TLSF_AdjustRequestSize(uintptr_t Size, uintptr_t Align)
{   /* 调整分配大小使之与字大小一致,不能更小于内部最小值 */
    uintptr_t AlignedSize = Size != 0 ? TLSF_RoundHigh(Size, Align) : 0;
    AlignedSize = AlignedSize < TLSF_BlockMax ? TLSF_MAX(AlignedSize, TLSF_BlockMin) : 0;
    return AlignedSize;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void TLSF_Mapping(uintptr_t Size, uintptr_t *Fli, uintptr_t *Sli)
{
    uintptr_t FL = 0;
    uintptr_t SL = 0;
    /* 小于最小块时收集到第0块去 */
    if (Size < TLSF_FL_Lower) {
        FL  = 0;
        SL  = Size / (TLSF_FL_Lower / TLSF_SL_Log2);
    } else {
        FL  = TLSF_FLS(Size);
        SL  = (Size >> (FL - TLSF_SL_Log2)) ^ ((uintptr_t)1 << TLSF_SL_Log2);
        FL -= TLSF_FL_Shift - 1;
    }
    *Fli = FL;
    *Sli = SL;
    /* 检查一下插入时映射: */
    // TLSF_Visit("TLSF_Mapping:\r\n");
    // TLSF_Visit("Size:%d FL:%d SL:%d \r\n", Size, *Fli, *Sli);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void TLSF_MappingSearch(uintptr_t Size, uintptr_t *Fli, uintptr_t *Sli)
{
    if (Size >= TLSF_FL_Lower)
        Size += ((uintptr_t)1 << (TLSF_FLS(Size) - TLSF_SL_Log2)) - 1;
    TLSF_Mapping(Size, Fli, Sli);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static TLSF_Block * TLSF_BlockFindSuitbale(TLSF_Control *Control, uintptr_t *Fli, uintptr_t *Sli)
{
    uintptr_t FL = *Fli, FL_Map = 0;
    uintptr_t SL = *Sli, SL_Map = 0;
    
    /* 查找二级索引图,注意:比目标索引大的索引都需要找到,以便迭代 */
    SL_Map = Control->SL_Bitmap[FL] & ((uintptr_t)(~0) << (SL + 0));
    /* 当前一级索引下,查找不到比目标大的二级索引图,查找一级索引图 */
    if (SL_Map == 0) {
        FL_Map  = Control->FL_Bitmap & ((uintptr_t)(~0) << (FL + 1));
        /* 一级索引图也找不到有效块,内存被耗尽了 */
        if (FL_Map == 0)
            return NULL;
        /* 在一级索引图找到最邻近的下一级索引 */
        FL = TLSF_FFS(FL_Map);
        SL_Map = Control->SL_Bitmap[FL];
    }
    /* 在二级索引图找到最邻近的下一级索引 */
    SL = TLSF_FFS(SL_Map);
    /* 索引可能会调整,更新它 */
    *Fli = FL;
    *Sli = SL;
    /* 返回空闲链表 */
    return Control->Blocks[FL][SL];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void TLSF_BlockRemoveFree(TLSF_Control *Control, TLSF_Block *Block, uintptr_t FL, uintptr_t SL)
{
    /* 即使是链表头和链表尾都会指向一个空节点 */
    /* 所以前向索引和后向索引都不能为空 */
    TLSF_Check(Block->Prev != NULL);
    TLSF_Check(Block->Next != NULL);
    Block->Prev->Next = Block->Next;
    Block->Next->Prev = Block->Prev;
    /* 更新连接索引以及位索引图 */
    if (Control->Blocks[FL][SL] == Block) {
        Control->Blocks[FL][SL]  = Block->Next;
        /* 索引被更新到了空节点,更新索引图 */
        if (Control->Blocks[FL][SL] == &Control->BlockNull)
            Control->SL_Bitmap[FL] &= ~((uintptr_t)1 << SL);
        if (Control->SL_Bitmap[FL] == 0)
            Control->FL_Bitmap &= ~((uintptr_t)1 << FL);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void TLSF_BlockInsertFree(TLSF_Control *Control, TLSF_Block *Block, uintptr_t FL, uintptr_t SL)
{
    TLSF_Block *Current = Control->Blocks[FL][SL];
    TLSF_Check(Block   != NULL);
    TLSF_Check(Current != NULL);
    /* 更新目标索引 */
    Current->Prev =  Block;
    Block->Next   =  Current;
    Block->Prev   = &Control->BlockNull;
    /* 简要核查一下,块地址要平台字节对齐 */
    TLSF_Check((uintptr_t)TLSF_BlockToPointer(Block) == TLSF_RoundLow(
               (uintptr_t)TLSF_BlockToPointer(Block), TLSF_AlignByte));
    /* 更新连接索引以及位索引图 */
    Control->Blocks[FL][SL] = Block;
    Control->FL_Bitmap     |= (uintptr_t)1 << FL;
    Control->SL_Bitmap[FL] |= (uintptr_t)1 << SL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void TLSF_BlockRemove(TLSF_Control *Control, TLSF_Block *Block)
{
    uintptr_t FL = 0;
    uintptr_t SL = 0;
    TLSF_Mapping(TLSF_BlockGetSize(Block), &FL, &SL);
    TLSF_BlockRemoveFree(Control, Block, FL, SL);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void TLSF_BlockInsert(TLSF_Control *Control, TLSF_Block *Block)
{
    uintptr_t FL = 0;
    uintptr_t SL = 0;
    TLSF_Mapping(TLSF_BlockGetSize(Block), &FL, &SL);
    TLSF_BlockInsertFree(Control, Block, FL, SL);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static bool TLSF_BlockSplitSupport(TLSF_Block *Block, uintptr_t Size)
{
    return TLSF_BlockGetSize(Block) >= sizeof(TLSF_Block) + Size;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static TLSF_Block * TLSF_BlockSplit(TLSF_Block *Block, uintptr_t Size)
{
    /* 计算剩余块的总量并获得断点 */
    TLSF_Block *RemainBlock = TLSF_BlockByPointer(
                              TLSF_BlockToPointer(Block), Size - TLSF_BlockOccupy);
    uintptr_t   RemainSize  = TLSF_BlockGetSize(Block) - (Size + TLSF_BlockOccupy);
    /* 简要核查一下,块地址要平台字节对齐 */
    TLSF_Check((uintptr_t)TLSF_BlockToPointer(RemainBlock) == TLSF_RoundLow(
               (uintptr_t)TLSF_BlockToPointer(RemainBlock), TLSF_AlignByte));
    /* 检查计算时是否发生截断 */
    TLSF_Check(TLSF_BlockGetSize(Block) == RemainSize + Size + TLSF_BlockOccupy);
    TLSF_BlockSetSize(RemainBlock, RemainSize);
    TLSF_Check(TLSF_BlockGetSize(RemainBlock) >= TLSF_BlockMin);
    /* 更新原有块大小 */
    TLSF_BlockSetSize(Block, Size);
    TLSF_BlockMark(RemainBlock, true);
    return RemainBlock;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static TLSF_Block * TLSF_BlockAbsorb(TLSF_Block *Prev, TLSF_Block *Block)
{
    TLSF_Check(TLSF_BlockGetSize(Prev) != 0);
    Prev->Size += TLSF_BlockGetSize(Block) + TLSF_BlockOccupy;
    TLSF_BlockLinkNext(Prev);
    return Prev;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static TLSF_Block * TLSF_BlockMergePrev(TLSF_Control *Control, TLSF_Block *Block)
{
    if (TLSF_BlockGetPrevFree(Block)) {
        TLSF_Block *Prev = TLSF_BlockGetPrev(Block);
        TLSF_Check(Prev != NULL);
        TLSF_Check(TLSF_BlockGetCurrFree(Prev));
        TLSF_BlockRemove(Control, Prev);
        Block = TLSF_BlockAbsorb(Prev, Block);
    }
    return Block;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static TLSF_Block * TLSF_BlockMergeNext(TLSF_Control *Control, TLSF_Block *Block)
{
    TLSF_Block *Next = TLSF_BlockGetNext(Block);
    TLSF_Check(Next != NULL);
    
    if (TLSF_BlockGetCurrFree(Next)) {
        TLSF_Check(TLSF_BlockGetSize(Block) != 0);
        TLSF_BlockRemove(Control, Next);
        Block = TLSF_BlockAbsorb(Block, Next);
    }
    return Block;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void TLSF_BlockTrimFree(TLSF_Control *Control, TLSF_Block *Block, uintptr_t Size)
{
    TLSF_Check( TLSF_BlockGetCurrFree(Block));
    
    if (TLSF_BlockSplitSupport(Block, Size)) {
        TLSF_Block *RemainBlock = TLSF_BlockSplit(Block, Size);
        TLSF_BlockLinkNext(Block);
        TLSF_BlockSetPrevFree(RemainBlock, true);
        TLSF_BlockInsert(Control, RemainBlock);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void TLSF_BlockTrimUsed(TLSF_Control *Control, TLSF_Block *Block, uintptr_t Size)
{
    TLSF_Check(!TLSF_BlockGetCurrFree(Block));
    
    if (TLSF_BlockSplitSupport(Block, Size)) {
        TLSF_Block *RemainBlock = TLSF_BlockSplit(Block, Size);
        TLSF_BlockSetPrevFree(RemainBlock, false);
        
        RemainBlock = TLSF_BlockMergeNext(Control, RemainBlock);
        TLSF_BlockInsert(Control, RemainBlock);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static TLSF_Block * TLSF_BlockTrimFreeLead(TLSF_Control *Control, TLSF_Block *Block, uintptr_t Size)
{
    TLSF_Block *RemainBlock = Block;
    
    if (TLSF_BlockSplitSupport(Block, Size)) {
        RemainBlock = TLSF_BlockSplit(Block, Size - TLSF_BlockOccupy);
        TLSF_BlockSetPrevFree(RemainBlock, true);
        
        TLSF_BlockLinkNext(Block);
        TLSF_BlockInsert(Control, Block);
    }
    return RemainBlock;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static TLSF_Block * TLSF_BlockFreeLocate(TLSF_Control *Control, uintptr_t Size)
{
    uintptr_t FL = 0;
    uintptr_t SL = 0;
    TLSF_Block *Block = NULL;
    
    if (Size) {
        TLSF_MappingSearch(Size, &FL, &SL);
        
        if (FL < TLSF_FL_Count)
            Block = TLSF_BlockFindSuitbale(Control, &FL, &SL);
    }
    
    if (Block != NULL) {
        TLSF_Check(TLSF_BlockGetSize(Block) >= Size);
        TLSF_BlockRemoveFree(Control, Block, FL, SL);
    }
    
    return Block;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void * TLSF_BlockUsedPrepare(TLSF_Control *Control, TLSF_Block *Block, uintptr_t Size)
{
    void *Pointer = NULL;
    
    if (Block != NULL) {
        TLSF_Check(Size != 0);
        
        TLSF_BlockTrimFree(Control, Block, Size);
        TLSF_BlockMark(Block, false);
        Pointer = TLSF_BlockToPointer(Block);
    }
    return Pointer;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void TLSF_ControlPrepare(TLSF_Control *Control)
{
    Control->BlockNull.Prev = &Control->BlockNull;
    Control->BlockNull.Next = &Control->BlockNull;
    
    for (uintptr_t Idx = 0; Idx < TLSF_FL_Count; Idx++)
        Control->SL_Bitmap[Idx] = 0;
        Control->FL_Bitmap = 0;
    
    for (uintptr_t Idx1 = 0; Idx1 < TLSF_FL_Count; Idx1++)
    for (uintptr_t Idx2 = 0; Idx2 < TLSF_SL_Count; Idx2++)
        Control->Blocks[Idx1][Idx2] = &Control->BlockNull;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void TLSF_LayoutCheck(uintptr_t TLSF)
{
    TLSF_Control *Control = (TLSF_Control *)TLSF;
    
    TLSF_Visit("\r\n\r\n");
    TLSF_Visit("Control:\r\n");
    TLSF_Visit("First Map:\r\n");
    for (uintptr_t Idx = 0; Idx < TLSF_FL_Count; Idx++)
        TLSF_Visit("%d", (Control->FL_Bitmap & (1 << Idx)) != 0);
    TLSF_Visit("\r\n");
    TLSF_Visit("Second Map:\r\n");
    for (uintptr_t Idx1 = 0; Idx1 < TLSF_FL_Count; Idx1++) {
    for (uintptr_t Idx2 = 0; Idx2 < TLSF_SL_Count; Idx2++)
        TLSF_Visit("%d", (Control->SL_Bitmap[Idx1] & (1 << Idx2)) != 0);
        TLSF_Visit("\r\n");
    }
    for (uintptr_t Idx1 = 0; Idx1 < TLSF_FL_Count; Idx1++) {
    for (uintptr_t Idx2 = 0; Idx2 < TLSF_SL_Count; Idx2++) {
        if (Control->Blocks[Idx1][Idx2] != &Control->BlockNull) {
            TLSF_Visit("Block List(%d-%d):\r\n", Idx1, Idx2);
            TLSF_Block *Block = NULL;
            for (Block  =  Control->Blocks[Idx1][Idx2];
                 Block != &Control->BlockNull;
                 Block  =  Block->Next) {
                 TLSF_Visit("PrevTail:%p\r\n", Block->PrevTail);
                 TLSF_Visit("Size:%u\r\n",     TLSF_BlockGetSize(Block));
                 TLSF_Visit("CurrFree:%u\r\n", TLSF_BlockGetCurrFree(Block));
                 TLSF_Visit("PrevFree:%u\r\n", TLSF_BlockGetPrevFree(Block));
                 TLSF_Visit("Prev:%p\r\n",     Block->Prev);
                 TLSF_Visit("Next:%p\r\n",     Block->Next);
            }
        }
    }
    }
    TLSF_Visit("\r\n");
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
uintptr_t TLSF_Init(void *Memory, uintptr_t Size)
{
    TLSF_Control *Control = NULL;
    TLSF_Block   *Block   = NULL;
    TLSF_Block   *Next    = NULL;
    
    if ((uintptr_t)Memory % TLSF_AlignSize != 0) {
        void *AlignMemory = (void *)TLSF_RoundHigh((uintptr_t)Memory, TLSF_AlignSize);
        Size  -= (uintptr_t)AlignMemory - (uintptr_t)Memory;
        Memory = AlignMemory;
    }
    
    TLSF_ControlPrepare(Memory);
    Control = Memory;
    Memory  = (void *)((uintptr_t)Memory + sizeof(TLSF_Control));
    Memory  = (void *)TLSF_RoundHigh((uintptr_t)Memory, TLSF_AlignSize);
    Size   -= (uintptr_t)Memory - (uintptr_t)Control;
    
    uintptr_t Reserve = TLSF_BlockOccupy * 2;
    uintptr_t Bytes   = TLSF_RoundLow(Size - Reserve, TLSF_AlignSize);
    
    TLSF_Check(TLSF_BlockMin <= Bytes && Bytes <= TLSF_BlockMax);
    
    Block = TLSF_BlockByPointer(Memory, 0 - TLSF_BlockOccupy);
    TLSF_BlockSetSize(Block, Bytes);
    TLSF_BlockSetCurrFree(Block, true);
    TLSF_BlockSetPrevFree(Block, false);
    TLSF_BlockInsert(Control, Block);
    
    Next = TLSF_BlockLinkNext(Block);
    TLSF_BlockSetSize(Next, 0);
    TLSF_BlockSetCurrFree(Next, false);
    TLSF_BlockSetPrevFree(Next, true);
    
    return (uintptr_t)Control;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * TLSF_Alloc(uintptr_t TLSF, uintptr_t Size)
{
    if (Size == 0)
        return NULL;
    if (Size <= TLSF_AlignSize)
        Size  = TLSF_AlignSize;
    
    TLSF_Control *Control = (TLSF_Control *)TLSF;
    uintptr_t RequestSize = TLSF_AdjustRequestSize(Size, TLSF_AlignSize);
    TLSF_Block   *Block   = TLSF_BlockFreeLocate(Control, RequestSize);
    return TLSF_BlockUsedPrepare(Control, Block, RequestSize);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void TLSF_Free(uintptr_t TLSF, void *Pointer)
{
    if (Pointer == NULL)
        return;
    
    TLSF_Control *Control = (TLSF_Control *)TLSF;
    TLSF_Block   *Block   = TLSF_BlockFromPointer(Pointer);
    TLSF_Check(!TLSF_BlockGetCurrFree(Block));
    TLSF_BlockMark(Block, true);
    Block = TLSF_BlockMergePrev(Control, Block);
    Block = TLSF_BlockMergeNext(Control, Block);
    TLSF_BlockInsert(Control, Block);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * TLSF_ReAlloc(uintptr_t TLSF, void *Pointer, uintptr_t Size)
{
    if (Pointer == NULL)
        return TLSF_Alloc(TLSF, Size);
    if (Pointer != NULL && Size == 0) {
        TLSF_Free(TLSF, Pointer);
        return NULL;
    }
    
    TLSF_Control *Control = (TLSF_Control *)TLSF;
    
    TLSF_Block *Block = TLSF_BlockFromPointer(Pointer);
    TLSF_Block *Next  = TLSF_BlockGetNext(Block);
    
    uintptr_t CurrentSize  = TLSF_BlockGetSize(Block);
    uintptr_t CombinedSize = TLSF_BlockGetSize(Next) + TLSF_BlockOccupy + CurrentSize;
    uintptr_t RequestSize  = TLSF_AdjustRequestSize(Size, TLSF_AlignSize);
    
    TLSF_Check(!TLSF_BlockGetCurrFree(Block));
    
    if (RequestSize > CurrentSize && (!TLSF_BlockGetCurrFree(Next) || RequestSize > CombinedSize)) {
        void *PointerNew = TLSF_Alloc(TLSF, Size);
        for (uintptr_t Idx = 0; Idx < TLSF_MIN(CurrentSize, Size); Idx++)
           *(uint8_t *)PointerNew = *(uint8_t *)Pointer;
        TLSF_Free(TLSF, Pointer);
        return PointerNew;
    }
    
    if (RequestSize > CurrentSize) {
        TLSF_BlockMergeNext(Control, Block);
        TLSF_BlockMark(Block, false);
    }
    
    TLSF_BlockTrimUsed(Control, Block, RequestSize);
    
    return Pointer;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
