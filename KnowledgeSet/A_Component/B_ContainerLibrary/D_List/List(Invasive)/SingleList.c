/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单向链表迭代器 */
struct SingleList_Node {
    struct SingleList_Node *Near; //单向链表就近项
};

struct SingleList_List {
    struct SingleList_Node *Head; //单向链表头
    struct SingleList_Node *Tail; //单向链表尾
};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 核心转义 */
typedef struct SingleList_List SL_List;
typedef struct SingleList_Node SL_Node;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SL_List_Reset(SL_List *List)
{
    List->Head = NULL;
    List->Tail = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SL_Node_Reset(SL_Node *Node)
{
    Node->Near = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
SL_Node * SL_List_GetHead(SL_List *List)
{
    return List->Head;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
SL_Node * SL_List_GetTail(SL_List *List)
{
    return List->Head;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
SL_Node * SL_Node_GetNear(SL_Node *Node)
{
    return Node->Near;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点头追加 */
void SL_List_Prepend(SL_List *List, SL_Node *Node)
{
    Node->Near = List->Head;
    if (List->Head == NULL)
        List->Tail = Node;
    List->Head = Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点尾追加 */
void SL_List_Append(SL_List *List, SL_Node *Node)
{
    Node->Near = NULL;
    if (List->Tail != NULL)
        List->Tail->Near = Node;
    if (List->Tail == NULL)
        List->Head = Node;
    List->Tail = Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点插入(单链表插入需要指定目标节点,Target不为NULL) */
void SL_List_Insert(SL_List *List, SL_Node *Target, SL_Node *Node)
{
    Node->Near = Target->Near;
    Target->Near = Node;
    if (List->Tail == Target)
        List->Tail = Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点删除(单链表删除需要指定前项节点) */
void SL_List_Remove(SL_List *List, SL_Node *Target, SL_Node *Node)
{
    if (Target != NULL)
        Target->Near = Node->Near;
    if (List->Head == Node)
        List->Head = Node->Near;
    if (List->Tail == Node)
        List->Tail = Target;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
