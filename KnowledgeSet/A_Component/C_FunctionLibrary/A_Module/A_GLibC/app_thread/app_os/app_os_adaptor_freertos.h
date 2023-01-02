#ifndef APP_OS_ADAPTOR_FREERTOS_H
#define APP_OS_ADAPTOR_FREERTOS_H

/* APP需要对平台进行的封装体 */
/* 因为敏感信息都在准备阶段出现 */
/* 同时APP会在一个专门的地点和时刻准备这些资源 */
/* 所以仅需要对所有平台做一个全通配的最低配置 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* app thread */
typedef struct {
    TaskHandle_t         handle;        /* 线程空间 */
    TaskFunction_t       task;          /* 线程体 */
    const char          *name;          /* 线程名字 */
    StackType_t          stack;         /* 栈空间 */
    uint16_t             stack_size;    /* 栈空间大小 */
    UBaseType_t          priority;      /* 优先级 */
} app_thread_t;

/*@brief        准备与执行线程
 *              创建一个线程并启动(线程创建时立即启动)
 *@param[in]    thread 静态实例
 */
static inline void app_thread_process(app_thread_t *thread)
{
    xTaskCreate(thread->task,
                thread->name,
                thread->stack_size,
                NULL,
                thread->priority,
               &thread->handle);
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
    SemaphoreHandle_t sem;  /* 信号量空间 */
} app_sem_t;

/*@brief        创建一个信号量并准备好使用
 *@param[in]    sem 静态实例
 */
static inline void app_sem_process(app_sem_t *sem)
{
    sem->sem = xSemaphoreCreateCounting(100, 0);
}

/*@brief        获取一个信号量
 *@param[in]    sem 静态实例
 */
static inline void app_sem_take(app_sem_t *sem)
{
    if (app_os_not_in_irq())
        xSemaphoreTake(sem->sem, portMAX_DELAY);
    else
        xSemaphoreTakeFromISR(sem->sem, portMAX_DELAY);
}

/*@brief        发布一个信号量
 *@param[in]    sem 静态实例
 */
static inline void app_sem_give(app_sem_t *sem)
{
    if (app_os_not_in_irq())
        xSemaphoreGive(sem->sem);
    else
        xSemaphoreGiveFromISR(sem->sem);
}

/* app mutex */
typedef struct {
    SemaphoreHandle_t mutex;  /* 互斥锁空间 */
} app_mutex_t;

/*@brief        创建一个互斥锁并准备好使用
 *@param[in]    mutex 静态实例
 */
static inline void app_mutex_process(app_mutex_t *mutex)
{
    mutex->mutex = xSemaphoreCreateMutex();
}

/*@brief        抢占一个互斥锁
 *@param[in]    mutex 静态实例
 */
static inline void app_mutex_take(app_mutex_t *mutex)
{
    xSemaphoreTake(mutex->mutex, portMAX_DELAY);
}

/*@brief        释放一个互斥锁
 *@param[in]    mutex 静态实例
 */
static inline void app_mutex_give(app_mutex_t *mutex)
{
    xSemaphoreGive(mutex->mutex);
}

/*@brief        内存分配
 *@param[in]    size 分配空间字节大小
 #@retval       分配空间,失败为NULL
 */
static inline void * app_mem_alloc(uint32_t size)
{
    return pvPortMalloc(size);
}

/*@brief        内存释放
 *@param[in]    pointer 分配空间
 */
static inline void app_mem_free(void *pointer)
{
    vPortFree(pointer);
}

/*@brief 临界区保护(注意:当且仅当必要的使用)
 */
static inline void app_critical_enter(void)
{
    taskDISABLE_INTERRUPTS();
}

/*@brief 临界区退出(注意:当且仅当必要的使用)
 */
static inline void app_critical_exit(void)
{
    taskENABLE_INTERRUPTS();
}

/*@brief 毫秒延时
 */
static inline void app_delay_ms(uint32_t ms)
{
    /* 延时可能会涉及到调度 */
    /* 取决于操作系统单次轮转的时间片大小及RTC精度 */
    vTaskDelay(ms);
}

/*@brief 微秒延时
 */
static inline void app_delay_us(uint32_t us)
{
    /* 延时可能会涉及到调度 */
    /* 取决于操作系统单次轮转的时间片大小及RTC精度 */
    //根据平台提供;
}

#endif