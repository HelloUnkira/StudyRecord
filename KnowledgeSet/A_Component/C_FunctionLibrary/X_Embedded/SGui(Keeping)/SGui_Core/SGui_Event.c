#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:
 * 设计简要的事件管理
 * 用于传递事件以及事件携带的数据
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void (*EventQueueSyncLock)(uint32_t   EventQueueType) = NULL;
static void (*EventQueueSyncUnlock)(uint32_t EventQueueType) = NULL;
static void (*EventQueueSyncNotify)(void)                    = NULL;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EventQueueSyncRegister(void (*Lock)(uint32_t   EventQueueType),
                                 void (*Unlock)(uint32_t EventQueueType),
                                 void (*Notify)(void))
{
    EventQueueSyncLock   = Lock;
    EventQueueSyncUnlock = Unlock;
    EventQueueSyncNotify = Notify;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件队列 */
typedef struct SimpleGui_EventQueue {
    SGui_Event *Head;       /* 内部使用 */
    SGui_Event *Tail;       /* 内部使用 */
} SGui_EventQueue;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static SGui_EventQueue EventQueue[SGui_EventQueue_Number] = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
bool SGui_EventQueueIsEmpty(uint32_t EventQueueType)
{
    return EventQueue[EventQueueType].Head == NULL &&
           EventQueue[EventQueueType].Tail == NULL ? true : false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EventEnqueue(uint32_t EventQueueType, SGui_Event *Event)
{
    if (EventQueueSyncLock != NULL)
        EventQueueSyncLock(EventQueueType);
    
    /* 生成事件记录备份 */
    SGui_Event *BackupEvent = SGUI_ALLOC(sizeof(SGui_Event));
    *BackupEvent = *Event;
    /* 事件备份入队列 */
    if (EventQueue[EventQueueType].Tail != NULL) {
        EventQueue[EventQueueType].Tail->Next = BackupEvent;
        EventQueue[EventQueueType].Tail = BackupEvent;
    }
    if (EventQueue[EventQueueType].Tail == NULL) {
        EventQueue[EventQueueType].Head = BackupEvent;
        EventQueue[EventQueueType].Tail = BackupEvent;
    }
    
    if (EventQueueSyncUnlock != NULL)
        EventQueueSyncUnlock(EventQueueType);
    if (EventQueueSyncNotify != NULL)
        EventQueueSyncNotify();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EventDequeue(uint32_t EventQueueType, SGui_Event *Event)
{
    if (EventQueueSyncLock != NULL)
        EventQueueSyncLock(EventQueueType);
    
    /* 取出事件记录备份 */
    SGui_Event *BackupEvent = EventQueue[EventQueueType].Head;
    /* 事件出队列 */
    if (EventQueue[EventQueueType].Head != NULL)
        EventQueue[EventQueueType].Head =  BackupEvent->Next;
    if (EventQueue[EventQueueType].Head == NULL)
        EventQueue[EventQueueType].Tail =  NULL;
    /* 提取事件 */
    if (BackupEvent == NULL)
       (*Event).EventType = SGui_Event_Invalid;
    if (BackupEvent != NULL) {
        *Event = *BackupEvent;
        SGUI_FREE(BackupEvent);
    }
    
    if (EventQueueSyncUnlock != NULL)
        EventQueueSyncUnlock(EventQueueType);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_INTERNAL_TEST
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_Event_Test(void)
{
    uint8_t Data1[7] = "Event1";
    uint8_t Data2[7] = "Event2";
    uint8_t Data3[7] = "Event3";
    
    SGui_Event Event  = {0};
    SGui_Event Event1 = {.EventType = 1, .Parameter1 = 7, .Parameter2 = Data1};
    SGui_Event Event2 = {.EventType = 2, .Parameter1 = 7, .Parameter2 = Data2};
    SGui_Event Event3 = {.EventType = 3, .Parameter1 = 7, .Parameter2 = Data3};
    
    SGui_EventEnqueue(0, &Event2);
    SGui_EventEnqueue(0, &Event1);
    SGui_EventEnqueue(0, &Event3);
    
    while (SGui_EventQueueIsEmpty(0) == false) {
        SGui_EventDequeue(0, &Event);
        SGUI_LOGMESSAGE("EventType:%d,Length:%d,Data:%s",
                         Event.EventType, Event.Parameter1, Event.Parameter2);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
