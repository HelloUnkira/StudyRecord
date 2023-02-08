/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "SoftwareTimer.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 停止,中止,终止软件定时器 */
void ST_Node_Stop(ST_Queue *Queue, ST_Node *Timer)
{
    /* 检查等待者队列 */
    if (Queue->Timers == NULL)
        return;
    /* 检查第一个等待者 */
    if (Queue->Timers == Timer) {
        Queue->Timers  = Queue->Timers->Near;
        /* 如果后面还有等待者,需要将约减值累加到后面 */
        if (Queue->Timers != NULL)
            Queue->Timers->ReduceCount += Timer->ReduceCount;
        return;
    }
    /* 遍历等待者队列 */
    for (ST_Node *Current = Queue->Timers; Current->Near != NULL; Current = Current->Near)
        /* 如果下一等待着是目标 */
        if (Current->Near == Timer) {
            Current->Near  = Timer->Near;
            /* 如果后面还有等待者,需要将约减值累加到后面 */
            if (Current->Near != NULL)
                Current->Near->ReduceCount += Timer->ReduceCount;
            return;
        }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 启动软件定时器 */
bool ST_Node_Start(ST_Queue *Queue, ST_Node *Timer)
{
    if (Timer->ReduceCallback == NULL || Timer->Peroid == 0)
        return false;
    /* 首项检查,不要加入俩个相同的定时器到队列中去 */
    for (ST_Node *Current = Queue->Timers; Current != NULL; Current = Current->Near)
        if (Current == Timer)
            return false;
    /* 开始前重加载定时器,我们默认0周期为错误操作 */
    Timer->Near        = NULL;
    Timer->ReduceCount = Timer->Peroid;
    /* 检查等待者队列 */
    if (Queue->Timers == NULL) {
        Queue->Timers  = Timer;
        return true;
    }
    /* 比较第一个等待者 */
    if (Queue->Timers->ReduceCount >= Timer->ReduceCount) {
        Queue->Timers->ReduceCount -= Timer->ReduceCount;
        Timer->Near = Queue->Timers;
        Queue->Timers = Timer;
        return true;
    }
    Timer->ReduceCount -= Queue->Timers->ReduceCount;
    /*  */
    ST_Node *Current = NULL;
    /* 遍历等待者队列 */
    for (Current = Queue->Timers; Current->Near != NULL; Current = Current->Near) {
        /* 如果下一等待着不是目标 */
        if (Timer->ReduceCount >= Current->Near->ReduceCount) {
            Timer->ReduceCount -= Current->Near->ReduceCount;
            continue;
        }
        /* 如果下一等待着是目标 */
        Current->Near->ReduceCount -= Timer->ReduceCount;
        Timer->Near = Current->Near;
        Current->Near = Timer;
        return true;
    }
    /* 添加到末尾 */
    Current->Near = Timer;
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 约减软件定时器 */
void ST_Queue_Reduce(ST_Queue *Queue)
{
    /* 检查等待者队列 */
    if (Queue->Timers == NULL)
        return;
    /* 约减首项等待者即可,有些值恰好为0,那么当到达时不需约减 */
    if (Queue->Timers->ReduceCount != 0)
        Queue->Timers->ReduceCount--;
    /* 检查首项是否约减完毕 */
    while (Queue->Timers != NULL && Queue->Timers->ReduceCount == 0) {
        /* 将首项剥离出队列头 */
        ST_Node *Timer = Queue->Timers;
        Queue->Timers = Queue->Timers->Near;
        /* 检查是否需要重加载 */
        if (Timer->Reload == true)
            ST_Node_Start(Queue, Timer);
        /* 将该事件通过回调投掷出去 */
        if (Queue->EventThrowCallback != NULL)
            Queue->EventThrowCallback(Timer);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 运行软件定时器 */
void ST_Node_Execute(ST_Node *Timer)
{
    if (Timer->ReduceCallback != NULL)
        Timer->ReduceCallback(Timer->Parameter);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
