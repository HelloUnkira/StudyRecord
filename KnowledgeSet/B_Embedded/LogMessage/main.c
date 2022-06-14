/* 文件Main.c */
#define   LOG_PART_STATUS     1   /* 1:ENABLE,0:DISABLE */
#define   LOG_PART_LEVEL      4   /* 0:DEBUG,1:INFO,2:WARN,3:ERROR,4:NONE */
#include "LogMessage.h"

void LogMessageTest1(void);
void LogMessageTest2(void);

int main(int argc, char *argv[]) {
    
    LOG_DEBUG(1, "main");
    LOG_INFO(1,  "main");
    LOG_WARN(1,  "main");
    LOG_ERROR(1, "main");
    
    LogMessageTest2();
    LogMessageTest1();
}
