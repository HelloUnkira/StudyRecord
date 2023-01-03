#ifndef APP_OS_LOG_H
#define APP_OS_LOG_H

/* 全局宏控开关及其等级 */
#define LOG_GLOBAL_STATUS       1   /* 1:ENABLE,0:DISABLE */

/* 全局打印宏控检测,局部打印宏控检测 */
#if (LOG_GLOBAL_STATUS == 1 && LOG_LOCAL_STATUS == 1)

/* 控制打印集合 */
/* 无格式打印:{内容} */
/* 带格式打印:[文件名][行数][级别]{内容}[换行] */
void app_os_log(bool status, char *file, uint32_t line, char *flag, char *format, ...);

#define LOG_FORMAT(level, string)   \
do {                                \
    if (LOG_LOCAL_LEVEL <= level)   \
        app_os_log(status,          \
                   __FILE__,        \
                   __LINE__,        \
                   string,          \
                   format,          \
                   ##__VA_ARGS__);  \
} while (0)

/* status:true带格式,false不带格式 */
#define APP_OS_LOG_DEBUG(status, format, ...) LOG_FORMAT(0, "D")
#define APP_OS_LOG_INFO( status, format, ...) LOG_FORMAT(1, "I")
#define APP_OS_LOG_WARN( status, format, ...) LOG_FORMAT(2, "W")
#define APP_OS_LOG_ERROR(status, format, ...) LOG_FORMAT(3, "E")

#else

/* status:true带格式,false不带格式 */
#define APP_OS_LOG_DEBUG(status, format, ...)
#define APP_OS_LOG_INFO( status, format, ...)
#define APP_OS_LOG_WARN( status, format, ...)
#define APP_OS_LOG_ERROR(status, format, ...)

#endif

#endif
