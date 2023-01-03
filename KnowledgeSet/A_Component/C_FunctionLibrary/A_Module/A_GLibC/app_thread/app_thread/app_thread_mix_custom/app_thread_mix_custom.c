/*
 *实现目的:
 *    杂项事件不归属与其他线程模组时放于此处
 */

#include "app_thread_interface.h"

/*@brief 混合事件线程服务例程
 */
void app_thread_mix_custom_routine(void)
{
    app_sem_t  *sem  = NULL;
    app_pipe_t *pipe = NULL;
    app_package_t package = {0};
    app_thread_get_sync_by_id(app_thread_id_mix_custom, &sem);
    app_thread_get_pipe_by_id(app_thread_id_mix_custom, &pipe);
    /* 模组初始化 */
    app_module_clock_ready();
    /* 主流程 */
    while (true) {
        app_sem_take(sem);
        while (app_sys_pipe_package_num(pipe)) {
            app_sys_pipe_take(pipe, &package);
            /* 现在我们需要处理这个包裹了 */
            switch (package.module) {
            case app_thread_mix_custom_work:
                /* 我们利用data和size传递内部特殊信息 */
                if (package.data != NULL && package.size == (sizeof(void **) * 2)) {
                    void (*routine)(void *parameter) = ((void **)package.data)[0];
                    void  *parameter                 = ((void **)package.data)[1];
                    routine(parameter);
                }
                break;
            case app_thread_mix_custom_clock:
                if (package.event == app_thread_mix_custom_clock_event_update)
                    app_module_clock_event_update();
                if (package.event == app_thread_mix_custom_clock_timestamp_update) {
                    uint64_t utc_new = package.data != NULL ? *(uint64_t *)package.data : 0;
                    app_module_clock_timestamp_update(utc_new);
                }
                break;
            default:
                APP_OS_PRINT("\n");
                APP_OS_PRINT("thread mix custom pipe a unknown package\n");
                APP_OS_PRINT("package send_tid:%u\n", package.send_tid);
                APP_OS_PRINT("package recv_tid:%u\n", package.recv_tid);
                APP_OS_PRINT("package module:%u\n",   package.module);
                APP_OS_PRINT("package event:%u\n",    package.event);
                APP_OS_PRINT("package data:%p\n",     package.data);
                APP_OS_PRINT("package size:%u\n",     package.size);
                APP_OS_PRINT("\n");
                break;
            }
        }
    }
}
