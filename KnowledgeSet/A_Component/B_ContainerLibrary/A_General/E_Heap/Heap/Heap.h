#ifndef GENERAL_HEAP_H
#define GENERAL_HEAP_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
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
typedef struct GenericsHeap {
    void   **Array;
    uint32_t Length;
    uint32_t Number;
    bool   (*Compare)(void *Node1, void *Node2);
} Heap;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 重置一个堆 */
static inline void Heap_Reset(Heap *Instance, void **Array, uint32_t Length,
                             bool (*Compare)(void *Node1, void *Node2))
{
    Instance->Array   = Array;
    Instance->Length  = Length;
    Instance->Number  = 0;
    Instance->Compare = Compare;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool Heap_Push(Heap *Instance, void *Pointer)
{
    if (Instance->Number == Instance->Length)
        return false;
    /* 节点入堆 */
    Instance->Array[Instance->Number++] = Pointer;
    /*  */
    uint32_t Node = Instance->Number - 1;
    uint32_t Parent = 0;
    /* 逆向调整至根 */
    while (Node != 0) {
        /* 计算父节点 */
        Parent = (Node - 1) / 2;
        /* 比较子节点与父节点,无需交换退出 */
        if (Instance->Compare(Instance->Array[Parent], Instance->Array[Node]) == true)
            return true;
        /* 比较子节点与父节点,交换 */
        void *Temp1 = Instance->Array[Parent];
        void *Temp2 = Instance->Array[Node];
        Instance->Array[Parent] = Temp2;
        Instance->Array[Node]   = Temp1;
        /* 指向上一层 */
        Node = Parent;
    }
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool Heap_Pop(Heap *Instance, void **Pointer)
{
    if (Instance->Number == 0)
        return false;
    /* 节点出堆 */
    *Pointer = Instance->Array[0];
    /* 最后一个节点迁移到根 */
    Instance->Array[0] = Instance->Array[--Instance->Number];
    /*  */
    uint32_t Parent = 0;
    uint32_t Left   = 0;
    uint32_t Right  = 0;
    uint32_t Node   = 0;
    /* 顺向调整至叶子 */
    while (Parent < Instance->Number) {
        /* 计算左右子节点 */
        Left  = Parent * 2 + 1;
        Right = Parent * 2 + 2;
        /* 如果左孩子溢出,结束 */
        if (Left >= Instance->Number)
            return true;
        /* 如果右孩子溢出 */
        if (Right >= Instance->Number) {
            Node = Left;
            /* 比较子节点与父节点,交换 */
            if (Instance->Compare(Instance->Array[Parent], Instance->Array[Node]) == false) {
                void *Temp1 = Instance->Array[Parent];
                void *Temp2 = Instance->Array[Node];
                Instance->Array[Parent] = Temp2;
                Instance->Array[Node]   = Temp1;
            }
            return true;
        }
        /* 如果都没溢出 */
        Node = (Instance->Compare(Instance->Array[Left], Instance->Array[Right]) == true) ? Left : Right;
        /* 比较子节点与父节点,无需交换退出 */
        if (Instance->Compare(Instance->Array[Parent], Instance->Array[Node]) == true)
            return true;
        /* 比较子节点与父节点,交换 */
        void *Temp1 = Instance->Array[Parent];
        void *Temp2 = Instance->Array[Node];
        Instance->Array[Parent] = Temp2;
        Instance->Array[Node]   = Temp1;
        /* 指向下一层 */
        Parent = Node;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
