#ifndef SOFTWARE_TIMER_H
#define SOFTWARE_TIMER_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 软件定时器:定时约减优先队列 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SoftwareTimer {
    void   (*ReduceCallback)(void *Parameter);  
    void    *Parameter;
    struct   SoftwareTimer *Near;
    uint32_t ReduceCount;
    uint32_t Peroid;
    uint32_t Reload:1;
} STimer;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SoftwareTimerQueue {
    void  (*EventThrowCallback)(STimer *Timer);
    STimer *Timers;
} STimerQueue;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 静态配置软件定时器 */
#define STimer_CONFIGURE(CALLBACK, PARAMETER, PEROID, RELOAD)   \
{                                                               \
    .ReduceCallback = CALLBACK,     /* 约减到0时执行回调 */     \
    .Parameter      = PARAMETER,    /* 执行回调的参数 */        \
    .Peroid         = PEROID,       /* 约减周期 */              \
    .Reload         = RELOAD,       /* 是否自动重加载 */        \
    .ReduceCount    = PEROID,                                   \
    .Near           = NULL,                                     \
}                                                               \
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 动态配置软件定时器 */
static inline void STimer_Configure(STimer  *Timer,
                                    void   (*Callback)(void *Parameter),
                                    void    *Parameter,
                                    uint32_t Peroid,
                                    bool     Reload)
{
    Timer->ReduceCallback = Callback;
    Timer->Parameter      = Parameter;
    Timer->Peroid         = Peroid;
    Timer->Reload         = Reload;
    Timer->ReduceCount    = Peroid;
    Timer->Near           = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void STimerQueue_Configure(STimerQueue *Queue, void (*Callback)(STimer *Timer))
{
    Queue->EventThrowCallback = Callback;
    Queue->Timers = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 停止,中止,终止软件定时器(如果它还在运行,否则什么也不做) */
void STimer_Stop(STimerQueue *Queue, STimer *Timer);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 启动软件定时器(如果它还在运行,否则什么也不做) */
bool STimer_Start(STimerQueue *Queue, STimer *Timer);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 约减软件定时器(将它挂到硬件定时器回调中,通过回调投掷定时器事件)*/
void STimer_Reduce(STimerQueue *Queue);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 运行软件定时器(将它挂到线程中调用,处理从硬件中断中投递过来的定时器事件) */
void STimer_Execute(STimer *Timer);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
