#ifndef SGUI_EVENT_H
#define SGUI_EVENT_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 错误的事件(或者空事件) */
#define SGUI_EVENT_INVALID  0xFFFFFFFF
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 注意:事件携带数据不额外进行层级转发,数据生命期自行维护 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef uint32_t (*SGui_EventCallback)(uint32_t EventType, uint32_t Parameter1, uint8_t *Parameter2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件为空判断 */
bool SGui_EventSetIsEmpty(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件入队列 */
void SGui_EventEnqueue(uint32_t EventType, uint32_t Parameter1, uint8_t *Parameter2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件出队列 */
void SGui_EventDequeue(uint32_t *EventType, uint32_t *Parameter1, uint8_t **Parameter2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件同步(引擎专用) */
void SGui_EventSync(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件同步回调钩子注册 */
void SGui_EventSyncRegister(void (*Lock)(void), void (*Unlock)(void),
                            void (*Notify)(void), void (*Wait)(void));
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_INTERNAL_TEST
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_Event_Test(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
