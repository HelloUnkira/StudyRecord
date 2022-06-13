#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标: 优先队列(侵入式队列, 带头尾节点单向队列) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 从当前链表节点地址获得它的所有者地址(编译时解析) */
#define PQ_GetOwner(Type, Member, MemberAddress)    \
    ((Type *)((uint8_t *)(MemberAddress) - ((uint64_t) &((Type *)0)->Member)))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
struct PriorityQueue {
    void *Parameter;
};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct PriorityQueue PQ_Node;
typedef struct PriorityQueue PQ_Body;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void PQ_ResetQueue(PQ_Body *Queue);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void PQ_ResetNode(PQ_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
PQ_Node * PQ_GetHead(PQ_Body *Queue);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void PQ_EnQueue(PQ_Body *Queue, PQ_Node *Node, bool (*Compare)(PQ_Node *Node1, PQ_Node *Node2));
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void PQ_DeQueue(PQ_Body *Queue, PQ_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
