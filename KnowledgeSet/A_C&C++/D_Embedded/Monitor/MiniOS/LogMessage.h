#ifndef LOG_MESSAGE_H
#define LOG_MESSAGE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
/* 全局宏控开关及其等级 */
#define LOG_GLOBAL_STATUS       1   /* 1:ENABLE,0:DISABLE */
/* 全局打印宏控检测,局部打印宏控检测 */
#if (LOG_GLOBAL_STATUS == 1 && LOG_PART_STATUS == 1)
/* 控制打印集合 */
/* 无格式打印:{内容} */
/* 带格式打印:[文件名][行数][级别]{内容}[换行] */
void LogMessagePrint(bool status, char *file, uint32_t line, char *flag, char *format, ...);

#define LOG_DEBUG(status, format, ...)  do { if (LOG_PART_LEVEL <= 0)  LogMessagePrint(status, __FILE__, __LINE__, "D", format, ##__VA_ARGS__); } while (0)
#define LOG_INFO(status, format, ...)   do { if (LOG_PART_LEVEL <= 1)  LogMessagePrint(status, __FILE__, __LINE__, "I", format, ##__VA_ARGS__); } while (0)
#define LOG_WARN(status, format, ...)   do { if (LOG_PART_LEVEL <= 2)  LogMessagePrint(status, __FILE__, __LINE__, "W", format, ##__VA_ARGS__); } while (0)
#define LOG_ERROR(status, format, ...)  do { if (LOG_PART_LEVEL <= 3)  LogMessagePrint(status, __FILE__, __LINE__, "E", format, ##__VA_ARGS__); } while (0)

#else

/* status:1带格式,0不带格式 */
#define LOG_DEBUG(status, format, ...)
#define LOG_INFO(status, format, ...)
#define LOG_WARN(status, format, ...)
#define LOG_ERROR(status, format, ...)

#endif

#endif
