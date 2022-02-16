/*
 *实现目标:
 *    在此处完成对底层操作系统继承与封装
 *    用以保证最为基本的服务可以被提供
 *    其中最为主要的几个部分:
 *    完成线程动作的封装,基本同步原语的封装,软件定时器的封装
 */

#include "app_kernel_package.h"

//创建一个线程
void thread_create(thread_body_t *thread)
{
    k_thread_create(&(thread->new_thread),
                    thread->stack,
                    thread->stack_size,
                    thread->entry,
                    NULL,
                    NULL,
                    NULL,
                    thread->prio,
                    0,
                    K_NO_WAIT);
    return thread;
}

//启用一个线程(或者创建时被启用)
void thread_running(thread_body_t *thread)
{
}

//创建一个信号量
void semaphone_create(semaphone_body_t *semaphone)
{
    k_sem_init(&(semaphone->sem), 0, 100);
}

//获取一个信号量
void semaphone_take(semaphone_body_t *semaphone)
{
    k_sem_take(&(semaphone->sem), K_FOREVER);
}

//发布一个信号量
void semaphone_give(semaphone_body_t *semaphone)
{
    k_sem_give(&(semaphone->sem));
}

//创建一个互斥锁
void mutex_create(mutex_body_t *mutex)
{
    k_mutex_init(&(mutex->mutex));
}

//抢占一个互斥锁
void mutex_take(mutex_body_t *mutex)
{
    k_mutex_lock(&(mutex->mutex), K_FOREVER);
}

//释放一个互斥锁
void mutex_release(mutex_body_t *mutex)
{
    k_mutex_unlock(&(mutex->mutex));
}


//临界区保护
void critical_zone_entry()
{
    __disable_irq();
}

//临界区退出
void critical_zone_exit()
{
    __enable_irq();
}

//毫秒延时
void delay_ms(int32_t ms)
{
    k_msleep(ms);
}

//微秒延时
void delay_us(int32_t us)
{
    k_usleep(us);
}
