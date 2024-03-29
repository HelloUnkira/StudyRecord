#ifndef SGUI_ITERATOR_H
#define SGUI_ITERATOR_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:
 * 宏迭代器(这里只需要提供部分迭代策略)
 * 注意:此处使用的是普通版本的宏迭代器
 *      稳定在几个单位级别的额外字节开销
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_ITERATOR_DL
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 双向链表迭代器 */
struct SGuiItrator_DoubleListNode {
    struct SGuiItrator_DoubleListNode *Prev;//双向链表上一节点
    struct SGuiItrator_DoubleListNode *Next;//双向链表下一节点
    void  *Member;
};
struct SGuiItrator_DoubleListBody {
    struct SGuiItrator_DoubleListNode *Head;//双向链表头
    struct SGuiItrator_DoubleListNode *Tail;//双向链表尾
};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 核心转义 */
typedef struct SGuiItrator_DoubleListBody SGui_DList;
typedef struct SGuiItrator_DoubleListNode SGui_DNode;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_DListReset(SGui_DList *List)
{
    List->Head = NULL;
    List->Tail = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_DListSetHead(SGui_DList *List, SGui_DNode *Node)
{
    List->Head = Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_DListSetTail(SGui_DList *List, SGui_DNode *Node)
{
    List->Tail = Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline SGui_DNode * SGui_DListGetHead(SGui_DList *List)
{
    return List->Head;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline SGui_DNode * SGui_DListGetTail(SGui_DList *List)
{
    return List->Tail;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_DNodeReset(SGui_DNode *Node)
{
    Node->Prev = NULL;
    Node->Next = NULL;
    Node->Member = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_DNodeSetPrev(SGui_DNode *Node, SGui_DNode *Prev)
{
    Node->Prev = Prev;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_DNodeSetNext(SGui_DNode *Node, SGui_DNode *Next)
{
    Node->Next = Next;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_DNodeSetMember(SGui_DNode *Node, void *Member)
{
    Node->Member = Member;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline SGui_DNode * SGui_DNodeGetPrev(SGui_DNode *Node)
{
    return Node->Prev;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline SGui_DNode * SGui_DNodeGetNext(SGui_DNode *Node)
{
    return Node->Next;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * SGui_DNodeGetMember(SGui_DNode *Node)
{
    return Node->Member;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点前插入(节点Target为NULL时为头部插入) */
static inline void SGui_DListInsertPrepend(SGui_DList *List, SGui_DNode *Target, SGui_DNode *Node)
{
    if (Target == NULL) {
        SGui_DNode *Head = SGui_DListGetHead(List);
        /* 更新插入节点 */
        SGui_DNodeSetPrev(Node, NULL);
        SGui_DNodeSetNext(Node, Head);
        if (Head != NULL)
            SGui_DNodeSetPrev(Head, Node);
        if (Head == NULL)
            SGui_DListSetTail(List, Node);
        SGui_DListSetHead(List, Node);
    }
    if (Target != NULL) {
        SGui_DNode *Prev = SGui_DNodeGetPrev(Target);
        /* 更新插入节点 */ 
        SGui_DNodeSetPrev(Node, Prev);
        SGui_DNodeSetNext(Node, Target);
        /* 更新后项节点 */
        SGui_DNodeSetPrev(Target, Node);
        /* 前项节点存在时 */
        if (Prev != NULL)
            SGui_DNodeSetNext(Prev, Node);
        if (Prev == NULL)
            SGui_DListSetHead(List, Node);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点后插入(节点Target为NULL时为尾部插入) */
static inline void SGui_DListInsertAppend(SGui_DList *List, SGui_DNode *Target, SGui_DNode *Node)
{
    if (Target == NULL) {
        SGui_DNode *Tail = SGui_DListGetTail(List);
        /* 更新插入节点 */
        SGui_DNodeSetPrev(Node, Tail);
        SGui_DNodeSetNext(Node, NULL);
        if (Tail != NULL)
            SGui_DNodeSetNext(Tail, Node);
        if (Tail == NULL)
            SGui_DListSetHead(List, Node);
        SGui_DListSetTail(List, Node);
    }
    if (Target != NULL) {
        SGui_DNode *Next = SGui_DNodeGetNext(Target);
        /* 更新插入节点 */
        SGui_DNodeSetPrev(Node, Target);
        SGui_DNodeSetNext(Node, Next);
        /* 更新前项节点 */
        SGui_DNodeSetNext(Target, Node);
        /* 后项节点存在时 */
        if (Next != NULL)
            SGui_DNodeSetPrev(Next, Node);
        if (Next == NULL)
            SGui_DListSetTail(List, Node);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点移除 */
static inline void SGui_DListRemove(SGui_DList *List, SGui_DNode *Node)
{
    SGui_DNode *Prev = SGui_DNodeGetPrev(Node);
    SGui_DNode *Next = SGui_DNodeGetNext(Node);
    
    /* 链表头尾节点一并检查 */
    if (Prev == NULL)
        SGui_DListSetHead(List, Next);
    if (Next == NULL)
        SGui_DListSetTail(List, Prev);
    /* 接触关联索引 */
    if (Prev != NULL)
        SGui_DNodeSetNext(Prev, Next);
    if (Next != NULL)
        SGui_DNodeSetPrev(Next, Prev);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 向后遍历链表宏 */
#define SGui_DListTraverse_Backward(List, Node) \
    for (Node = SGui_DListGetHead(List); Node != NULL; Node = SGui_DNodeGetNext(Node))
/* 向前遍历链表宏 */
#define SGui_DListTraverse_Forward(List, Node)  \
    for (Node = SGui_DListGetTail(List); Node != NULL; Node = SGui_DNodeGetPrev(Node))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_ITERATOR_SL
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单向链表宏迭代器 */
struct SGuiIterator_SingleListNode {
    struct SGuiIterator_SingleListNode *Near;//单向链表临近项
    void  *Member;
};
struct SGuiIterator_SingleListBody {
    struct SGuiIterator_SingleListNode *Head;//单向链表头
    struct SGuiIterator_SingleListNode *Tail;//单向链表尾
};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 核心转义 */
/* 对于(list)它的成员是head或tail */
/* 对于(node)它的成员是touch */
typedef struct SGuiIterator_SingleListBody SGui_SList;
typedef struct SGuiIterator_SingleListNode SGui_SNode;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_SListReset(SGui_SList *List)
{
    List->Head = NULL;
    List->Tail = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_SListSetHead(SGui_SList *List, SGui_SNode *Node)
{
    List->Head = Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_SListSetTail(SGui_SList *List, SGui_SNode *Node)
{
    List->Tail = Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline SGui_SNode * SGui_SListGetHead(SGui_SList *List)
{
    return List->Head;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline SGui_SNode * SGui_SListGetTail(SGui_SList *List)
{
    return List->Tail;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_SNodeReset(SGui_SNode *Node)
{
    Node->Near = NULL;
    Node->Member = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_SNodeSetNear(SGui_SNode *Node, SGui_SNode *Near)
{
    Node->Near = Near;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_SNodeSetMember(SGui_SNode *Node, void *Member)
{
    Node->Member = Member;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline SGui_SNode * SGui_SNodeGetNear(SGui_SNode *Node)
{
    return Node->Near;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * SGui_SNodeGetMember(SGui_SNode *Node)
{
    return Node->Member;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点头追加 */
static inline void SGui_SListPrepend(SGui_SList *List, SGui_SNode *Node)
{
    SGui_SNode *Head = SGui_SListGetHead(List);
    
    SGui_SNodeSetNear(Node, Head);
    if (Head == NULL)
        SGui_SListSetTail(List, Node);
    SGui_SListSetHead(List, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点尾追加 */
static inline void SGui_SListAppend(SGui_SList *List, SGui_SNode *Node)
{
    SGui_SNode *Tail = SGui_SListGetTail(List);
    
    if (Tail != NULL)
        SGui_SNodeSetNear(Tail, Node);
    if (Tail == NULL)
        SGui_SListSetHead(List, Node);
    SGui_SListSetTail(List, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点插入(单链表插入需要指定目标节点,Target不为NULL) */
static inline void SGui_SListInsert(SGui_SList *List, SGui_SNode *Target, SGui_SNode *Node)
{
    SGui_SNodeSetNear(Node, SGui_SNodeGetNear(Target));
    SGui_SNodeSetNear(Target, Node);
    if (SGui_SListGetTail(List) == Target)
        SGui_SListSetTail(List, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点删除(单链表删除需要指定目标节点) */
static inline void SGui_SListRemove(SGui_SList *List, SGui_SNode *Target, SGui_SNode *Node)
{
    if (Target != NULL)
        SGui_SNodeSetNear(Target, SGui_SNodeGetNear(Node));
    if (SGui_SListGetHead(List) == Node)
        SGui_SListSetHead(List, SGui_SNodeGetNear(Node));
    if (SGui_SListGetTail(List) == Node)
        SGui_SListSetTail(List, Target);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 遍历链表(这里不带成员量) */
#define SGui_SListTraserve(List, Node) \
    for (Node = SGui_SListGetHead(List); Node != NULL; Node = SGui_SNodeGetNear(Node))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 下面的接口是定制化扩展接口 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 将一个空闲单元加入节点,并加入到链表(使用最小化索引) */
void SGui_SNodeUnitAdd(SGui_SList *List, void *Unit);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 将一个空闲单元移除节点,并移除出链表(使用最小化索引) */
void SGui_SNodeUnitRemove(SGui_SList *List, void *Unit);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 将一个空闲单元寻找到(使用最小化索引) */
void * SGui_SNodeUnitSearch(SGui_SList *List, uint32_t Index);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
