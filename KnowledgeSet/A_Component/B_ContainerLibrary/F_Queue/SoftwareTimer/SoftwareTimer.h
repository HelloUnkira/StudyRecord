#ifndef SOFTWARE_TIMER_H
#define SOFTWARE_TIMER_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 软件定时器:定时约减优先队列 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SoftwareTimer_Node {
    void   (*ReduceCallback)(void *Parameter);  
    void    *Parameter;
    struct   SoftwareTimer_Node *Near;
    uint32_t ReduceCount;
    uint32_t Peroid;
    uint32_t Reload:1;
} ST_Node;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SoftwareTimer_Queue {
    void   (*EventThrowCallback)(ST_Node *Timer);
    ST_Node *Timers;
} ST_Queue;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 静态配置软件定时器 */
#define ST_Node_CONFIGURE(CALLBACK, PARAMETER, PEROID, RELOAD)  \
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
static inline void ST_Node_Configure(ST_Node *Timer,
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
static inline void ST_Queue_Configure(ST_Queue *Queue, void (*Callback)(ST_Node *Timer))
{
    Queue->EventThrowCallback = Callback;
    Queue->Timers = NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 停止,中止,终止软件定时器(如果它还在运行,否则什么也不做) */
void ST_Node_Stop(ST_Queue *Queue, ST_Node *Timer);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 启动软件定时器(如果它还在运行,否则什么也不做) */
bool ST_Node_Start(ST_Queue *Queue, ST_Node *Timer);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 约减软件定时器(将它挂到硬件定时器回调中,通过回调投掷定时器事件)*/
void ST_Queue_Reduce(ST_Queue *Queue);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 运行软件定时器(将它挂到线程中调用,处理从硬件中断中投递过来的定时器事件) */
void ST_Node_Execute(ST_Node *Timer);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
