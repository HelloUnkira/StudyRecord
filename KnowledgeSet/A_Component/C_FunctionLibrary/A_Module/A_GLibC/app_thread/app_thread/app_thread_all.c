/*
 *实现目的:
 *    线程组汇总
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "app_os_adaptor.h"
#include "app_thread_master.h"
#include "app_thread_all.h"

/*@brief 准备所有线程
 */
static void app_thread_all_ready(void);

/*@brief 启动所有线程
 */
static void app_thread_all_execute(void);

/*@brief 初始化所有线程及其附属资源
 *       启动所有线程
 */
void app_thread_work(void)
{
    app_thread_source_ready();
    app_thread_all_ready();
    app_thread_all_execute();
}

/*系统线程资源配置
 *注意:
 *主线程优先级最高
 *混合中断线程次之
 *其余中断后次之并优先级保持相同(使用时间片轮转调度)
 */

#if 0
#elif APP_OS_IS_LINUX
#include "app_thread_adaptor_linux.h"
#elif APP_OS_IS_ZEPHYR
#include "app_thread_adaptor_zephyr.h"
#else
#error "unknown os"
#endif

/*@brief 准备所有线程
 */
static void app_thread_all_ready(void)
{
    app_thread_process(&app_thread_master);
    app_thread_process(&app_thread_mix_irq);
    app_thread_process(&app_thread_mix_custom);
}

/*@brief 启动所有线程
 */
static void app_thread_all_execute(void)
{
}
