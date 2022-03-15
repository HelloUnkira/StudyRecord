#include "MemoryManage.h"
#include "MemoryManageConfigure.h"

/* Slab分配器集实现 */
#ifdef MemoryManageStrategySlab

/* 重置Slab分配器 */
static void MemoryManageSlabReset(void)
{
    uint8_t *pointer = NULL;
	uint32_t index0 = 0;
    uint32_t index1 = 0;
	
	for (index0 = 0; index0 < SlabDistributorType; index0++) {
		/* 获得指定分配器 */
		SlabDistributor *Slab = &SlabSet[index0];
		/* 初始化分配器 */
        Slab->MemoryStart = Slab->BlockList;
        Slab->MemoryEnd   = Slab->BlockList;
        Slab->MemoryEnd  += Slab->BlockSize * Slab->BlockNumber;
        /* 索引回退 */
        Slab->BlockList -= Slab->BlockSize;
        /* 初始化块链表 */
        for (index1 = 0; index1 < Slab->BlockNumber; index1++) {
            /* 当前块移动到下一块 */
            Slab->BlockList += Slab->BlockSize;
            /* 当前块指向前一块索引 */
            *((uint8_t **)(Slab->BlockList)) = pointer;
            /* 前一块移动到当前块 */
            pointer = Slab->BlockList;
        }
        /* 重置索引 */
        pointer = NULL;
	}
}

/* 从Slab分配器获取一个块 */
bool MemoryManageSlabTake(uint32_t size, uint8_t **block)
{
	uint32_t index = 0;
	
	for (index = 0; index < SlabDistributorType; index++) {
		/* 获得指定分配器 */
		SlabDistributor *Slab = &SlabSet[index];
        /* 检查分配器 */
        if (Slab->BlockSize == size &&
            Slab->BlockList != NULL) {
            #if MemoryManageCheck == 1
            Slab->BlockUsed += Slab->BlockSize;
            #endif
            /* 获得首块 */
            *block = Slab->BlockList;
            /* 块索引移动到下一块 */
            Slab->BlockList = *((uint8_t **)(*block));
            /* 返回结果 */
            return true;
        }
    }
    return false;
}

/* 向Slab分配器归还一个块 */
bool MemoryManageSlabGive(uint32_t size, uint8_t *block)
{
	uint32_t index = 0;
	
	for (index = 0; index < SlabDistributorType; index++) {
		/* 获得指定分配器 */
		SlabDistributor *Slab = &SlabSet[index];
        /* 检查分配器 */
        if (Slab->MemoryStart <= block &&
            Slab->MemoryEnd   >  block) {
            #if MemoryManageCheck == 1
            Slab->BlockUsed -= Slab->BlockSize;
            #endif
            /* 释放首块 */
            *((uint8_t **)(block)) = Slab->BlockList;
            /* 块索引移动到下一块 */
            Slab->BlockList = block;
            /* 返回结果 */
            return true;
        }
    }
    return false;
}

#if MemoryManageCheck == 1
void MemoryManageSlabCheck(MemoryManageCheckPrint print)
{
	uint32_t index = 0;
	
	for (index = 0; index < SlabDistributorType; index++) {
		/* 获得指定分配器 */
		SlabDistributor *Slab = &SlabSet[index];
        /* 打印分配器信息 */
        print("\tSlab:  %u\r\n"    , index);
        print("\t\tsize:    %u\r\n", Slab->BlockSize);
        print("\t\tnumber:  %u\r\n", Slab->BlockNumber);
        print("\t\tstart:   %p\r\n", Slab->MemoryStart);
        print("\t\tend:     %p\r\n", Slab->MemoryEnd);
        print("\t\tused:    %u\r\n", Slab->BlockUsed);
        print("\t\tall:     %u\r\n", Slab->MemoryEnd - 
                                     Slab->MemoryStart);
    }
}

void MemoryManageSlabMem(uint32_t index, uint8_t **start, uint8_t **end)
{
    if (index < SlabDistributorType)
    {
        *start = SlabSet[index].MemoryStart;
        *end   = SlabSet[index].MemoryEnd;
    }
}
#endif

