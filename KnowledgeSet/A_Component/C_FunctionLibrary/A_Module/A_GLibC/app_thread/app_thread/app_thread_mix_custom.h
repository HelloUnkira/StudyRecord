#ifndef APP_THREAD_MIX_CUSTOM_H
#define APP_THREAD_MIX_CUSTOM_H

typedef struct {
    void (*routine)(void *parameter);   /* 数据处理例程 */
    void  *parameter;                   /* 数据处理例程参数 */
} app_thread_mix_custom_work_t;

typedef enum {
    app_thread_mix_custom_work,         /* 工作队列模组 */
} app_thread_mix_custom_module;

/*@brief 混合事件线程服务例程
 */
void app_thread_mix_custom_routine(void);

#endif