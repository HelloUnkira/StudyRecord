#ifndef STACK_H
#define STACK_H
//C std lib
#include <stddef.h>
#include <stdint.h>
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 实现目标: 泛型堆栈(侵入式堆栈, 带栈顶节点堆栈) */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 从当前链表节点地址获得它的所有者地址(编译时解析) */
#define Stack_GetOwner(Type, Member, MemberAddress)     \
    ((Type *)((uint8_t *)(MemberAddress) - ((uint64_t) &((Type *)0)->Member)))
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
typedef struct GenericsStack {
    union {
        struct GenericsStack *Near;
        struct GenericsStack *Top;
    };
} Stack;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static inline void Stack_ResetTop(Stack *StackTop)
{
    StackTop->Top = NULL;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static inline void Stack_ResetNode(Stack *Node)
{
    Node->Near = NULL;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static inline void Stack_Push(Stack *StackTop, Stack *Node)
{
    Node->Near = StackTop->Top;
    StackTop->Top = Node;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static inline Stack * Stack_Pop(Stack *StackTop)
{
    Stack *Node = NULL;
    
    if (StackTop->Top != NULL) {
        Node = StackTop->Top;
        StackTop->Top = StackTop->Top->Near;
    }
    
    return Node;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