#endif

/* Buddy分配器集实现 */
#ifdef MemoryManageStrategyBuddy

/* 重置Buddy分配器 */
static void MemoryManageBuddyReset(void)
{
    uint8_t *pointer1 = 0;
    uint8_t *pointer2 = 0;
    uint8_t *pointer3 = 0;
    uint32_t size  = 1;
	uint32_t index = 0;
	
	for (index = 0; index < BuddyDistributorType; index++) {
		/* 获得指定分配器 */
		BuddyDistributor *Buddy = &BuddySet[index];
        /* 计算最大块尺寸 */
        size <<= Buddy->BlockSize;
        size <<= Buddy->BlockFactor;
		/* 初始化分配器 */
        Buddy->MemoryStart = Buddy->BlockOffset;
        Buddy->MemoryEnd   = Buddy->BlockOffset;
        Buddy->MemoryEnd  += Buddy->BlockNumber * size;
        Buddy->BlockList[Buddy->BlockFactor] = Buddy->BlockOffset;
        /* 初始化块链表 */
        pointer1 = Buddy->BlockOffset;
        pointer2 = pointer1 + size;
        pointer3 = pointer2 + size - 1;

        while (pointer2 != size) {
            //1.下一个块存在性检查
            pointer2 = (pointer3 >= Buddy->MemoryEnd) ? NULL : pointer2;
            //2.当前块指向下一个块
            *((uint8_t **)pointer1) = pointer2;
            //3.迭代到下一个块
            pointer1 += size;
            pointer2 += size;
            pointer3 += size;
        }
	}
}

//从分配器集合获取指定大小的块
bool MemoryManageBuddyTake(uint32_t size, uint8_t **block)
{
	uint32_t index0 = 0;
	uint32_t index1 = 0;
	uint32_t index2 = 0;
    uint32_t offset = 0;
    uint8_t *buddy  = NULL;
	
    /* 预处理,将size转化成offset */
    do {
        if ((1 << offset) == size)
            break;
        if ((1 << offset) > size)
            return false;
        if ((1 << offset) < size)
            offset++;
    } while (1);
    
	for (index0 = 0; index0 < BuddyDistributorType; index0++) {
		/* 获得指定分配器 */
		BuddyDistributor *Buddy = &BuddySet[index0];
        /* 可分配buddy分配器检查 */
        if (offset < (Buddy->BlockSize) ||
            offset > (Buddy->BlockSize + Buddy->BlockFactor))
            continue;
        /* 获取偏移量 */
        offset -= Buddy->BlockSize;
        /* 获得块单元阵列 */
        /* 首要检查,刚好存在可分配块,直接分配 */
        if (Buddy->BlockList[offset] != NULL) {
            *block = Buddy->BlockList[offset];
            Buddy->BlockList[offset] = *((uint8_t **)(*block));
            #if MemoryManageCheck == 1
            Buddy->BlockUsed += size;
            #endif
            return true;
        }
        /* 先向上寻找一个"最小可分配"块 */
        for (index1 = offset; index1 <= Buddy->BlockFactor; index1++)
            if (Buddy->BlockList[index1] != NULL)
                break;
        /* 本分配单元空间不足检查 */
        if (index1 <= Buddy->BlockFactor) {
            /* 取出"最小可分配"块 */
            *block = Buddy->BlockList[index1];
            Buddy->BlockList[index1] = *((uint8_t **)(*block));
            /* 反复掐掉右伙伴加入次级链表中,直到回退 */
            for (index2 = index1 - 1; offset <= index2 && index2 < index1; index2--) {
                /* 计算右兄弟位置 */
                buddy = *block + (1 << (Buddy->BlockSize + index2));
                //将后半部分加入到次级集合中,前半部分继续迭代分裂
                *((uint8_t **)(buddy)) = Buddy->BlockList[index2];
                Buddy->BlockList[index2] = buddy;
            }
            #if MemoryManageCheck == 1
            Buddy->BlockUsed += size;
            #endif
            return true;
        }
    }
    return false;
}

