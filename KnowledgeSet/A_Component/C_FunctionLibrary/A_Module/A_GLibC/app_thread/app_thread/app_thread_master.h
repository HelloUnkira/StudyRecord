#ifndef APP_THREAD_MASTER_H
#define APP_THREAD_MASTER_H

#include <stdint.h>
#include "app_sys_pipe.h"

/* 主线程管控的子线程组数量 */
#define APP_THREAD_MAX  10

/*@brief        通过从线程ID获得与主线程的交互管道
 *@param[in]    thread_id 线程ID
 *@param[out]   send 发送者管道
 *@param[out]   recv 接受者管道
 */
void app_thread_get_pipe_by_id(uint32_t thread_id, app_pipe_t **send, app_pipe_t **recv);

/*@brief        通过从线程ID获得管道同步资源
 *@param[in]    thread_id 线程ID
 *@param[out]   sem   线程信号量
 */
void app_thread_get_sync_by_id(uint32_t thread_id, app_sem_t **sem);

/*@brief 初始化资源
 */
void app_thread_source_ready(void);

/*@brief 主线程服务例程
 */
void app_thread_master_routine(void);

/*@brief        线程组接收一个事件包裹
 *@param[in]    thread_id 线程ID
 *@param[in]    package   事件包裹
 */
void app_thread_package_notify(app_package_t *package);

#endif
