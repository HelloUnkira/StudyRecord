/*
 *实现目标:
 *    按文件裁剪的打印宏
 */

#include "app_os_interface.h"

void app_os_log1(char *format, ...);
void app_os_log2(char *format, va_list list);

void app_os_log(bool status, char *file, uint32_t line, char *flag, char *format, ...)
{
    va_list  list;
    va_start(list, format);
    
    if (status == true)
    {
        app_os_log1("[%s][%u][%s]", file, line, flag);
        app_os_log2(format, list);
        app_os_log1("\r\n");
    }
    else
    {
        app_os_log1(format, list);
    }
    
    va_end(list);
}

/* 以下内容适配到平台 */

#if 0
#elif APP_OS_IS_LINUX

void app_os_log1(char *format, ...)
{
    va_list  list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
}

void app_os_log2(char *format, va_list list)
{
    vprintf(format, list);
}

#elif APP_OS_IS_ZEPHYR
#error "arch os not adaptor yet"
#elif APP_OS_IS_FREERTOS
#error "arch os not adaptor yet"
#else
#error "unknown os"
#endif
