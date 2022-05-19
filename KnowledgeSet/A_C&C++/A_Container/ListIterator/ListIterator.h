#ifndef LISTITERATOR_H
#define LISTITERATOR_H
//C std lib
#include <stdint.h>
#include <stdbool.h>

/* 实现目标:
 * 链表迭代器
 * 注意:此处使用的是普通版本的迭代器
 *      稳定在几个单位级别的额外字节开销
 *      内嵌版本的宏迭代器高效实现一般需要系统宏接口
 */

#define LISTITERATOR_DOUBLELIST
#ifdef  LISTITERATOR_DOUBLELIST
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 双向链表迭代器 */
struct ListIterator_DoubleListNode {
    struct ListIterator_DoubleListNode *Prev;//双向链表上一节点
    struct ListIterator_DoubleListNode *Next;//双向链表下一节点
    void  *Member;
};

struct ListIterator_DoubleListBody {
    struct ListIterator_DoubleListNode *Head;//双向链表头
    struct ListIterator_DoubleListNode *Tail;//双向链表尾
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 核心转义 */
typedef struct ListIterator_DoubleListBody DListBody;
typedef struct ListIterator_DoubleListNode DListNode;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 双向链表体抽象 */
static inline void DList_Reset(DListBody *List)
{
    List->Head = NULL;
    List->Tail = NULL;
}

static inline void DList_SetHead(DListBody *List, DListNode *Node)
{
    List->Head = Node;
}

static inline void DList_SetTail(DListBody *List, DListNode *Node)
{
    List->Tail = Node;
}

static inline DListNode * DList_GetHead(DListBody *List)
{
    return List->Head;
}

static inline DListNode * DList_GetTail(DListBody *List)
{
    return List->Tail;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 双向链表点抽象 */

static inline void DNode_Reset(DListNode *Node)
{
    Node->Prev = NULL;
    Node->Next = NULL;
    Node->Member = NULL;
}

static inline void DNode_SetPrev(DListNode *Node, DListNode *Prev)
{
    Node->Prev = Prev;
}

static inline void DNode_SetNext(DListNode *Node, DListNode *Next)
{
    Node->Next = Next;
}

static inline void DNode_SetMember(DListNode *Node, void *Member)
{
    Node->Member = Member;
}

static inline DListNode * DNode_GetPrev(DListNode *Node)
{
    return Node->Prev;
}

static inline DListNode * DNode_GetNext(DListNode *Node)
{
    return Node->Next;
}

static inline void * DNode_GetMember(DListNode *Node)
{
    return Node->Member;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 节点前插入(节点Target为NULL时为头部插入) */
static inline void DList_InsertPrepend(DListBody *List, DListNode *Target, DListNode *Node)
{
    if (Target == NULL) {
        DListNode *Head = DList_GetHead(List);
        /* 更新插入节点 */
        DNode_SetPrev(Node, NULL);
        DNode_SetNext(Node, Head);
        if (Head != NULL)
            DNode_SetPrev(Head, Node);
        if (Head == NULL)
            DList_SetTail(List, Node);
        DList_SetHead(List, Node);
    }
    if (Target != NULL) {
        DListNode *Prev = DNode_GetPrev(Target);
        /* 更新插入节点 */ 
        DNode_SetPrev(Node, Prev);
        DNode_SetNext(Node, Target);
        /* 更新后项节点 */
        DNode_SetPrev(Target, Node);
        /* 前项节点存在时 */
        if (Prev != NULL)
            DNode_SetNext(Prev, Node);
        if (Prev == NULL)
            DList_SetHead(List, Node);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 节点后插入(节点Target为NULL时为尾部插入) */
static inline void DList_InsertAppend(DListBody *List, DListNode *Target, DListNode *Node)
{
    if (Target == NULL) {
        DListNode *Tail = DList_GetTail(List);
        /* 更新插入节点 */
        DNode_SetPrev(Node, Tail);
        DNode_SetNext(Node, NULL);
        if (Tail != NULL)
            DNode_SetNext(Tail, Node);
        if (Tail == NULL)
            DList_SetHead(List, Node);
        DList_SetTail(List, Node);
    }
    if (Target != NULL) {
        DListNode *Next = DNode_GetNext(Target);
        /* 更新插入节点 */
        DNode_SetPrev(Node, Target);
        DNode_SetNext(Node, Next);
        /* 更新前项节点 */
        DNode_SetNext(Target, Node);
        /* 后项节点存在时 */
        if (Next != NULL)
            DNode_SetPrev(Next, Node);
        if (Next == NULL)
            DList_SetTail(List, Node);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 节点移除 */
static inline void DList_Remove(DListBody *List, DListNode *Node)
{
    DListNode *Prev = DNode_GetPrev(Node);
    DListNode *Next = DNode_GetNext(Node);
    
    /* 链表头尾节点一并检查 */
    if (Prev == NULL)
        DList_SetHead(List, Next);
    if (Next == NULL)
        DList_SetTail(List, Prev);
    /* 接触关联索引 */
    if (Prev != NULL)
        DNode_SetNext(Prev, Next);
    if (Next != NULL)
        DNode_SetPrev(Next, Prev);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 向后遍历链表宏 */
#define DList_Traverse_Backward(List, Node) \
    for (Node = DList_GetHead(List); Node != NULL; Node = DNode_GetNext(Node))
/* 向前遍历链表宏 */
#define DList_Traverse_Forward(List, Node)  \
    for (Node = DList_GetTail(List); Node != NULL; Node = DNode_GetPrev(Node))

#endif

#define LISTITERATOR_SINGLELIST
#ifdef  LISTITERATOR_SINGLELIST
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 单向链表宏迭代器 */
struct ListIterator_SingleListNode {
    struct ListIterator_SingleListNode *Near;//单向链表临近项
    void  *Member;
};
struct ListIterator_SingleListBody {
    struct ListIterator_SingleListNode *Head;//单向链表头
    struct ListIterator_SingleListNode *Tail;//单向链表尾
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 核心转义 */
/* 对于(list)它的成员是head或tail */
/* 对于(node)它的成员是touch */
typedef struct ListIterator_SingleListBody SListBody;
typedef struct ListIterator_SingleListNode SListNode;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 单向链表体抽象 */

static inline SList_Reset(SListBody *List)
{
    List->Head = NULL;
    List->Tail = NULL;
}

static inline SList_SetHead(SListBody *List, SListNode *Node)
{
    List->Head = Node;
}

static inline SList_SetTail(SListBody *List, SListNode *Node)
{
    List->Tail = Node;
}

static inline SListNode * SList_GetHead(SListBody *List)
{
    return List->Head;
}

static inline SListNode * SList_GetTail(SListBody *List)
{
    return List->Tail;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 单向链表点抽象 */

static inline void SNode_Reset(SListNode *Node)
{
    Node->Near = NULL;
    Node->Member = NULL;
}

static inline void SNode_SetNear(SListNode *Node, SListNode *Near)
{
    Node->Near = Near;
}

static inline void SNode_SetMember(SListNode *Node, void *Member)
{
    Node->Member = Member;
}

static inline SListNode * SNode_GetNear(SListNode *Node)
{
    return Node->Near;
}

static inline void * SNode_GetMember(SListNode *Node)
{
    return Node->Member;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 节点头追加 */
static inline void SList_Prepend(SListBody *List, SListNode *Node)
{
    SListNode *Head = SList_GetHead(List);
    
    SNode_SetNear(Node, Head);
    if (Head == NULL)
        SList_SetTail(List, Node);
    SList_SetHead(List, Node);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 节点尾追加 */
static inline void SList_Append(SListBody *List, SListNode *Node)
{
    SListNode *Tail = SList_GetTail(List);
    
    if (Tail != NULL)
        SNode_SetNear(Tail, Node);
    if (Tail == NULL)
        SList_SetHead(List, Node);
    SList_SetTail(List, Node);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 节点插入(单链表插入需要指定目标节点,Target不为NULL) */
static inline void SList_Insert(SListBody *List, SListNode *Target, SListNode *Node)
{
    SNode_SetNear(Node, SNode_GetNear(Target));
    SNode_SetNear(Target, Node);
    if (SList_GetTail(List) == Target)
        SList_SetTail(List, Node);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 节点删除(单链表删除需要指定目标节点) */
static inline void SList_Remove(SListBody *List, SListNode *Target, SListNode *Node)
{
    if (Target != NULL)
        SNode_SetNear(Target, SNode_GetNear(Node));
    if (SList_GetHead(List) == Node)
        SList_SetHead(List, SNode_GetNear(Node));
    if (SList_GetTail(List) == Node)
        SList_SetTail(List, Target);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 遍历链表(这里不带成员量) */
#define SList_Traserve(List, Node) \
    for (Node = SList_GetHead(List); Node != NULL; Node = SNode_GetNear(Node))
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif

#endif
