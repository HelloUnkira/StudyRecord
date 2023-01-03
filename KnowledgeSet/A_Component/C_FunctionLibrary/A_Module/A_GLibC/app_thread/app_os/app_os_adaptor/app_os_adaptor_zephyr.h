#ifndef APP_OS_ADAPTOR_ZEPHYR_H
#define APP_OS_ADAPTOR_ZEPHYR_H

/* APP需要对平台进行的封装体 */
/* 因为敏感信息都在准备阶段出现 */
/* 同时APP会在一个专门的地点和时刻准备这些资源 */
/* 所以仅需要对所有平台做一个全通配的最低配置 */

#include <kernel.h>

/* app thread */
typedef struct {
    struct k_thread      thread;        /* 线程空间 */
    k_thread_stack_t    *stack;         /* 静态分配一个栈 */
    k_thread_entry_t     entry;         /* 线程体 */
    size_t               stack_size;    /* 栈大小 */
    int                  prio;          /* 优先级 */
} app_thread_t;

/*@brief        准备与执行线程
 *              创建一个线程并启动(线程创建时立即启动)
 *@param[in]    thread 静态实例
 */
static inline void app_thread_process(app_thread_t *thread)
{
    k_thread_create(&thread->thread,
                     thread->stack,
                     thread->stack_size,
                     thread->entry,
                     NULL, NULL, NULL,
                     thread->prio, 0,
                     K_NO_WAIT);
}

/*@brief 当前环境是否为中断环境(注意:当且仅当必要的使用)
 */
static inline bool app_os_not_in_irq(void)
{        
    uint32_t interrupt;
    
    /* 获取当前正在执行的中断数 */
    __asm volatile("mrs %0, ipsr" : "=r" (interrupt)::"memory");
    
    if(interrupt == 0)
        return true;
    else
        return false;
}

/* app semaphone */
typedef struct {
    struct k_sem sem;   /* 信号量空间 */
} app_sem_t;

/*@brief        创建一个信号量并准备好使用
 *@param[in]    sem 静态实例
 */
static inline void app_sem_process(app_sem_t *sem)
{
    k_sem_init(&sem->sem, 0, 100);
}

/*@brief        获取一个信号量
 *@param[in]    sem 静态实例
 */
static inline void app_sem_take(app_sem_t *sem)
{
    k_sem_take(&sem->sem, K_FOREVER);
}

/*@brief        发布一个信号量
 *@param[in]    sem 静态实例
 */
static inline void app_sem_give(app_sem_t *sem)
{
    k_sem_give(&sem->sem);
}

/* app mutex */
typedef struct {
    struct k_mutex mutex;   /* 互斥锁空间 */
} app_mutex_t;

/*@brief        创建一个互斥锁并准备好使用
 *@param[in]    mutex 静态实例
 */
static inline void app_mutex_process(app_mutex_t *mutex)
{
    k_mutex_init(&mutex->mutex);
}

/*@brief        抢占一个互斥锁
 *@param[in]    mutex 静态实例
 */
static inline void app_mutex_take(app_mutex_t *mutex)
{
    k_mutex_lock(&mutex->mutex, K_FOREVER);
}

/*@brief        释放一个互斥锁
 *@param[in]    mutex 静态实例
 */
static inline void app_mutex_give(app_mutex_t *mutex)
{
    k_mutex_unlock(&mutex->mutex);
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
    __disable_irq();
}

/*@brief 临界区退出(注意:当且仅当必要的使用)
 */
static inline void app_critical_exit(void)
{
    __enable_irq();
}

/*@brief 毫秒延时
 */
static inline void app_delay_ms(uint32_t ms)
{
    /* 延时可能会涉及到调度 */
    /* 取决于操作系统单次轮转的时间片大小及RTC精度 */
    k_msleep(ms);
}

/*@brief 微秒延时
 */
static inline void app_delay_us(uint32_t us)
{
    /* 延时可能会涉及到调度 */
    /* 取决于操作系统单次轮转的时间片大小及RTC精度 */
    k_usleep(us);
}

/*@brief 打印输出转接宏
 */
#define APP_OS_PRINT

#endif