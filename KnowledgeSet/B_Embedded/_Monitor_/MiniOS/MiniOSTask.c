//C std lib
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define   LOG_PART_STATUS     0   /* 1:ENABLE,0:DISABLE */
#define   LOG_PART_LEVEL      0   /* 0:DEBUG,1:INFO,2:WARN,3:ERROR,4:NONE */
#include "LogMessage.h"
#include "MemoryManageInterface.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "MiniOSQueue.h"
#include "MiniOSCritical.h"
#include "MiniOSTask.h"
#include "MiniOSTimer.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MiniOSTaskControlBlock {
    void (*TaskHandle)(uint32_t Event);     /* 任务体 */
    uint8_t    Priority;                    /* 任务优先级 */
    MiniOS_PQN TaskNode;                    /* 优先队列任务关联 */
    uint32_t   Event;                       /* 事件 */
} MiniOS_TCB;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static MiniOS_PQB MiniOS_ReadyQueue = {0};
static MiniOS_PQB MiniOS_BlockQueue = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static bool MiniOS_TCB_Compare(MiniOS_PQN *Node1, MiniOS_PQN *Node2)
{
    uint8_t Priority1 = (MiniOS_PQ_GetOwner(MiniOS_TCB, TaskNode, Node1))->Priority;
    uint8_t Priority2 = (MiniOS_PQ_GetOwner(MiniOS_TCB, TaskNode, Node2))->Priority;
    
    return (Priority1 < Priority2) ? true : false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_ScheduleReady(void)
{
    LOG_WARN(true, "MiniOS_ScheduleReady");
    
    MemoryManageConfigure();
    
    MiniOS_PQ_ResetQueue(&MiniOS_ReadyQueue);
    MiniOS_PQ_ResetQueue(&MiniOS_BlockQueue);
    
    extern void MiniOS_TimerReady(void);
                MiniOS_TimerReady();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_ScheduleExecute(void)
{
    while (1) {
        MiniOS_PQN *Node = NULL;
        MiniOS_TCB *TCB  = NULL;
        MiniOSCriticalEnter();
        Node = MiniOS_PQ_CheckHead(&MiniOS_ReadyQueue);
        
        if (Node != NULL) {
            LOG_WARN(true, "MiniOS_ScheduleExecute");
            TCB = MiniOS_PQ_GetOwner(MiniOS_TCB, TaskNode, Node);
            MiniOS_PQ_DeQueue(&MiniOS_ReadyQueue, &(TCB->TaskNode));
            MiniOS_PQ_EnQueue(&MiniOS_BlockQueue, &(TCB->TaskNode), MiniOS_TCB_Compare);
        }
        MiniOSCriticalExit();
        
        if (TCB == NULL)
            continue;
        
        if (TCB->TaskHandle != NULL)
            TCB->TaskHandle(TCB->Event);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MiniOS_TaskHandle MiniOS_TaskCreate(void (*TaskHandle)(uint32_t Event), uint8_t Priority)
{
    LOG_WARN(true, "MiniOS_TaskCreate");
    
    MiniOS_TCB *TCB = NULL;
    
    if (MemoryManageHeapTake(sizeof(MiniOS_TCB), &TCB) == false) {
        LOG_ERROR(true, "Heap Configure Is Too Small");
        return NULL;
    }
    
    TCB->TaskHandle = TaskHandle;
    TCB->Priority   = Priority;
    TCB->Event      = 0;
    
    MiniOS_PQ_ResetNode(&(TCB->TaskNode));
    
    MiniOSCriticalEnter();
    MiniOS_PQ_EnQueue(&MiniOS_BlockQueue, &(TCB->TaskNode), MiniOS_TCB_Compare);
    MiniOSCriticalExit();
    
    return TCB;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TaskDestroy(MiniOS_TaskHandle Handle)
{
    LOG_WARN(true, "MiniOS_TaskDestroy");
    
    MiniOS_TCB *TCB = Handle;
    
    MiniOSCriticalEnter();
    MiniOS_PQ_DeQueue(&MiniOS_ReadyQueue, &(TCB->TaskNode));
    MiniOS_PQ_DeQueue(&MiniOS_BlockQueue, &(TCB->TaskNode));
    
    if (MemoryManageHeapGive(sizeof(MiniOS_TCB), TCB) == false) {
        LOG_ERROR(true, "Error Task Handle");
    }
    
    MiniOSCriticalExit();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TaskToggle(MiniOS_TaskHandle Handle, uint32_t Event)
{
    LOG_WARN(true, "MiniOS_TaskToggle");
    
    MiniOS_TCB *TCB = Handle;
    MiniOSCriticalEnter();
    
    TCB->Event = Event;
    MiniOS_PQ_DeQueue(&MiniOS_BlockQueue, &(TCB->TaskNode));
    MiniOS_PQ_EnQueue(&MiniOS_ReadyQueue, &(TCB->TaskNode), MiniOS_TCB_Compare);
    
    MiniOSCriticalExit();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
