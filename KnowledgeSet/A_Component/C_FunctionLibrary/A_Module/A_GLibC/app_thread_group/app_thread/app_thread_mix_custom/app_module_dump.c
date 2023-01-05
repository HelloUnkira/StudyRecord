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
    APP_OS_LOG_WARN("app_module_dump_mem_to_storage");
    /* ... */
    app_module_system_dump_over();
}

/*@brief 资源数据从外存转储到内存
 */
void app_module_dump_storage_to_mem(void)
{
    APP_OS_LOG_WARN("app_module_dump_storage_to_mem");
    /* ... */
}
