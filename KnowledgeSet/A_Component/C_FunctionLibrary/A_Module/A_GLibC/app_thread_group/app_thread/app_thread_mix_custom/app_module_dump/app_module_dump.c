/*实现目标:
 *    一些和资源转储相关的功能组件
 */

#define APP_OS_LOG_LOCAL_STATUS     1
#define APP_OS_LOG_LOCAL_LEVEL      2   /* 0:DEBUG,1:INFO,2:WARN,3:ERROR,4:NONE */

#include "app_thread_interface.h"

/*@brief 资源数据从内存转储到外存
 */
void app_module_dump_mem_to_storage(void)
{
    APP_OS_LOG_WARN("\napp_module_dump_mem_to_storage\n");
    /* ... */
    app_module_system_dump_over();
}

/*@brief 资源数据从外存转储到内存
 */
void app_module_dump_storage_to_mem(void)
{
    /* 设置系统时钟 */
    app_module_clock_t clock = {.year = 2023, .month = 1, .day = 1};
    app_module_clock_to_utc(&clock);
    app_module_clock_set_system_clock(&clock);
    /* 继续添加...... */
    APP_OS_LOG_WARN("\napp_module_dump_storage_to_mem\n");
    /* ... */
}
