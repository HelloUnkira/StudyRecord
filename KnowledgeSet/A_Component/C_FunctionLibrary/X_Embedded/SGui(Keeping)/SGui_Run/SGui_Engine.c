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
void SGui_EngineReady(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_EngineExecute(void)
{
    /* 动画事件投递回调钩子绑定 */
    /* 动画进动回调钩子绑定 */
    /* 就绪动画所使用的定时器 */
    SGui_AnimationRegister(SGui_EngineAdaptorAnimationEventDispatch);
    SGui_TimerConfigure(SGui_AnimationMSHandler);
    SGui_TimerReady();
    /*  */
    #if 1   //临时写入测试引擎的流程通路
    SGui_Handle Handle1 = 1;
    SGui_Handle Handle2 = 2;
    SGui_Handle Handle3 = 3;
    
    SGui_AnimationCreate(Handle1, 1);
    SGui_AnimationCreate(Handle1, 2);
    SGui_AnimationCreate(Handle1, 3);
    SGui_AnimationCreate(Handle2, 1);
    SGui_AnimationCreate(Handle2, 2);
    SGui_AnimationCreate(Handle3, 1);
    
    SGui_AnimationStart(Handle1, 1, 1000, false);
    SGui_AnimationStart(Handle1, 2, 2000, false);
    SGui_AnimationStart(Handle1, 3, 3000, true);
    SGui_AnimationStart(Handle2, 1, 4000, false);
    SGui_AnimationStart(Handle2, 2, 5000, true);
    SGui_AnimationStart(Handle3, 1, 1000, true);
    
    #endif
    /*  */
    /* 启动动画及其定时器 */
    SGui_TimerExecute();
    SGui_AnimationExecute();
    /* 等待事件,并对事件进行派发和处理 */
    while (1) {
        /* 等待事件同步信号产生 */
        SGui_EventSync();
        /* 检查是否产生事件 */
        if (SGui_EventSetIsEmpty() == true)
            continue;
        /* 从事件集合取出事件 */
        uint8_t *Parameter2 = NULL;
        uint32_t Parameter1 = 0;
        uint32_t EventType  = SGUI_EVENT_INVALID;
        /* 取出引擎产生的事件 */
        SGui_EventDequeue(&EventType, &Parameter1, &Parameter2);
        /* 派发事件交付到控件执行 */
        #if SGUI_USE_LOG_MESSAGE
        SGUI_LOGMESSAGE("EventType:%u",  EventType);
        SGUI_LOGMESSAGE("Parameter1:%u", Parameter1);
        for (uint32_t Index = 0; Index < Parameter1; Index++)
            SGUI_LOGMESSAGE("Parameter2:%u", Parameter2[Index]);
        #endif
        /* 派发事件交付到控件执行 */
        /* 事件派发需要层级向控件派发 */
        /* 直到控件选择拒绝向下派发为止 */
    }
    /* 关闭动画及其定时器 */
    SGui_AnimationDormancy();
    SGui_TimerDormancy();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
