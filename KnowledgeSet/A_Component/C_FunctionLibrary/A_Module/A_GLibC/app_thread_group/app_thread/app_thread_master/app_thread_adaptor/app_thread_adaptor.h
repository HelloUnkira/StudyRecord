#ifndef APP_THREAD_ADAPTOR_H
#define APP_THREAD_ADAPTOR_H

/* APP thread adaptor: */

/*系统线程资源配置
 *主线程优先级最高
 *混合中断线程次之
 *混合常规线程次之
 *其余自定线程次之同时并优先级保持相同(让系统使用时间片轮转调度)
 *注意:这个文件不可以以任何方式被其他地方引用
 *     它的唯一使用者为app_thread_master.c
 */

/* 线程组模组初始化 */
void app_thread_master_ready(void);
void app_thread_mix_irq_ready(void);
void app_thread_mix_custom_ready(void);
/* 线程组服务例程 */
void app_thread_master_routine(void);
void app_thread_mix_irq_routine(void);
void app_thread_mix_custom_routine(void);

#if 0
#elif APP_OS_IS_LINUX
#include "app_thread_adaptor_linux.h"
#elif APP_OS_IS_ZEPHYR
#include "app_thread_adaptor_zephyr.h"
#elif APP_OS_IS_FREERTOS
#include "app_thread_adaptor_freertos.h"
#else
#error "unknown os"
#endif

#endif