//给分配器集合归还指定大小的块
bool MemoryManageBuddyGive(uint32_t size, uint8_t *block)
{
	uint32_t index0 = 0;
	uint32_t index1 = 0;
    uint32_t offset = 0;
    uint8_t *iterator       = NULL;
    uint8_t *buddy_offset   = NULL;
    uint8_t *buddy          = NULL;
	
    /* 预处理,将size转化成offset */
    do {
        if ((1 << offset) == size)
            break;
        if ((1 << offset) > size)
            return false;
        if ((1 << offset) < size)
            offset++;
    } while (1);
    
	for (index0 = 0; index0 < BuddyDistributorType; index0++) {
		/* 获得指定分配器 */
		BuddyDistributor *Buddy = &BuddySet[index0];
        /* 可分配buddy分配器检查 */
        if (block < Buddy->MemoryStart && block >= Buddy->MemoryEnd)
            continue;
        /* 可分配buddy分配器检查 */
        if (offset < (Buddy->BlockSize) ||
            offset > (Buddy->BlockSize + Buddy->BlockFactor))
            continue;
        /* 获取偏移量 */
        offset -= Buddy->BlockSize;
        /* 寻找当前块的归属并找到"可能的"伙伴 */
        for (index1 = offset; index1 <= Buddy->BlockFactor; index1++) {
            //备注:p % 2**(k+1) == 0时,p 和 p + 2**k 互为伙伴
            buddy_offset = block - Buddy->BlockOffset;
            if ((uint32_t)buddy_offset % (1 << (Buddy->BlockSize + index1 + 1)) == 0)
                buddy = block + (1 << (Buddy->BlockSize + index1));
            if ((uint32_t)buddy_offset % (1 << (Buddy->BlockSize + index1 + 1)) != 0)
                buddy = block - (1 << (Buddy->BlockSize + index1));
            /* 检查伙伴块是否在集合中,如果在则将其取出集合 */
            if (Buddy->BlockList[index1] != NULL && Buddy->BlockFactor != index1) {
                /* 首项是伙伴时 */
                if (Buddy->BlockList[index1] == buddy) {
                    Buddy->BlockList[index1] = *((uint8_t **)(buddy));
                    //伙伴在链表集中,与伙伴合并,进入下一阶段
                    block = buddy < block ? buddy : block;
                    continue;
                }
                /* 向后迭代 */
                for (iterator = Buddy->BlockList[index1];
                     *((uint8_t **)(iterator)) != NULL &&
                     *((uint8_t **)(iterator)) < buddy;
                     iterator = *((unsigned char **)(iterator)));
                /* 检查"可能的"兄弟 */
                if (*((uint8_t **)(iterator)) == buddy) {
                    *((uint8_t **)(iterator)) = *((uint8_t **)(buddy));
                    *((uint8_t **)(buddy)) = NULL;
                    //伙伴在链表集中,与伙伴合并,进入下一阶段
                    block = buddy < block ? buddy : block;
                    continue;
                }
            }
            /* 不可继续迭代合并,此处退出,假伙伴 */
            /* 伙伴不在链表集中,自己加入到集合中 */
            
            /* 首项是高地址块时 */
            if (Buddy->BlockList[index1] == NULL ||
                Buddy->BlockList[index1] >= block) {
                *((uint8_t **)(block)) = Buddy->BlockList[index1];
                Buddy->BlockList[index1] = block;
                #if MemoryManageCheck == 1
                Buddy->BlockUsed -= size;
                #endif
                return true;
            }
            /* 向后迭代 */
            for (iterator = Buddy->BlockList[index1];
                 *((uint8_t **)(iterator)) != NULL &&
                 *((uint8_t **)(iterator)) < block;
                 iterator = *((uint8_t **)(iterator)));
            /* 到达插入位置 */
            *((uint8_t **)(block)) = *((uint8_t **)(iterator));
            *((uint8_t **)(iterator)) = block;
            #if MemoryManageCheck == 1
            Buddy->BlockUsed -= size;
            #endif
            return true;
        }
    }
    return false;
}

