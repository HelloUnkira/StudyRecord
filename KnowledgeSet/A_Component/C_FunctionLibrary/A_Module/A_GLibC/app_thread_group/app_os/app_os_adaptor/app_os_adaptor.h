#ifndef APP_OS_ADAPTOR_H
#define APP_OS_ADAPTOR_H

/* 实现目标:
 *     APP需要对系统进行的封装体
 *     APP会在一个专门的地点和时刻准备这些资源
 *     对所有平台做一个全通配的最低配置
 */

/* APP OS adaptor: */
#define APP_OS_IS_LINUX     1
#define APP_OS_IS_ZEPHYR    0
#define APP_OS_IS_FREERTOS  0

#include "app_os_adaptor_linux.h"
#include "app_os_adaptor_zephyr.h"
#include "app_os_adaptor_freertos.h"

/*@brief        准备与执行线程
 *              创建一个线程并启动(线程创建时立即启动)
 *@param[in]    thread 静态实例
 */
void app_thread_process(app_thread_t *thread);

/*@brief 当前环境是否为中断环境(注意:当且仅当必要的使用)
 */
bool app_os_not_in_irq(void);

/*@brief        创建一个信号量并准备好使用
 *@param[in]    sem 静态实例
 */
void app_sem_process(app_sem_t *sem);

/*@brief        获取一个信号量
 *@param[in]    sem 静态实例
 */
void app_sem_take(app_sem_t *sem);

/*@brief        发布一个信号量
 *@param[in]    sem 静态实例
 */
void app_sem_give(app_sem_t *sem);

/*@brief        创建一个互斥锁并准备好使用
 *@param[in]    mutex 静态实例
 */
void app_mutex_process(app_mutex_t *mutex);

/*@brief        抢占一个互斥锁(中断环境不可调用)
 *@param[in]    mutex 静态实例
 */
void app_mutex_take(app_mutex_t *mutex);

/*@brief        释放一个互斥锁(中断环境不可调用)
 *@param[in]    mutex 静态实例
 */
void app_mutex_give(app_mutex_t *mutex);

/*@brief        内存分配
 *@param[in]    size 分配空间字节大小
 #@retval       分配空间,失败为NULL
 */
void * app_mem_alloc(uint32_t size);

/*@brief        内存释放
 *@param[in]    pointer 分配空间
 */
void app_mem_free(void *pointer);

/*@brief 临界区保护(注意:当且仅当必要的使用)
 */
void app_critical_enter(void);

/*@brief 临界区退出(注意:当且仅当必要的使用)
 */
void app_critical_exit(void);

/*@brief 毫秒延时
 */
void app_delay_ms(uint32_t ms);

/*@brief 微秒延时
 */
void app_delay_us(uint32_t us);

/*@brief 重启
 */
void app_os_reset(void);

#endif
