/*
 *实现目的:
 *    有一些事件在中断环境下产生
 *前提条件:
 *    在中断环境下产生的事件
 *    中断服务例程一般不能完整的处理该事件所有的任务
 *    从实际角度出发大家会选择"立即触发"的软件定时器
 *    这其实相当于使用OS提供的软件定时器线程
 *    提供了一个非中断的环境下处理该事件
 *    如果大量使用这种方式
 *    势必造成软件定时器的响应延迟
 *    所以混合中断线程现在代替了这种策略
 *    之所以单独起线程,则是希望通过设置较高优先级
 *    用以区分中断环境下与常规环境产生的事件紧急度
 */

#include "app_os_adaptor.h"
#include "app_sys_pipe.h"
#include "app_thread_master.h"
#include "app_thread_mix_irq.h"
#include "app_thread_all.h"

/*@brief 混合中断线程服务例程
 */
void app_thread_mix_irq_routine(void)
{
    APP_OS_PRINT("\napp_thread_mix_irq_routine\n");
    app_sem_t  *sem  = NULL;
    app_pipe_t *recv = NULL;
    app_package_t package = {0};
    app_thread_get_pipe_by_id(app_thread_id_mix_irq, NULL, &recv);
    app_thread_get_sync_by_id(app_thread_id_mix_irq, &sem);
    /* 主流程 */
    while (true) {
        app_sem_take(sem);
        while (app_sys_pipe_package_num(recv)) {
            app_sys_pipe_take(recv, &package);
            APP_OS_PRINT("\n");
            APP_OS_PRINT("thread mix irq recv a package\n");
            APP_OS_PRINT("package send_tid:%u\n", package.send_tid);
            APP_OS_PRINT("package recv_tid:%u\n", package.recv_tid);
            APP_OS_PRINT("package module:%u\n", package.module);
            APP_OS_PRINT("package data:%p\n",   package.data);
            APP_OS_PRINT("package size:%u\n",   package.size);
            APP_OS_PRINT("\n");
            /* 现在我们需要处理这个包裹了 */
            switch (package.module) {
            case app_thread_mix_irq_work:
                if (package.data != NULL &&
                    package.size == sizeof(app_thread_mix_irq_work_t)) {
                    app_thread_mix_irq_work_t *work = package.data;
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
