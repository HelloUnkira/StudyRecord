#include <stdio.h>

#include "MiniOS_Memory.h"
#include "MiniOS_Task.h"
#include "MiniOS_Timer.h"
#include "MiniOSTimerPort.h"

MiniOS_TimerHandle TimerHandle1 = NULL;
MiniOS_TimerHandle TimerHandle2 = NULL;
MiniOS_TaskHandle  TaskHandle1  = NULL;
MiniOS_TaskHandle  TaskHandle2  = NULL;

void TestTimer1Callback(void *Parameter)
{
    printf("TestTimer1Callback\n");
    
    static uint32_t Count = 0;
    MiniOS_TaskToggle(TaskHandle1, Count++);
}

void TestTimer2Callback(void *Parameter)
{
    printf("TestTimer2Callback\n");
    
    static uint32_t Count = 0;
    MiniOS_TaskToggle(TaskHandle2, Count++);
}

void TestTask1Handler(uint32_t Event)
{
    printf("TestTask1Handler Event:%d\n", Event);
}

void TestTask2Handler(uint32_t Event)
{
    printf("TestTask2Handler Event:%d\n", Event);
}

int main(int argc, char *argv[])
{
    MiniOS_MemoryReady();
    MiniOS_TaskReady();
    MiniOS_TimerReady();

    TaskHandle1  = MiniOS_TaskCreate(TestTask1Handler, 2);
    TaskHandle2  = MiniOS_TaskCreate(TestTask2Handler, 1);
    TimerHandle1 = MiniOS_TimerCreate(TestTimer1Callback, NULL, 1000, true);
    TimerHandle2 = MiniOS_TimerCreate(TestTimer2Callback, NULL, 2000, true);
    MiniOS_TimerStart(TimerHandle1);
    MiniOS_TimerStart(TimerHandle2);
    
    /* 硬件定时器的启动放在线程执行的开始,其他任务创建结束 */
    MiniOS_TimerPortConfigure(MiniOS_TimerMSCallback);
    MiniOS_TimerPortInit();
    MiniOS_TaskExecute();
}


