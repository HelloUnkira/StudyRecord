//C std lib
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define   LOG_PART_STATUS     0   /* 1:ENABLE,0:DISABLE */
#define   LOG_PART_LEVEL      0   /* 0:DEBUG,1:INFO,2:WARN,3:ERROR,4:NONE */
#include "LogMessage.h"
#include "PriorityQueue.h"
#include "SoftwareTimer.h"
#include "SpinLock.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "MiniOSTask.h"
#include "MiniOSTimer.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static SpinLock    TimerLock = 0;
static STimerQueue TimerQueue = {0};
static PQ_Body     TimerExecuteQueue = {0};
static uint64_t    TimerExecutePriority = 0;    /* 假设每秒一百万次事件计算,一万年也不会溢出 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static MiniOS_TaskHandle TimerHandle = NULL;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MiniOSTaskTimer {
    STimer  *Timer;
    uint32_t Priority;
    PQ_Node  TimerNode;
} MiniOS_TT;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static bool MiniOS_TT_Compare(PQ_Node *Node1, PQ_Node *Node2)
{
    uint8_t Priority1 = (PQ_GetOwner(MiniOS_TT, TimerNode, Node1))->Priority;
    uint8_t Priority2 = (PQ_GetOwner(MiniOS_TT, TimerNode, Node2))->Priority;
    
    return (Priority1 < Priority2) ? true : false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void MiniOS_TimerEventCallback(STimer *Timer)
{
    /* 它应被注册到硬件定时器中断回调,所以无需加锁 */
    MiniOS_TT *TT = NULL;
    LOG_WARN(true, "MiniOS_TimerEventCallback");
    if (MemoryManageHeapTake(sizeof(MiniOS_TT), &TT) == false)
        LOG_ERROR(true, "Heap Configure Is Too Small");
    else {
        TT->Timer    = Timer;
        TT->Priority = TimerExecutePriority++;
        PQ_ResetNode(&(TT->TimerNode));
        PQ_EnQueue(&TimerExecuteQueue, &(TT->TimerNode), MiniOS_TT_Compare);
    }
    MiniOS_TaskToggleInterrupt(TimerHandle, 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TimerMSCallback(void)
{
    /* 它应被注册到硬件定时器中断回调,所以无需加锁 */
    LOG_WARN(true, "MiniOS_TimerMSCallback");
    STimer_Reduce(&TimerQueue);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TimerTask(uint32_t Event)
{
    PQ_Node *Node = NULL;
    MiniOS_TT *TT = NULL;
    
    while(Spin_Lock(&TimerLock) == false);
    Node = PQ_GetHead(&TimerExecuteQueue);
    if (Node != NULL) {
        LOG_WARN(true, "MiniOS_TimerTask");
        TT = PQ_GetOwner(MiniOS_TT, TimerNode, Node);
        PQ_DeQueue(&TimerExecuteQueue, &(TT->TimerNode));
        STimer_Execute(TT->Timer);
        if (MemoryManageHeapGive(sizeof(MiniOS_TT), TT) == false)
            LOG_ERROR(1, "Error Task Handle");
    }
    Spin_Unlock(&TimerLock);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TimerReady(void)
{
    LOG_WARN(true, "MiniOS_TimerReady");
    PQ_ResetQueue(&TimerExecuteQueue);
    TimerHandle = MiniOS_TaskCreate(MiniOS_TimerTask, 0);
    STimerQueue_Configure(&TimerQueue, MiniOS_TimerEventCallback);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TimerStop(MiniOS_TimerHandle Handle)
{
    STimer *Timer = Handle;
    LOG_WARN(true, "MiniOS_TimerStop");
    while(Spin_Lock(&TimerLock) == false);
    STimer_Stop(&TimerQueue, Timer);
    Spin_Unlock(&TimerLock);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
bool MiniOS_TimerStart(MiniOS_TimerHandle Handle)
{
    STimer *Timer = Handle;
    LOG_WARN(true, "MiniOS_TimerStart");
    while(Spin_Lock(&TimerLock) == false);
    bool Result = STimer_Start(&TimerQueue, Timer);
    Spin_Unlock(&TimerLock);
    return Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TimerDestroy(MiniOS_TimerHandle Handle)
{
    STimer *Timer = Handle;
    LOG_WARN(true, "MiniOS_TimerDestroy");
    while(Spin_Lock(&TimerLock) == false);
    STimer_Stop(&TimerQueue, Timer);
    if (MemoryManageHeapGive(sizeof(TimerQueue), &Timer) == false)
        LOG_ERROR(true, "Error Task Handle");
    Spin_Unlock(&TimerLock);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MiniOS_TimerHandle MiniOS_TimerCreate(void   (*Callback)(void *Parameter),
                                      void    *Parameter,
                                      uint32_t Peroid,
                                      bool     Reload)
{
    STimer *Timer = NULL;
    LOG_WARN(true, "MiniOS_TimerCreate");
    while(Spin_Lock(&TimerLock) == false);
    if (MemoryManageHeapTake(sizeof(STimer), &Timer) == false)
        LOG_ERROR(1, "Heap Configure Is Too Small");
    else
        STimer_Configure(Timer, Callback, Parameter, Peroid, Reload);
    Spin_Unlock(&TimerLock);
    return Timer;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
