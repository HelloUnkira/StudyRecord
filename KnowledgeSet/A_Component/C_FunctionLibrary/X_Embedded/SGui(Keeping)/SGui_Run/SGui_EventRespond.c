#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 引擎适配器(目标:动画事件投递) */
void SGui_EventAnimationDispatch(SGui_Handle Handle, uint8_t Label)
{
    uint32_t Length = sizeof(SGui_Handle) + sizeof(uint8_t);
    uint8_t *Data   = SGUI_ALLOC(Length);
    ((SGui_Handle *)Data)[0] = Handle;
    Data[Length - 1] = Label;
    
    SGui_Event Event = {
        .EventType  = SGui_EventType_Animation,
        .Parameter1 = Length,
        .Parameter2 = Data,
    };
    
    SGui_EventEnqueue(SGui_EventGetEventQueueType(&Event), &Event);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 引擎适配器(目标:动画事件执行) */
uint32_t SGui_EventAnimationExecute(SGui_Event *Event)
{
    uint8_t *Data = Event->Parameter2;
    /* 动画事件来源于指定的控件,最终也只需要派发给指定的控件 */
    SGui_Widget *Widget = SGui_HandleSourceGet(((SGui_Handle *)Data)[0]);
    if (Widget != NULL && Widget->Callback != NULL)
        Widget->Callback(Event);
    SGUI_LOGMESSAGE("SGui_EventAnimationExecute:%p", Widget);
    SGUI_FREE(Data);
    return SGui_EventStatus_Over;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