#if MemoryManageCheck == 1
void MemoryManageBuddyCheck(MemoryManageCheckPrint print)
{
	uint32_t index = 0;
	
	for (index = 0; index < BuddyDistributorType; index++) {
		/* 获得指定分配器 */
		BuddyDistributor *Buddy = &BuddySet[index];
        /* 打印分配器信息 */
        print("\tBuddy:  %u\r\n"   , index);
        print("\t\tsize:    %u\r\n", Buddy->BlockSize);
        print("\t\tfactor:  %u\r\n", Buddy->BlockFactor);
        print("\t\tnumber:  %u\r\n", Buddy->BlockNumber);
        print("\t\tstart:   %p\r\n", Buddy->MemoryStart);
        print("\t\tend:     %p\r\n", Buddy->MemoryEnd);
        print("\t\tused:    %u\r\n", Buddy->BlockUsed);
        print("\t\tall:     %u\r\n", Buddy->MemoryEnd - 
                                     Buddy->MemoryStart);
    }
}

void MemoryManageBuddyMem(uint32_t index, uint8_t **start, uint8_t **end)
{
    if (index < BuddyDistributorType)
    {
        *start = BuddySet[index].MemoryStart;
        *end   = BuddySet[index].MemoryEnd;
    }
}
#endif

#endif

/* Heap分配器集实现 */
#ifdef MemoryManageStrategyHeap

/* Heap分配器内部抽象,描述内存块虚拟对象 */
typedef struct MemoryManageHeap_Block {
    uint64_t Size;
    /* 用单链表也可以,不过,既然是堆分配,就没必要这么麻烦 */
    struct MemoryManageHeap_Block *Prev;
    struct MemoryManageHeap_Block *Next;
} HeapDistributor_Block;

