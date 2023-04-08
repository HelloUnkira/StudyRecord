#ifndef SINGLE_LIST_H
#define SINGLE_LIST_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:泛型链表(侵入式单双链表, 带头尾节点) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 从当前链表节点地址获得它的所有者地址(编译时解析) */
#define SL_GetOwner(Type, Member, MemberAddress)    \
    ((Type *)((uint8_t *)(MemberAddress) - ((uintptr_t) &((Type *)0)->Member)))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
struct SingleList_Node {
    void *Parameter;
};

struct SingleList_List {
    void *Parameter[2];
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
void SL_List_Reset(SL_List *List);
void SL_Node_Reset(SL_Node *Node);
SL_Node * SL_List_GetHead(SL_List *List);
SL_Node * SL_List_GetTail(SL_List *List);
SL_Node * SL_Node_GetNear(SL_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 节点头追加 */
void SL_List_Prepend(SL_List *List, SL_Node *Node);
/* 节点尾追加 */
void SL_List_Append(SL_List *List, SL_Node *Node);
/* 节点插入(单链表插入需要指定目标节点,Target不为NULL) */
void SL_List_Insert(SL_List *List, SL_Node *Target, SL_Node *Node);
/* 节点删除(单链表删除需要指定前项节点) */
void SL_List_Remove(SL_List *List, SL_Node *Target, SL_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 遍历链表(这里不带成员量) */
#define SL_List_Traserve(List, Node) \
    for (SL_Node *Node = SL_List_GetHead(List); Node != NULL; Node = SL_Node_GetNear(Node))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif