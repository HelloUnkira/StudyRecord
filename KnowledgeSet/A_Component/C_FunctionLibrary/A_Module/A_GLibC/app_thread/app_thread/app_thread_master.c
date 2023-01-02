/*
 *实现目标:
 *    获得所有受管控线程传递的管道信息
 *    将管道中的全部信息摘出
 *    选择性的转交给对应的线程
 *灾难的产生:
 *    明细信息的发源地与去处是一件过于繁杂的事宜
 *    控制线程最为直接的目的是要实现中心化管理
 *    这意味着它需要得到所有明细才能精准无误的
 *    对数据进行合理的分发,构建一个体系化的容器去
 *    保存这些数据容器是一项有挑战性的工作
 *    在实际情况下我们反而发现
 *    最基本的容器居然更能高效的完成指定的工作
 */

#include "app_os_adaptor.h"
#include "app_sys_pipe.h"
#include "app_thread_master.h"
#include "app_thread_all.h"

#if     APP_THREAD_MAX <= app_thread_id_all
#error "APP_THREAD_MAX is too small"
#endif

static const uint32_t app_thread_number = app_thread_id_all;
/* 管道集合,为每一个线程分发一套传输管道 */
static app_pipe_t app_thread_pipe_src[APP_THREAD_MAX] = {0};
static app_pipe_t app_thread_pipe_dst[APP_THREAD_MAX] = {0};
/* 信号量,控制线程组的进动 */
static app_sem_t app_thread_sem = {0};
static app_sem_t app_thread_sem_set[APP_THREAD_MAX] = {0};

/*@brief        通过从线程ID获得与主线程的交互管道
 *@param[in]    thread_id 线程ID
 *@param[out]   send 发送者管道
 *@param[out]   recv 接受者管道
 */
void app_thread_get_pipe_by_id(uint32_t thread_id, app_pipe_t **send, app_pipe_t **recv)
{
    if (thread_id <= app_thread_number) {
        if (send != NULL)
           *send = &app_thread_pipe_src[thread_id];
        if (recv != NULL)
           *recv = &app_thread_pipe_dst[thread_id];
    }
}

/*@brief        通过从线程ID获得管道同步资源
 *@param[in]    thread_id 线程ID
 *@param[out]   sem   线程信号量
 */
void app_thread_get_sync_by_id(uint32_t thread_id, app_sem_t **sem)
{
    if (thread_id <= app_thread_number) {
        *sem = &app_thread_sem_set[thread_id];
    }
}

/*@brief 初始化资源
 */
void app_thread_source_ready(void)
{
    app_sem_process(&app_thread_sem);
    for (uint32_t idx = 0; idx < app_thread_number; idx++) {
        app_sys_pipe_ready(&app_thread_pipe_src[idx]);
        app_sys_pipe_ready(&app_thread_pipe_dst[idx]);
        app_sem_process(&app_thread_sem_set[idx]);
    }
}

/*@brief 主线程服务例程
 */
void app_thread_master_routine(void)
{
    APP_OS_PRINT("\napp_thread_master_routine\n");
    app_sem_t  *sem  = NULL;
    app_pipe_t *send = NULL;
    app_pipe_t *recv = NULL;
    app_package_t package = {0};
    /* 主流程 */
    while (true) {
        app_sem_take(&app_thread_sem);
        for (uint32_t idx = 0; idx < app_thread_number; idx++) {
            app_thread_get_pipe_by_id(idx, &send, NULL);
            while (app_sys_pipe_package_num(send)) {
                app_sys_pipe_take(send, &package);
                app_thread_get_pipe_by_id(package.recv_tid, NULL, &recv);
                app_thread_get_sync_by_id(package.recv_tid, &sem);
                app_sys_pipe_give(recv, &package);
                app_sem_give(sem);
            }
        }
    }
}

/*@brief        线程组接收一个事件包裹
 *@param[in]    thread_id 线程ID
 *@param[in]    package   事件包裹
 */
void app_thread_package_notify(app_package_t *package)
{
    app_pipe_t *send = NULL;
    app_thread_get_pipe_by_id(package->send_tid, &send, NULL);
    app_sys_pipe_give(send, package);
    app_sem_give(&app_thread_sem);
}
