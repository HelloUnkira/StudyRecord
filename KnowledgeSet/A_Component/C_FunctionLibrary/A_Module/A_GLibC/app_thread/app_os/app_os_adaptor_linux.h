#ifndef APP_OS_ADAPTOR_LINUX_H
#define APP_OS_ADAPTOR_LINUX_H

/* APP需要对平台进行的封装体 */
/* 因为敏感信息都在准备阶段出现 */
/* 同时APP会在一个专门的地点和时刻准备这些资源 */
/* 所以仅需要对所有平台做一个全通配的最低配置 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* app thread */
typedef struct {
    pthread_t       thread;                 /* 线程空间 */
    pthread_attr_t  attribute;              /* 线程属性 */
    int             priority;               /* 优先级 */
    size_t          stacksize;              /* 堆栈大小 */
    void *        (*routine)(void *args);   /* 线程体 */
    void *          args;                   /* 线程参数 */
} app_thread_t;

/*@brief        准备与执行线程
 *              创建一个线程并启动(线程创建时立即启动)
 *@param[in]    thread 静态实例
 */
static inline void app_thread_process(app_thread_t *thread)
{
    thread->stacksize = 8 * 1024 * 1024;
    struct sched_param param = {.sched_priority = thread->priority};
    pthread_attr_init(&thread->attribute);
    pthread_attr_setschedparam(&thread->attribute,  &param);
    //pthread_attr_setschedpolicy(&thread->attribute,  SCHED_RR);
    pthread_attr_setguardsize(&thread->attribute,    7 * 1024 *1024);
    pthread_attr_setstacksize(&thread->attribute,    thread->stacksize);
    pthread_attr_setinheritsched(&thread->attribute, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&thread->attribute,  PTHREAD_CREATE_JOINABLE);
    pthread_create(&thread->thread,
                   &thread->attribute,
                    thread->routine,
                    thread->args);
}

/*@brief 当前环境是否为中断环境(注意:当且仅当必要的使用)
 */
static inline bool app_os_not_in_irq(void)
{
    return true;
}

/* app semaphore */
typedef struct {
    sem_t sem;  /* 信号量空间 */
} app_sem_t;

/*@brief        创建一个信号量并准备好使用
 *@param[in]    sem 静态实例
 */
static inline void app_sem_process(app_sem_t *sem)
{
    sem_init(&sem->sem, 0, 100);
}

/*@brief        获取一个信号量
 *@param[in]    sem 静态实例
 */
static inline void app_sem_take(app_sem_t *sem)
{
    sem_post(&sem->sem);
}

/*@brief        发布一个信号量
 *@param[in]    sem 静态实例
 */
static inline void app_sem_give(app_sem_t *sem)
{
    sem_wait(&sem->sem);
}

/* app mutex */
typedef struct {
    pthread_mutex_t mutex;  /* 互斥锁空间 */
} app_mutex_t;

/*@brief        创建一个互斥锁并准备好使用
 *@param[in]    mutex 静态实例
 */
static inline void app_mutex_process(app_mutex_t *mutex)
{
    pthread_mutex_init(&mutex->mutex, NULL);
}

/*@brief        抢占一个互斥锁
 *@param[in]    mutex 静态实例
 */
static inline void app_mutex_take(app_mutex_t *mutex)
{
    pthread_mutex_lock(&mutex->mutex);
}

/*@brief        释放一个互斥锁
 *@param[in]    mutex 静态实例
 */
static inline void app_mutex_give(app_mutex_t *mutex)
{
    pthread_mutex_unlock(&mutex->mutex);
}

/*@brief        内存分配
 *@param[in]    size 分配空间字节大小
 #@retval       分配空间,失败为NULL
 */
static inline void * app_mem_alloc(uint32_t size)
{
    return malloc(size);
}

/*@brief        内存释放
 *@param[in]    pointer 分配空间
 */
static inline void app_mem_free(void *pointer)
{
    free(pointer);
}

/*@brief 临界区保护(注意:当且仅当必要的使用)
 */
static inline void app_critical_enter(void)
{
    /* Linux不需要临界区保护,因为资源不会被中断打断 */
}

/*@brief 临界区退出(注意:当且仅当必要的使用)
 */
static inline void app_critical_exit(void)
{
    /* Linux不需要临界区保护,因为资源不会被中断打断 */
}

/*@brief 毫秒延时
 */
static inline void app_delay_ms(uint32_t ms)
{
    /* 延时可能会涉及到调度 */
    /* 取决于操作系统单次轮转的时间片大小及RTC精度 */
    usleep(ms * 1000);
}

/*@brief 微秒延时
 */
static inline void app_delay_us(uint32_t us)
{
    /* 延时可能会涉及到调度 */
    /* 取决于操作系统单次轮转的时间片大小及RTC精度 */
    usleep(us);
}

/*@brief 打印输出转接宏
 */
#define APP_OS_PRINT    printf

#endif