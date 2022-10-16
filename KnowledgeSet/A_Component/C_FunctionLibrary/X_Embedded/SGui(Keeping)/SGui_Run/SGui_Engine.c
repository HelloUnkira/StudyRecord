#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:
 * 设计简要的控件绘制引擎
 * 用于进行基本绘制工作及其流程
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void (*EngineThreadOpen)(void)  = NULL;
static void (*EngineThreadClose)(void) = NULL;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EngineThreadRegister(void (*Open)(void), void (*Close)(void))
{
    EngineThreadOpen  = Open;
    EngineThreadClose = Close;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EngineResume(void)
{
    /* 启动屏幕 */
    SGui_ScreenInit();
    /* 启动定时器 */
    SGui_TimerInit();
    /* 启动动画 */
    SGui_AnimationExecute();
    /* 恢复引擎的工作 */
    if (EngineThreadOpen != NULL)
        EngineThreadOpen();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EngineDormancy(void)
{
    /* 中止引擎的工作 */
    if (EngineThreadClose != NULL)
        EngineThreadClose();
    /* 关闭动画 */
    SGui_AnimationDormancy();
    /* 关闭定时器 */
    SGui_TimerDeinit();
    /* 关闭屏幕 */
    SGui_ScreenDeinit();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EngineReady(void)
{
    /* 动画事件投递回调钩子绑定 */
    /* 动画进动回调钩子绑定 */
    /* 就绪动画所使用的定时器 */
    SGui_AnimationRegister(SGui_EventAnimationDispatch);
    SGui_TimerCallBackRegister(SGui_AnimationMSHandler);
    
    /* 启动屏幕 */
    SGui_ScreenInit();
    /* 启动定时器 */
    SGui_TimerInit();
    /* 启动动画 */
    SGui_AnimationExecute();
    
    
    #if SGUI_INTERNAL_TEST
    // SGui_Handle_Test();
    // SGui_ClipRegion1_Test();
    // SGui_ClipRegion2_Test();
    // SGui_Event_Test();
    // SGui_Animation_Test();
    #endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void SGui_EngineCanvas_Test(SGui_Canvas *Canvas)
{
    static uint8_t Count = 0;
    
    SGui_Pixel RectColor = {
        .Alpha = Count * 10,
        .Color.C_R = (Count + 0) % 3 == 0 ? 255 : 0,
        .Color.C_G = (Count + 1) % 3 == 0 ? 255 : 0,
        .Color.C_B = (Count + 2) % 3 == 0 ? 255 : 0,};
    
    Count++;
    
    SGui_DrawFillRect(Canvas, &Canvas->Clip, RectColor);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EngineExecute(void)
{
    #if SGUI_INTERNAL_TEST
    // 测试引擎的流程通路
    SGui_Handle HandleX = ~0;
    SGui_AnimationCreate(HandleX, ~0);
    SGui_AnimationStart(HandleX, ~0, 5000, true);
    // 引擎画布滚动推送一帧
    SGui_ScreenFrameLock();
    SGui_EngineCanvasRoll(SGui_EngineCanvas_Test);
    SGui_ScreenFrameUnlock();
    #endif
    
    /* 等待事件,并对事件进行派发和处理 */
    while (1) {
        /* 等待事件同步信号产生 */
        SGui_EventAdaptorQueueSyncWait();
        /* 按事件队列优先级,检查是否产生事件 */
        for (uint32_t EventQueueType = 0; EventQueueType < SGui_EventQueue_All; EventQueueType++) {
            /* 跳过空事件队列 */
            if (SGui_EventQueueIsEmpty(EventQueueType))
                continue;
            /* 从事件集合取出事件 */
            SGui_Event Event = {0};
            /* 取出产生的事件 */
            SGui_EventDequeue(EventQueueType, &Event);
            /* 检查产生的事件 */
            #if SGUI_USE_LOG_MESSAGE
                SGUI_LOGMESSAGE("EventType:%u",  Event.EventType);
                SGUI_LOGMESSAGE("Parameter1:%u", Event.Parameter1);
                for (uint32_t Index = 0; Index < Event.Parameter1; Index++)
                    SGUI_LOGMESSAGE("Parameter2:%u", Event.Parameter2[Index]);
            #endif
            /* 派发事件交付到控件执行 */
            if (SGui_EventExecute(&Event) == SGui_EventStatus_None)
                SGUI_LOGMESSAGE("Catch Unknown EventType:%u", Event.EventType);
        }
    }

}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
