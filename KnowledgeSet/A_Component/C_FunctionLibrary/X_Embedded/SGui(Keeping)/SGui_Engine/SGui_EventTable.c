#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//#include ...
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 适配层,通配事件表,一个或多个类型的事件归属于一个特定的事件集合 */
static const SGui_EventAdaptor EventAdaptorTable[SGui_EventType_All] = {
    /* 事件,响应回调,从属事件集合 */
    [SGui_EventType_Animation] = {SGui_EventAnimationExecute, SGui_EventQueue_Main},
};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EventAdaptorTableConfigure(void)
{
    SGui_EventAdaptorRegister((SGui_EventAdaptor *)EventAdaptorTable, SGui_EventType_All);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
