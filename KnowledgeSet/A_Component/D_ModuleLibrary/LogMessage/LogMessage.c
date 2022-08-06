#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

void LogMessagePrint1(char *format, ...);
void LogMessagePrint2(char *format, va_list list);

void LogMessagePrint(bool status, char *file, uint32_t line, char *flag, char *format, ...)
{
    va_list  list;
    va_start(list, format);
    
    if (status == true)
    {
        LogMessagePrint1("[%s][%u][%s]", file, line, flag);
        LogMessagePrint2(format, list);
        LogMessagePrint1("\r\n");
    }
    else
    {
        LogMessagePrint1(format, list);
    }
    
    va_end(list);
}

/* 以下内容适配到平台 */
//导入打印接口库
#include <stdio.h>

void LogMessagePrint1(char *format, ...)
{
    va_list  list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
}

void LogMessagePrint2(char *format, va_list list)
{
    vprintf(format, list);
}
