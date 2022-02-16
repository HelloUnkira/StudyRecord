#ifndef APP_KERNEL_PACKAGE_H
#define APP_KERNEL_PACKAGE_H

#include <kernel.h>

//创建规则参见注释
//该平台下的线程创建
typedef struct thread_body {
    struct k_thread new_thread;     //线程空间
    k_thread_stack_t *stack;        //静态分配一个栈
    size_t stack_size;              //栈大小
    k_thread_entry_t entry;         //线程体(强制转化)
    int prio;                       //优先级
} thread_body_t;

//创建一个线程
void thread_create(thread_body_t *thread);
//启用一个线程(或者创建时被启用)
void thread_running(thread_body_t *thread);



//该平台下的信号量创建
typedef struct semaphone_body {
    struct k_sem sem;               //信号量空间
} semaphone_body_t;

//创建一个信号量
void semaphone_create(semaphone_body_t *semaphone);
//获取一个信号量
void semaphone_take(semaphone_body_t *semaphone);
//发布一个信号量
void semaphone_give(semaphone_body_t *semaphone);



//该平台下的互斥锁创建
typedef struct mutex_body {
    struct k_mutex mutex;
} mutex_body_t;

//创建一个互斥锁
void mutex_create(mutex_body_t *mutex);
//抢占一个互斥锁
void mutex_take(mutex_body_t *mutex);
//释放一个互斥锁
void mutex_release(mutex_body_t *mutex);
//注:互斥锁可使用轻量级同步原语替代



//临界区保护
void critical_zone_entry();
//临界区退出
void critical_zone_exit();

//调度器提供的延时服务
void delay_ms(int32_t ms);
void delay_us(int32_t us);

#endif
