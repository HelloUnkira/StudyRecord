/*实现目标:
 *    断言相关的功能组件
 */

#define APP_OS_LOG_LOCAL_STATUS     1
#define APP_OS_LOG_LOCAL_LEVEL      0   /* 0:DEBUG,1:INFO,2:WARN,3:ERROR,4:NONE */

#include "app_thread_interface.h"

void app_module_assert(char *file, uint32_t line, bool condition)
{
    if (condition)
        return;
    /* 资源转储 */
    /* 中止线程组运行 */
    app_thread_set_abort();
    /* 输出错误信息 */
    APP_OS_LOG_ERROR("app_module_assert:[%s][%d]", file, line);
    /* 重启系统 */
    app_os_reset();
}
