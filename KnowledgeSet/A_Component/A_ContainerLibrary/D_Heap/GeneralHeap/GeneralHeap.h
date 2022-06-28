#ifndef GENERAL_HEAP_H
#define GENERAL_HEAP_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//Root:   ((Index = 0))
//Left:   ((Index * 2 + 1))
//Right:  ((Index * 2 + 2))
//Parent: ((Index - 1) / 2)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct GeneralHeap {
    void   **Array;
    uint32_t Length;
    uint32_t Number;
    bool (*Compare)(void *Node1, void *Node2);
} GH_Heap;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 重置一个堆 */
static inline void GH_HeapReset(GH_Heap *Heap, void **Array, uint32_t Length,
                                bool (*Compare)(void *Node1, void *Node2))
{
    Heap->Array   = Array;
    Heap->Length  = Length;
    Heap->Number  = 0;
    Heap->Compare = Compare;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool GH_HeapPush(GH_Heap *Heap, void *Pointer)
{
    if (Heap->Number == Heap->Length)
        return false;
    /* 节点入堆 */
    Heap->Array[Heap->Number++] = Pointer;
    /*  */
    uint32_t Node = Heap->Number - 1;
    uint32_t Parent = 0;
    /* 逆向调整至根 */
    while (Node != 0) {
        /* 计算父节点 */
        Parent = (Node - 1) / 2;
        /* 比较子节点与父节点,无需交换退出 */
        if (Heap->Compare(Heap->Array[Parent], Heap->Array[Node]) == true)
            return true;
        /* 比较子节点与父节点,交换 */
        void *Temp1 = Heap->Array[Parent];
        void *Temp2 = Heap->Array[Node];
        Heap->Array[Parent] = Temp2;
        Heap->Array[Node]   = Temp1;
        /* 指向上一层 */
        Node = Parent;
    }
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool GH_HeapPop(GH_Heap *Heap, void **Pointer)
{
    if (Heap->Number == 0)
        return false;
    /* 节点出堆 */
    *Pointer = Heap->Array[0];
    /* 最后一个节点迁移到根 */
    Heap->Array[0] = Heap->Array[--Heap->Number];
    /*  */
    uint32_t Parent = 0;
    uint32_t Left   = 0;
    uint32_t Right  = 0;
    uint32_t Node   = 0;
    /* 顺向调整至叶子 */
    while (Parent < Heap->Number) {
        /* 计算左右子节点 */
        Left  = Parent * 2 + 1;
        Right = Parent * 2 + 2;
        /* 如果左孩子溢出,结束 */
        if (Left >= Heap->Number)
            return true;
        /* 如果右孩子溢出 */
        if (Right >= Heap->Number) {
            Node = Left;
            /* 比较子节点与父节点,交换 */
            if (Heap->Compare(Heap->Array[Parent], Heap->Array[Node]) == false) {
                void *Temp1 = Heap->Array[Parent];
                void *Temp2 = Heap->Array[Node];
                Heap->Array[Parent] = Temp2;
                Heap->Array[Node]   = Temp1;
            }
            return true;
        }
        /* 如果都没溢出 */
        Node = (Heap->Compare(Heap->Array[Left], Heap->Array[Right]) == true) ? Left : Right;
        /* 比较子节点与父节点,无需交换退出 */
        if (Heap->Compare(Heap->Array[Parent], Heap->Array[Node]) == true)
            return true;
        /* 比较子节点与父节点,交换 */
        void *Temp1 = Heap->Array[Parent];
        void *Temp2 = Heap->Array[Node];
        Heap->Array[Parent] = Temp2;
        Heap->Array[Node]   = Temp1;
        /* 指向下一层 */
        Parent = Node;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
