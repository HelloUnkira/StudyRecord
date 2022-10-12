#ifndef SGUI_TIMER_H
#define SGUI_TIMER_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 软件定时器的初始化和反初始化(启动与关闭) */
void SGui_TimerInit(void);
void SGui_TimerDeinit(void);
/* 软件定时器的毫秒回调注册功能 */
void SGui_TimerCallBackRegister(void (*Handler)(void));
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
