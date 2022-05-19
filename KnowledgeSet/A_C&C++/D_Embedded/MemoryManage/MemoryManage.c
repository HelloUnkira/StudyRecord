#include "MemoryManage.h"
#include "MemoryManageConfigure.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Slab分配器集实现 */
#ifdef MemoryManageStrategySlab
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 重置Slab分配器 */
static void MemoryManageSlabReset(void)
{
    uint8_t *Pointer = NULL;

	for (uint32_t Index0 = 0; Index0 < SlabDistributorType; Index0++) {
		/* 获得指定分配器 */
		SlabDistributor *Slab = &SlabSet[Index0];
		/* 初始化分配器 */
        Slab->MemoryStart = Slab->BlockList;
        Slab->MemoryEnd   = Slab->BlockList;
        Slab->MemoryEnd  += Slab->BlockSize * Slab->BlockNumber;
        /* 索引回退 */
        Slab->BlockList -= Slab->BlockSize;
        /* 初始化块链表 */
        for (uint32_t Index1 = 0; Index1 < Slab->BlockNumber; Index1++) {
            /* 当前块移动到下一块 */
            Slab->BlockList += Slab->BlockSize;
            /* 当前块指向前一块索引 */
            *((uint8_t **)(Slab->BlockList)) = Pointer;
            /* 前一块移动到当前块 */
            Pointer = Slab->BlockList;
        }
        /* 重置索引 */
        Pointer = NULL;
	}
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 从Slab分配器获取一个块 */
bool MemoryManageSlabTake(uint32_t Size, uint8_t **Block)
{
	for (uint32_t Index = 0; Index < SlabDistributorType; Index++) {
		/* 获得指定分配器 */
		SlabDistributor *Slab = &SlabSet[Index];
        /* 检查分配器 */
        if (Slab->BlockSize == Size && Slab->BlockList != NULL) {
            #if MemoryManageCheck == 1
            Slab->BlockUsed += Slab->BlockSize;
            #endif
            /* 获得首块 */
            *Block = Slab->BlockList;
            /* 块索引移动到下一块 */
            Slab->BlockList = *((uint8_t **)(*Block));
            /* 返回结果 */
            return true;
        }
    }
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 向Slab分配器归还一个块 */
bool MemoryManageSlabGive(uint32_t Size, uint8_t *Block)
{
	for (uint32_t Index = 0; Index < SlabDistributorType; Index++) {
		/* 获得指定分配器 */
		SlabDistributor *Slab = &SlabSet[Index];
        /* 检查分配器 */
        if (Slab->MemoryStart <= Block && Slab->MemoryEnd > Block) {
            #if MemoryManageCheck == 1
            Slab->BlockUsed -= Slab->BlockSize;
            #endif
            /* 释放首块 */
            *((uint8_t **)(Block)) = Slab->BlockList;
            /* 块索引移动到下一块 */
            Slab->BlockList = Block;
            /* 返回结果 */
            return true;
        }
    }
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#if MemoryManageCheck == 1
void MemoryManageSlabCheck(MemoryManageCheckPrint Print)
{
	for (uint32_t Index = 0; Index < SlabDistributorType; Index++) {
		/* 获得指定分配器 */
		SlabDistributor *Slab = &SlabSet[Index];
        /* 打印分配器信息 */
        Print("\tSlab:  %u\r\n"    , Index);
        Print("\t\tSize:    %u\r\n", Slab->BlockSize);
        Print("\t\tnumber:  %u\r\n", Slab->BlockNumber);
        Print("\t\tstart:   %p\r\n", Slab->MemoryStart);
        Print("\t\tend:     %p\r\n", Slab->MemoryEnd);
        Print("\t\tused:    %u\r\n", Slab->BlockUsed);
        Print("\t\tall:     %u\r\n", Slab->MemoryEnd - 
                                     Slab->MemoryStart);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void MemoryManageSlabMem(uint32_t Index, uint8_t **Start, uint8_t **End)
{
    if (Index < SlabDistributorType)
    {
        *Start = SlabSet[Index].MemoryStart;
        *End   = SlabSet[Index].MemoryEnd;
    }
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Buddy分配器集实现 */
#ifdef MemoryManageStrategyBuddy
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 重置Buddy分配器 */
static void MemoryManageBuddyReset(void)
{
	for (uint32_t Index = 0; Index < BuddyDistributorType; Index++) {
		/* 获得指定分配器 */
		BuddyDistributor *Buddy = &BuddySet[Index];
        /* 计算最大块尺寸 */
        uint32_t Size = 1;
        Size <<= Buddy->BlockSize;
        Size <<= Buddy->BlockFactor;
		/* 初始化分配器 */
        Buddy->MemoryStart = Buddy->BlockOffset;
        Buddy->MemoryEnd   = Buddy->BlockOffset;
        Buddy->MemoryEnd  += Buddy->BlockNumber * Size;
        Buddy->BlockList[Buddy->BlockFactor] = Buddy->BlockOffset;
        /* 初始化块链表 */
        uint8_t *Pointer1 = Buddy->BlockOffset;
        uint8_t *Pointer2 = Pointer1 + Size;
        uint8_t *Pointer3 = Pointer2 + Size - 1;

        while (Pointer2 != Size) {
            //1.下一个块存在性检查
            Pointer2 = (Pointer3 >= Buddy->MemoryEnd) ? NULL : Pointer2;
            //2.当前块指向下一个块
            *((uint8_t **)Pointer1) = Pointer2;
            //3.迭代到下一个块
            Pointer1 += Size;
            Pointer2 += Size;
            Pointer3 += Size;
        }
	}
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//从分配器集合获取指定大小的块
bool MemoryManageBuddyTake(uint32_t Size, uint8_t **Block)
{
    uint32_t Offset = 0;
	
    /* 预处理,将Size转化成offset */
    do {
        if ((1 << Offset) == Size)
            break;
        if ((1 << Offset) > Size)
            return false;
        if ((1 << Offset) < Size)
            Offset++;
    } while (1);
    
	for (uint32_t Index0 = 0; Index0 < BuddyDistributorType; Index0++) {
		/* 获得指定分配器 */
		BuddyDistributor *Buddy = &BuddySet[Index0];
        /* 可分配buddy分配器检查 */
        if (Offset < (Buddy->BlockSize) ||
            Offset > (Buddy->BlockSize + Buddy->BlockFactor))
            continue;
        /* 获取偏移量 */
        Offset -= Buddy->BlockSize;
        /* 获得块单元阵列 */
        /* 首要检查,刚好存在可分配块,直接分配 */
        if (Buddy->BlockList[Offset] != NULL) {
            *Block = Buddy->BlockList[Offset];
            Buddy->BlockList[Offset] = *((uint8_t **)(*Block));
            #if MemoryManageCheck == 1
            Buddy->BlockUsed += Size;
            #endif
            return true;
        }
        uint32_t Index1 = 0;
        uint32_t Index2 = 0;
        /* 先向上寻找一个"最小可分配"块 */
        for (Index1 = Offset; Index1 <= Buddy->BlockFactor; Index1++)
            if (Buddy->BlockList[Index1] != NULL)
                break;
        /* 本分配单元空间不足检查 */
        if (Index1 <= Buddy->BlockFactor) {
            /* 取出"最小可分配"块 */
            *Block = Buddy->BlockList[Index1];
            Buddy->BlockList[Index1] = *((uint8_t **)(*Block));
            /* 反复掐掉右伙伴加入次级链表中,直到回退 */
            for (Index2 = Index1 - 1; Offset <= Index2 && Index2 < Index1; Index2--) {
                /* 计算右兄弟位置 */
                uint8_t *buddy = *Block + (1 << (Buddy->BlockSize + Index2));
                //将后半部分加入到次级集合中,前半部分继续迭代分裂
                *((uint8_t **)(buddy)) = Buddy->BlockList[Index2];
                Buddy->BlockList[Index2] = buddy;
            }
            #if MemoryManageCheck == 1
            Buddy->BlockUsed += Size;
            #endif
            return true;
        }
    }
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//给分配器集合归还指定大小的块
bool MemoryManageBuddyGive(uint32_t Size, uint8_t *Block)
{
    uint32_t Offset = 0;
    
    /* 预处理,将Size转化成offset */
    do {
        if ((1 << Offset) == Size)
            break;
        if ((1 << Offset) > Size)
            return false;
        if ((1 << Offset) < Size)
            Offset++;
    } while (1);
    
	for (uint32_t Index0 = 0; Index0 < BuddyDistributorType; Index0++) {
		/* 获得指定分配器 */
		BuddyDistributor *Buddy = &BuddySet[Index0];
        /* 可分配buddy分配器检查 */
        if (Block < Buddy->MemoryStart && Block >= Buddy->MemoryEnd)
            continue;
        /* 可分配buddy分配器检查 */
        if (Offset < (Buddy->BlockSize) ||
            Offset > (Buddy->BlockSize + Buddy->BlockFactor))
            continue;
        /* 获取偏移量 */
        Offset -= Buddy->BlockSize;
        /* 寻找当前块的归属并找到"可能的"伙伴 */
        for (uint32_t Index1 = Offset; Index1 <= Buddy->BlockFactor; Index1++) {
            //备注:p % 2**(k+1) == 0时,p 和 p + 2**k 互为伙伴
            uint8_t *buddy = NULL;
            uint8_t *buddyOffset = Block - Buddy->BlockOffset;
            if ((uint32_t)buddyOffset % (1 << (Buddy->BlockSize + Index1 + 1)) == 0)
                buddy = Block + (1 << (Buddy->BlockSize + Index1));
            if ((uint32_t)buddyOffset % (1 << (Buddy->BlockSize + Index1 + 1)) != 0)
                buddy = Block - (1 << (Buddy->BlockSize + Index1));
            /* 检查伙伴块是否在集合中,如果在则将其取出集合 */
            if (Buddy->BlockList[Index1] != NULL && Buddy->BlockFactor != Index1) {
                /* 首项是伙伴时 */
                if (Buddy->BlockList[Index1] == buddy) {
                    Buddy->BlockList[Index1] = *((uint8_t **)(buddy));
                    //伙伴在链表集中,与伙伴合并,进入下一阶段
                    Block = buddy < Block ? buddy : Block;
                    continue;
                }
                uint8_t *Iterator = NULL;
                /* 向后迭代 */
                for (Iterator = Buddy->BlockList[Index1];
                     *((uint8_t **)(Iterator)) != NULL &&
                     *((uint8_t **)(Iterator)) < buddy;
                     Iterator = *((unsigned char **)(Iterator)));
                /* 检查"可能的"兄弟 */
                if (*((uint8_t **)(Iterator)) == buddy) {
                    *((uint8_t **)(Iterator)) = *((uint8_t **)(buddy));
                    *((uint8_t **)(buddy)) = NULL;
                    //伙伴在链表集中,与伙伴合并,进入下一阶段
                    Block = buddy < Block ? buddy : Block;
                    continue;
                }
            }
            /* 不可继续迭代合并,此处退出,假伙伴 */
            /* 伙伴不在链表集中,自己加入到集合中 */
            
            /* 首项是高地址块时 */
            if (Buddy->BlockList[Index1] == NULL ||
                Buddy->BlockList[Index1] >= Block) {
                *((uint8_t **)(Block)) = Buddy->BlockList[Index1];
                Buddy->BlockList[Index1] = Block;
                #if MemoryManageCheck == 1
                Buddy->BlockUsed -= Size;
                #endif
                return true;
            }
            uint8_t *Iterator = NULL;
            /* 向后迭代 */
            for (Iterator = Buddy->BlockList[Index1];
                 *((uint8_t **)(Iterator)) != NULL &&
                 *((uint8_t **)(Iterator)) < Block;
                 Iterator = *((uint8_t **)(Iterator)));
            /* 到达插入位置 */
            *((uint8_t **)(Block)) = *((uint8_t **)(Iterator));
            *((uint8_t **)(Iterator)) = Block;
            #if MemoryManageCheck == 1
            Buddy->BlockUsed -= Size;
            #endif
            return true;
        }
    }
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#if MemoryManageCheck == 1
void MemoryManageBuddyCheck(MemoryManageCheckPrint Print)
{
	for (uint32_t Index = 0; Index < BuddyDistributorType; Index++) {
		/* 获得指定分配器 */
		BuddyDistributor *Buddy = &BuddySet[Index];
        /* 打印分配器信息 */
        Print("\tBuddy:  %u\r\n"   , Index);
        Print("\t\tSize:    %u\r\n", Buddy->BlockSize);
        Print("\t\tfactor:  %u\r\n", Buddy->BlockFactor);
        Print("\t\tnumber:  %u\r\n", Buddy->BlockNumber);
        Print("\t\tstart:   %p\r\n", Buddy->MemoryStart);
        Print("\t\tend:     %p\r\n", Buddy->MemoryEnd);
        Print("\t\tused:    %u\r\n", Buddy->BlockUsed);
        Print("\t\tall:     %u\r\n", Buddy->MemoryEnd - 
                                     Buddy->MemoryStart);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void MemoryManageBuddyMem(uint32_t Index, uint8_t **Start, uint8_t **End)
{
    if (Index < BuddyDistributorType)
    {
        *Start = BuddySet[Index].MemoryStart;
        *End   = BuddySet[Index].MemoryEnd;
    }
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Heap分配器集实现 */
#ifdef MemoryManageStrategyHeap
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Heap分配器内部抽象,描述内存块虚拟对象 */
typedef struct MemoryManageHeap_Block {
    uint64_t Size;
    /* 用单链表也可以,不过,既然是堆分配,就没必要这么麻烦 */
    struct MemoryManageHeap_Block *Prev;
    struct MemoryManageHeap_Block *Next;
} HeapDistributor_Block;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 重置Buddy分配器 */
static void MemoryManageHeapReset(void)
{
	for (uint32_t Index = 0; Index < HeapDistributorType; Index++) {
		/* 获得指定分配器 */
		HeapDistributor *Heap = &HeapSet[Index];
        /* 初始化分配器 */
        Heap->MemoryStart = Heap->BlockList;
        Heap->MemoryEnd   = Heap->BlockList;
        Heap->MemoryEnd  += Heap->BlockSize;
        /* 添加块内部信息 */
        HeapDistributor_Block *Block = Heap->BlockList;
        Block->Size = Heap->BlockSize;
        Block->Prev = NULL;
        Block->Next = NULL;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//从分配器集合获取指定大小的块
bool MemoryManageHeapTake(uint32_t Size, uint8_t **BlockNew)
{
	for (uint32_t Index = 0; Index < HeapDistributorType; Index++) {
		/* 获得指定分配器 */
		HeapDistributor *Heap = &HeapSet[Index];
        /* 获取块内部信息 */
        HeapDistributor_Block *Block = Heap->BlockList;
        /* 尝试找到一个大块(Size将会在回收重构时使用到) */
        for (Block = Heap->BlockList; Block != NULL; Block = Block->Next)
            if (Block->Size >= Size + sizeof(uint64_t))
                break;
        /* 如果成功找到一个大块 */
        if (Block != NULL) {
            /* 获得块地址 */
            *BlockNew = ((uint8_t *)Block + sizeof(uint64_t));
            /* 计算分裂后的块大小 */
            uint64_t BlockSize = Block->Size - Size - sizeof(uint64_t);
            /* 如果块已经不支持进一步分裂时 */
            if (BlockSize < sizeof(HeapDistributor_Block)) {
                #if MemoryManageCheck == 1
                Heap->BlockUsed += Block->Size;
                #endif
                if (Block->Next != NULL)
                    Block->Next->Prev = Block->Prev;
                if (Block->Prev != NULL)
                    Block->Prev->Next = Block->Next;
                if (Block->Prev == NULL)
                    Heap->BlockList   = Block->Next;
            }
            /* 如果块支持进一步分裂时 */
            if (BlockSize >= sizeof(HeapDistributor_Block)) {
                /* 记录旧的Block */
                HeapDistributor_Block *BlockOld = Block;
                /* 初始新的Block */
                Block = ((uint8_t *)BlockOld + Size + sizeof(uint64_t));
                Block->Size = BlockOld->Size - Size - sizeof(uint64_t);
                Block->Prev = BlockOld->Prev;
                Block->Next = BlockOld->Next;
                BlockOld->Size = Size + sizeof(uint64_t);
                #if MemoryManageCheck == 1
                Heap->BlockUsed += BlockOld->Size;
                #endif
                if (Block->Next != NULL)
                    Block->Next->Prev = Block;
                if (Block->Prev != NULL)
                    Block->Prev->Next = Block;
                if (Block->Prev == NULL)
                    Heap->BlockList   = Block;
            }
            return true;
        }
    }
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//给分配器集合归还指定大小的块
bool MemoryManageHeapGive(uint32_t Size, uint8_t *BlockOld)
{
    HeapDistributor_Block *Block = (BlockOld - sizeof(uint64_t));
	
	for (uint32_t Index = 0; Index < HeapDistributorType; Index++) {
		/* 获得指定分配器 */
		HeapDistributor *Heap = &HeapSet[Index];
        /* 检查分配器 */
        if (Heap->MemoryStart > Block && Heap->MemoryEnd <= Block)
            continue;
        #ifdef MemoryManageCheck
        Heap->BlockUsed -= Block->Size;
        #endif
        /* 预处理检查 */
        if (Heap->BlockList == NULL) {
            Heap->BlockList  = Block;
            Block->Prev = NULL;
            Block->Next = NULL;
            return true;
        }
        /* 获取块内部信息 */
        HeapDistributor_Block *BlockNew = Heap->BlockList;
        /* 找到合适的插入位点 */
        while (BlockNew < Block && BlockNew->Next != NULL && BlockNew->Next < Block)
            BlockNew = BlockNew->Next;
        /* 插入位点检查(不存在相等的情况) */
        if (BlockNew > Block) {
            Block->Next = BlockNew;
            Block->Prev = BlockNew->Prev;
            if (Block->Prev != NULL)
                Block->Prev->Next = Block;
            if (Block->Prev == NULL)
                Heap->BlockList = Block;
            BlockNew->Prev = Block;
        } else {
            Block->Prev = BlockNew;
            Block->Next = BlockNew->Next;
            if (Block->Next != NULL)
                Block->Next->Prev = Block;
            BlockNew->Next = Block;
        }
        /* 对BlockNew进行前项合并检查,无需递归合并 */
        uint8_t *Offset = 0;
        BlockNew = Block->Prev;
        if (BlockNew != NULL) {
            /* 计算地址是否接合 */
            Offset  = BlockNew;
            Offset += BlockNew->Size;
            /* 如果地址接合,可以合并 */
            if (Offset == Block) {
                BlockNew->Size += Block->Size;
                BlockNew->Next  = Block->Next;
                if (BlockNew->Next != NULL)
                    BlockNew->Next->Prev = BlockNew;
                /* 操作转移 */
                Block = BlockNew;
            }
        }
        /* 对BlockNew进行后项合并检查,无需递归合并 */
        BlockNew = Block->Next;
        if (BlockNew != NULL) {
            /* 计算地址是否接合 */
            Offset  = Block;
            Offset += Block->Size;
            /* 如果地址接合,可以合并 */
            if (Offset == BlockNew) {
                Block->Size += BlockNew->Size;
                Block->Next  = BlockNew->Next;
                if (Block->Next != NULL)
                    Block->Next->Prev = Block;
            }
        }
        return true;
    }
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#if MemoryManageCheck == 1
void MemoryManageHeapCheck(MemoryManageCheckPrint Print)
{
	uint32_t Record = 0;
	
	for (uint32_t Index = 0; Index < HeapDistributorType; Index++) {
		/* 获得指定分配器 */
		HeapDistributor *Heap = &HeapSet[Index];
        /* 打印分配器信息 */
        Print("\tBuddy:  %u\r\n"   , Index);
        Print("\t\tSize:    %u\r\n", Heap->BlockSize);
        Print("\t\tstart:   %p\r\n", Heap->MemoryStart);
        Print("\t\tend:     %p\r\n", Heap->MemoryEnd);
        Print("\t\tused:    %u\r\n", Heap->BlockUsed);
        Print("\t\tall:     %u\r\n", Heap->MemoryEnd - 
                                     Heap->MemoryStart);
        Print(" \r\n", 0);
        /* 遍历链表 */
        HeapDistributor_Block *Block = Heap->BlockList;
        for (Block = Heap->BlockList; Block != NULL; Block = Block->Next) {
            Print("\t\tBlock:%u", ++Record);
            Print("\tSize:%u",  Block->Size);
            Print("\tPrev:%8p", Block->Prev);
            Print("\tNext:%8p", Block->Next);
            Print(" \r\n", 0);
        }
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void MemoryManageHeapMem(uint32_t Index, uint8_t **Start, uint8_t **End)
{
    if (Index < HeapDistributorType)
    {
        *Start = HeapSet[Index].MemoryStart;
        *End   = HeapSet[Index].MemoryEnd;
    }
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void MemoryManageConfigure(void)
{
#ifdef  MemoryManageStrategySlab
    MemoryManageSlabConfigure();
    MemoryManageSlabReset();
#endif

#ifdef  MemoryManageStrategyBuddy
    MemoryManageBuddyConfigure();
    MemoryManageBuddyReset();
#endif

#ifdef MemoryManageStrategyHeap
    MemoryManageHeapConfigure();
    MemoryManageHeapReset();
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
