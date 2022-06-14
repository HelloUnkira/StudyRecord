/* 文件Test2.c */
#define   LOG_PART_STATUS     1   /* 1:ENABLE,0:DISABLE */
#define   LOG_PART_LEVEL      2   /* 0:DEBUG,1:INFO,2:WARN,3:ERROR,4:NONE */
#include "LogMessage.h"

void LogMessageTest2(void)
{
    LOG_DEBUG(0, "LogMessageTest2\r\n");
    LOG_INFO(0,  "LogMessageTest2\r\n");
    LOG_WARN(0,  "LogMessageTest2\r\n");
    LOG_ERROR(0, "LogMessageTest2\r\n");
    
    LOG_DEBUG(1, "LogMessageTest2");
    LOG_INFO(1,  "LogMessageTest2");
    LOG_WARN(1,  "LogMessageTest2");
    LOG_ERROR(1, "LogMessageTest2");
}
