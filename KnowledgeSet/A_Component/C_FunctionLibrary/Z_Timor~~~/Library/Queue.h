#ifndef QUEUE_H
#define QUEUE_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标: 泛型队列(侵入式队列, 带头尾节点单向队列) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 从当前链表节点地址获得它的所有者地址(编译时解析) */
#define Queue_GetOwner(Type, Member, MemberAddress)     \
    ((Type *)((uint8_t *)(MemberAddress) - ((uintptr_t) &((Type *)0)->Member)))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct GenericsQueue {
    union {
        struct GenericsQueue *Near;
        struct GenericsQueue *Head;
        struct GenericsQueue *Tail;
    };
} Queue;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void Queue_ResetQueue(Queue *QueueHead, Queue *QueueTail)
{
    QueueHead->Head = NULL;
    QueueTail->Tail = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void Queue_ResetNode(Queue *Node)
{
    Node->Near = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void Queue_EnQueue(Queue *QueueHead, Queue *QueueTail, Queue *Node)
{
    Node->Near = NULL;
    if (QueueTail->Tail != NULL) {
        QueueTail->Tail->Near = Node;
        QueueTail->Tail = Node;
    }
    if (QueueTail->Tail == NULL) {
        QueueTail->Tail = Node;
        QueueHead->Head = Node;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline Queue * Queue_DeQueue(Queue *QueueHead, Queue *QueueTail)
{
    Queue *Node = NULL;
    
    if (QueueHead->Head != NULL) {
        Node = QueueHead->Head;
        QueueHead->Head = QueueHead->Head->Near;
        if (QueueTail->Head == NULL)
            QueueTail->Tail = NULL;
    }
    
    return Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