/* 重置Buddy分配器 */
static void MemoryManageHeapReset(void)
{
	uint32_t index = 0;
	
	for (index = 0; index < HeapDistributorType; index++) {
		/* 获得指定分配器 */
		HeapDistributor *Heap = &HeapSet[index];
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

//从分配器集合获取指定大小的块
bool MemoryManageHeapTake(uint32_t size, uint8_t **block)
{
	uint32_t index = 0;
    uint64_t BlockSize = 0;
    HeapDistributor_Block *BlockOld = NULL;
	
	for (index = 0; index < HeapDistributorType; index++) {
		/* 获得指定分配器 */
		HeapDistributor *Heap = &HeapSet[index];
        /* 获取块内部信息 */
        HeapDistributor_Block *Block = Heap->BlockList;
        /* 尝试找到一个大块(Size将会在回收重构时使用到) */
        for (Block = Heap->BlockList; Block != NULL; Block = Block->Next)
            if (Block->Size >= size + sizeof(uint64_t))
                break;
        /* 如果成功找到一个大块 */
        if (Block != NULL) {
            /* 获得块地址 */
            *block = ((uint8_t *)Block + sizeof(uint64_t));
            /* 计算分裂后的块大小 */
            BlockSize = Block->Size - size - sizeof(uint64_t);
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
                BlockOld = Block;
                /* 初始新的Block */
                Block = ((uint8_t *)BlockOld + size + sizeof(uint64_t));
                Block->Size = BlockOld->Size - size - sizeof(uint64_t);
                Block->Prev = BlockOld->Prev;
                Block->Next = BlockOld->Next;
                BlockOld->Size = size + sizeof(uint64_t);
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

//给分配器集合归还指定大小的块
bool MemoryManageHeapGive(uint32_t size, uint8_t *block)
{
	uint32_t index  = 0;
    uint8_t *offset = 0;
    HeapDistributor_Block *BlockNew = (block - sizeof(uint64_t));
	
	for (index = 0; index < HeapDistributorType; index++) {
		/* 获得指定分配器 */
		HeapDistributor *Heap = &HeapSet[index];
        /* 检查分配器 */
        if (Heap->MemoryStart >  BlockNew &&
            Heap->MemoryEnd   <= BlockNew)
            continue;
        #ifdef MemoryManageCheck
        Heap->BlockUsed -= BlockNew->Size;
        #endif
        /* 预处理检查 */
        if (Heap->BlockList == NULL) {
            Heap->BlockList  = BlockNew;
            BlockNew->Prev = NULL;
            BlockNew->Next = NULL;
            return true;
        }
        /* 获取块内部信息 */
        HeapDistributor_Block *Block = Heap->BlockList;
        /* 找到合适的插入位点 */
        while (Block < BlockNew && Block->Next != NULL && Block->Next < BlockNew)
            Block = Block->Next;
        /* 插入位点检查(不存在相等的情况) */
        if (Block > BlockNew) {
            BlockNew->Next = Block;
            BlockNew->Prev = Block->Prev;
            if (BlockNew->Prev != NULL)
                BlockNew->Prev->Next = BlockNew;
            if (BlockNew->Prev == NULL)
                Heap->BlockList = BlockNew;
            Block->Prev = BlockNew;
        } else {
            BlockNew->Prev = Block;
            BlockNew->Next = Block->Next;
            if (BlockNew->Next != NULL)
                BlockNew->Next->Prev = BlockNew;
            Block->Next = BlockNew;
        }
        /* 对BlockNew进行前项合并检查,无需递归合并 */
        Block = BlockNew->Prev;
        if (Block != NULL) {
            /* 计算地址是否接合 */
            offset  = Block;
            offset += Block->Size;
            /* 如果地址接合,可以合并 */
            if (offset == BlockNew) {
                Block->Size += BlockNew->Size;
                Block->Next  = BlockNew->Next;
                if (Block->Next != NULL)
                    Block->Next->Prev = Block;
                /* 操作转移 */
                BlockNew = Block;
            }
        }
        /* 对BlockNew进行后项合并检查,无需递归合并 */
        Block = BlockNew->Next;
        if (Block != NULL) {
            /* 计算地址是否接合 */
            offset  = BlockNew;
            offset += BlockNew->Size;
            /* 如果地址接合,可以合并 */
            if (offset == Block) {
                BlockNew->Size += Block->Size;
                BlockNew->Next  = Block->Next;
                if (BlockNew->Next != NULL)
                    BlockNew->Next->Prev = BlockNew;
            }
        }
        return true;
    }
    return false;
}

#if MemoryManageCheck == 1
void MemoryManageHeapCheck(MemoryManageCheckPrint print)
{
	uint32_t index0 = 0;
	uint32_t index1 = 0;
	
	for (index0 = 0; index0 < HeapDistributorType; index0++) {
		/* 获得指定分配器 */
		HeapDistributor *Heap = &HeapSet[index0];
        /* 打印分配器信息 */
        print("\tBuddy:  %u\r\n"   , index0);
        print("\t\tsize:    %u\r\n", Heap->BlockSize);
        print("\t\tstart:   %p\r\n", Heap->MemoryStart);
        print("\t\tend:     %p\r\n", Heap->MemoryEnd);
        print("\t\tused:    %u\r\n", Heap->BlockUsed);
        print("\t\tall:     %u\r\n", Heap->MemoryEnd - 
                                     Heap->MemoryStart);
        print(" \r\n", 0);
        /* 遍历链表 */
        HeapDistributor_Block *Block = Heap->BlockList;
        for (Block = Heap->BlockList; Block != NULL; Block = Block->Next) {
            print("\t\tBlock:%u", ++index1);
            print("\tSize:%u",  Block->Size);
            print("\tPrev:%8p", Block->Prev);
            print("\tNext:%8p", Block->Next);
            print(" \r\n", 0);
        }
    }
}

void MemoryManageHeapMem(uint32_t index, uint8_t **start, uint8_t **end)
{
    if (index < HeapDistributorType)
    {
        *start = HeapSet[index].MemoryStart;
        *end   = HeapSet[index].MemoryEnd;
    }
}
#endif

#endif

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
