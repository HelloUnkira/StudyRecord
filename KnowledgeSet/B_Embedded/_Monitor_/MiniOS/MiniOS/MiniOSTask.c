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
#include "MemoryManageInterface.h"
#include "SpinLock.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "MiniOSTask.h"
#include "MiniOSTimer.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MiniOSTaskControlBlock {
    void   (*TaskHandle)(uint32_t Event);   /* 任务体 */
    uint8_t  Priority;                      /* 任务优先级 */
    PQ_Node  TaskNode;                      /* 优先队列任务关联 */
    uint32_t Event;                         /* 事件 */
} MiniOS_TCB;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static SpinLock TaskLock = 0;
static PQ_Body  TaskReadyQueue = {0};
static PQ_Body  TaskBlockQueue = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static bool MiniOS_TCB_Compare(PQ_Node *Node1, PQ_Node *Node2)
{
    uint8_t Priority1 = (PQ_GetOwner(MiniOS_TCB, TaskNode, Node1))->Priority;
    uint8_t Priority2 = (PQ_GetOwner(MiniOS_TCB, TaskNode, Node2))->Priority;
    
    return (Priority1 < Priority2) ? true : false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_ScheduleReady(void)
{
    LOG_WARN(true, "MiniOS_ScheduleReady");
    MemoryManageConfigure();
    PQ_ResetQueue(&TaskReadyQueue);
    PQ_ResetQueue(&TaskBlockQueue);
    
    extern void MiniOS_TimerReady(void);
                MiniOS_TimerReady();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_ScheduleExecute(void)
{
    /* 硬件定时器的启动放在线程执行的开始,其他任务创建结束 */
    void MiniOS_TimerMSCallback(void);
    MiniOS_TimerPortConfigure(MiniOS_TimerMSCallback);
    MiniOS_TimerPortInit();
    
    while (1) {
        PQ_Node *Node = NULL;
        MiniOS_TCB *TCB = NULL;
        
        while (Spin_Lock(&TaskLock) == false);
        Node = PQ_GetHead(&TaskReadyQueue);
        if (Node != NULL) {
            LOG_WARN(true, "MiniOS_ScheduleExecute");
            TCB = PQ_GetOwner(MiniOS_TCB, TaskNode, Node);
            PQ_DeQueue(&TaskReadyQueue, &(TCB->TaskNode));
            PQ_EnQueue(&TaskBlockQueue, &(TCB->TaskNode), MiniOS_TCB_Compare);
        }
        Spin_Unlock(&TaskLock);
        
        if (TCB  != NULL &&
            TCB->TaskHandle != NULL)
            TCB->TaskHandle(TCB->Event);
        
        /* 下面可以挂低功耗,CPU除了RTC其他都可以关闭 */
        static uint32_t IdleCount = 0;
        if (TCB == NULL) {
            IdleCount++;
            if (IdleCount > 10000) {
                /* 此处可进入休眠,因为很久没来事件了 */
                IdleCount = 0;
            }
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MiniOS_TaskHandle MiniOS_TaskCreate(void (*TaskHandle)(uint32_t Event), uint8_t Priority)
{
    MiniOS_TCB *TCB = NULL;
    LOG_WARN(true, "MiniOS_TaskCreate");
    
    while (Spin_Lock(&TaskLock) == false);
    if (MemoryManageHeapTake(sizeof(MiniOS_TCB), &TCB) == false)
        LOG_ERROR(true, "Heap Configure Is Too Small");
    else {
        TCB->TaskHandle = TaskHandle;
        TCB->Priority   = Priority;
        TCB->Event      = 0;
        PQ_ResetNode(&(TCB->TaskNode));
        PQ_EnQueue(&TaskBlockQueue, &(TCB->TaskNode), MiniOS_TCB_Compare);
    }
    Spin_Unlock(&TaskLock);
    return TCB;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TaskDestroy(MiniOS_TaskHandle Handle)
{
    MiniOS_TCB *TCB = Handle;
    LOG_WARN(true, "MiniOS_TaskDestroy");
    while (Spin_Lock(&TaskLock) == false);
    PQ_DeQueue(&TaskReadyQueue, &(TCB->TaskNode));
    PQ_DeQueue(&TaskBlockQueue, &(TCB->TaskNode));
    if (MemoryManageHeapGive(sizeof(MiniOS_TCB), TCB) == false) {
        LOG_ERROR(true, "Error Task Handle");
    }
    Spin_Unlock(&TaskLock);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TaskToggle(MiniOS_TaskHandle Handle, uint32_t Event)
{
    MiniOS_TCB *TCB = Handle;
    TCB->Event = Event;
    LOG_WARN(true, "MiniOS_TaskToggle");
    while (Spin_Lock(&TaskLock) == false);
    PQ_DeQueue(&TaskBlockQueue, &(TCB->TaskNode));
    PQ_EnQueue(&TaskReadyQueue, &(TCB->TaskNode), MiniOS_TCB_Compare);
    Spin_Unlock(&TaskLock);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TaskToggleInterrupt(MiniOS_TaskHandle Handle, uint32_t Event)
{
    MiniOS_TCB *TCB = Handle;
    TCB->Event = Event;
    LOG_WARN(true, "MiniOS_TaskToggleInterrupt");
    PQ_DeQueue(&TaskBlockQueue, &(TCB->TaskNode));
    PQ_EnQueue(&TaskReadyQueue, &(TCB->TaskNode), MiniOS_TCB_Compare);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
