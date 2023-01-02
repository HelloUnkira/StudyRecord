/*
 *实现目的:
 *    杂项事件不归属与其他线程模组时放于此处
 */

#include "app_os_adaptor.h"
#include "app_sys_pipe.h"
#include "app_thread_master.h"
#include "app_thread_mix_custom.h"
#include "app_thread_all.h"

/*@brief 混合事件线程服务例程
 */
void app_thread_mix_custom_routine(void)
{
    APP_OS_PRINT("\napp_thread_mix_custom_routine\n");
    app_sem_t  *sem  = NULL;
    app_pipe_t *recv = NULL;
    app_package_t package = {0};
    app_thread_get_pipe_by_id(app_thread_id_mix_custom, NULL, &recv);
    app_thread_get_sync_by_id(app_thread_id_mix_custom, &sem);
    /* 主流程 */
    while (true) {
        app_sem_take(sem);
        while (app_sys_pipe_package_num(recv)) {
            app_sys_pipe_take(recv, &package);
            APP_OS_PRINT("\n");
            APP_OS_PRINT("thread mix custom recv a package\n");
            APP_OS_PRINT("package send_tid:%u\n", package.send_tid);
            APP_OS_PRINT("package recv_tid:%u\n", package.recv_tid);
            APP_OS_PRINT("package module:%u\n", package.module);
            APP_OS_PRINT("package data:%p\n",   package.data);
            APP_OS_PRINT("package size:%u\n",   package.size);
            APP_OS_PRINT("\n");
            /* 现在我们需要处理这个包裹了 */
            switch (package.module) {
            case app_thread_mix_custom_work:
                if (package.data != NULL &&
                    package.size == sizeof(app_thread_mix_custom_work_t)) {
                    app_thread_mix_custom_work_t *work = package.data;
                    if (work->routine != NULL)
                        work->routine(work->parameter);
                }
                break;
            default:
                break;
            }
        }
    }
}
