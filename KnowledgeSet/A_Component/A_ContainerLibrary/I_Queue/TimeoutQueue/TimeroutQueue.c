#include "TimeroutQueue.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 停止,中止,终止软件定时器 */
void TQ_Node_Stop(TQ_Queue *Queue, TQ_Node *Node)
{
    /* 检查等待者队列 */
    if (Queue->List == NULL)
        return;
    /* 检查第一个等待者 */
    if (Queue->List == Node) {
        Queue->List  = Queue->List->Near;
        /* 如果后面还有等待者,需要将约减值累加到后面 */
        if (Queue->List != NULL)
            Queue->List->ReduceCount += Node->ReduceCount;
        return;
    }
    /* 遍历等待者队列 */
    for (TQ_Node *Current = Queue->List; Current->Near != NULL; Current = Current->Near)
        /* 如果下一等待着是目标 */
        if (Current->Near == Node) {
            Current->Near  = Current->Near->Near;
            /* 如果后面还有等待者,需要将约减值累加到后面 */
            if (Current->Near != NULL)
                Current->Near->ReduceCount += Node->ReduceCount;
            return;
        }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 启动软件定时器 */
bool TQ_Node_Start(TQ_Queue *Queue, TQ_Node *Node)
{
    /* 首项检查,不要加入俩个相同的定时器到队列中去 */
    for (TQ_Node *Current = Queue->List; Current != NULL; Current = Current->Near)
        if (Current == Node)
            return false;
    /* 开始前重加载定时器,我们默认0周期为错误操作 */
    Node->Near        = NULL;
    Node->ReduceCount = Node->Peroid;
    if ((Node->ReduceCount = Node->Peroid) == 0)
        return false;
    /* 检查等待者队列 */
    if (Queue->List == NULL) {
        Queue->List  = Node;
        return true;
    }
    /* 比较第一个等待者 */
    if (Queue->List->ReduceCount >= Node->ReduceCount) {
        Queue->List->ReduceCount -= Node->ReduceCount;
        Node->Near = Queue->List;
        Queue->List = Node;
        return true;
    }
    Node->ReduceCount -= Queue->List->ReduceCount;
    /*  */
    TQ_Node *Current = NULL;
    /* 遍历等待者队列 */
    for (Current = Queue->List; Current->Near != NULL; Current = Current->Near) {
        /* 如果下一等待着不是目标 */
        if (Node->ReduceCount >= Current->Near->ReduceCount) {
            Node->ReduceCount -= Current->Near->ReduceCount;
            continue;
        }
        /* 如果下一等待着是目标 */
        Current->Near->ReduceCount -= Node->ReduceCount;
        Node->Near = Current->Near;
        Current->Near = Node;
        return true;
    }
    /* 添加到末尾 */
    Current->Near = Node;
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 约减软件定时器 */
void TQ_Node_Reduce(TQ_Queue *Queue)
{
    /* 检查等待者队列 */
    if (Queue->List == NULL)
        return;
    /* 约减首项等待者即可,有些值恰好为0,那么当到达时不需约减 */
    if (Queue->List->ReduceCount != 0)
        Queue->List->ReduceCount--;
    /* 检查首项是否约减完毕 */
    while (Queue->List != NULL && Queue->List->ReduceCount == 0) {
        /* 将首项剥离出队列头 */
        TQ_Node *Node = Queue->List;
        Queue->List = Queue->List->Near;
        /* 检查是否需要重加载 */
        if (Node->Reload == true)
            TQ_Node_Start(Queue, Node);
        /* 将该事件通过回调投掷出去 */
        if (Queue->EventThrowCallback != NULL)
            Queue->EventThrowCallback(Node);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 运行软件定时器 */
void TQ_Node_Execute(TQ_Node *Node)
{
    if (Node->ReduceCallback != NULL)
        Node->ReduceCallback(Node->Parameter);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
