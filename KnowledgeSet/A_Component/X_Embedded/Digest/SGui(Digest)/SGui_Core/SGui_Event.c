#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:
 * 设计简要的事件管理方案
 * 用于传递事件以及事件携带的数据
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SimpleGui_Event {
    void    *Next;
    uint8_t *Parameter2;
    uint32_t Parameter1;
    uint32_t Event;
} SGui_Event;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static SGui_Event *QueueHead = NULL;
static SGui_Event *QueueTail = NULL;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
bool SGui_EventIsEmpty(void)
{
    if (QueueHead != NULL && QueueTail != NULL)
        return false;
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EventEnqueue(uint32_t Event, uint32_t Parameter1, uint8_t *Parameter2)
{
    /* 1.生成事件记录 */
    SGui_Event *Unit = SGUI_ALLOC(sizeof(SGui_Event));
    /* 2.记录事件 */    
    Unit->Next       = NULL;
    Unit->Event      = Event;
    Unit->Parameter1 = Parameter1;
    Unit->Parameter2 = Parameter2;
    /* 3.事件入队列 */
    if (QueueTail != NULL) {
        QueueTail->Next = Unit;
        QueueTail = Unit;
    }
    if (QueueTail == NULL) {
        QueueHead = Unit;
        QueueTail = Unit;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EventDequeue(uint32_t *Event, uint32_t *Parameter1, uint8_t **Parameter2)
{
    /* 1.生成事件记录 */
    SGui_Event *Unit = QueueHead;
    /* 2.事件出队列 */
    if (QueueHead != NULL)
        QueueHead = Unit->Next;
    if (QueueHead == NULL) {
        QueueHead = NULL;
        QueueTail = NULL;
    }
    /* 3.提取事件 */
    if (Unit == NULL)
        *Event = SGUI_EVENT_INVALID;
    if (Unit != NULL) {
        *Event      = Unit->Event;
        *Parameter1 = Unit->Parameter1;
        *Parameter2 = Unit->Parameter2;
         /* 移除事件记录 */
        SGUI_FREE(Unit);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
