#include <string.h>
#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_AnimationAdaptorEventDispatch(SGui_Handle Widget, uint8_t Label)
{
    /* 如果引擎正在工作,停止它 */
    if (SGui_EngineWaitReady(false) == true)
        SGui_EngineAbort();
        SGui_EngineWaitReady(true);
    
    uint32_t Length = sizeof(SGui_Handle) + sizeof(uint8_t);
    uint8_t *Data   = SGUI_ALLOC(Length);
    
    memcpy(Data, &Widget, sizeof(SGui_Handle));
    Data[Length - 1] = Label;
    
    SGui_EventEnqueue(SGui_EventType_Animation, Length, Data);
    
    /* 恢复停止的引擎 */
    SGui_EngineRun();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_AnimationAdaptorEventExecute(uint32_t Length, uint8_t *Data)
{
        SGui_Handle Handle = SGUI_EVENT_INVALID;
        uint8_t *label  = 0;
        
        memcpy(&Handle, Data, Length - 1);
        
        SGuiWidget *Widget = SGui_HandleSourceGet(Handle);
        
        Widget->Callback(SGui_EventType_Animation, 1, Data + sizeof(SGui_Handle));
        
        SGUI_FREE(Data);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
