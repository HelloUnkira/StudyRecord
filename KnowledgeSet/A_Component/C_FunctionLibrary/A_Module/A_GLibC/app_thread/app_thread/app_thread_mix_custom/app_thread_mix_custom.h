#ifndef APP_THREAD_MIX_CUSTOM_H
#define APP_THREAD_MIX_CUSTOM_H

#include "app_module_clock.h"

typedef enum {  /* 线程模组 */
    app_thread_mix_custom_work,         /* 工作队列模组 */
    app_thread_mix_custom_clock,        /* 时钟模组 */
} app_thread_mix_custom_module;

typedef struct {    /* 工作队列模组数据项 */
    void (*routine)(void *parameter);   /* 数据处理例程 */
    void  *parameter;                   /* 数据处理例程参数 */
} app_thread_mix_custom_work_t;

typedef enum {  /* 时钟模组事件 */
    app_thread_mix_custom_clock_event_update,
    app_thread_mix_custom_clock_timestamp_update,
} app_thread_mix_custom_clock_event;

#endif
