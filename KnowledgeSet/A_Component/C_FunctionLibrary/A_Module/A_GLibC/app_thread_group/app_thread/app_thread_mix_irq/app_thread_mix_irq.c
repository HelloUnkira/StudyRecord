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

#define APP_OS_LOG_LOCAL_STATUS     1
#define APP_OS_LOG_LOCAL_LEVEL      2   /* 0:DEBUG,1:INFO,2:WARN,3:ERROR,4:NONE */

#include "app_thread_interface.h"


/*@brief 混合中断线程模组初始化
 */
void app_thread_mix_irq_ready(void)
{
}

/*@brief 混合中断线程服务例程
 */
void app_thread_mix_irq_routine(void)
{
    app_sem_t  *sem  = NULL;
    app_pipe_t *pipe = NULL;
    app_package_t package = {0};
    app_thread_get_sync_by_id(app_thread_id_mix_irq, &sem);
    app_thread_get_pipe_by_id(app_thread_id_mix_irq, &pipe);
    /* 主流程 */
    while (true) {
        app_sem_take(sem);
        #if APP_THREAD_CHECK
        if (app_sys_pipe_package_num(pipe) >= APP_THREAD_PACKAGE_MAX)
            APP_OS_LOG_WARN("thread mix irq recv too much package:%u\n",
                            app_sys_pipe_package_num(pipe));
        #endif
        while (app_sys_pipe_package_num(pipe)) {
            app_sys_pipe_take(pipe, &package);
            /* 现在我们需要处理这个包裹了 */
            switch (package.module) {
            case app_thread_mix_irq_work: {
                /* 我们利用data和size传递内部特殊信息 */
                if (package.data != NULL && package.size == (sizeof(void **) * 2)) {
                    void (*routine)(void *parameter) = ((void **)package.data)[0];
                    void  *parameter                 = ((void **)package.data)[1];
                    routine(parameter);
                }
                break;
            }
            default: {
                #if APP_THREAD_CHECK
                APP_OS_LOG_ERROR("\n");
                APP_OS_LOG_ERROR("thread mix irq pipe recv a unknown package\n");
                APP_OS_LOG_ERROR("package send_tid:%u\n", package.send_tid);
                APP_OS_LOG_ERROR("package recv_tid:%u\n", package.recv_tid);
                APP_OS_LOG_ERROR("package module:%u\n",   package.module);
                APP_OS_LOG_ERROR("package event:%u\n",    package.event);
                APP_OS_LOG_ERROR("package data:%p\n",     package.data);
                APP_OS_LOG_ERROR("package size:%u\n",     package.size);
                APP_OS_LOG_ERROR("\n");
                #endif
                break;
            }
            }
        }
    }
}
