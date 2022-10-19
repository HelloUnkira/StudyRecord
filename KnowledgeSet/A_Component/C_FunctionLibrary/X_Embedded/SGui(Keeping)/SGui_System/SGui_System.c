#include "SGui_System.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 指定系统:Linux */
#if 0
#elif SGUI_USE_LINUX
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
/* 指定系统:unknown */
/* 继续添加其他平台 */
/* ... */
#else
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_LogMessage(char *File, uint32_t Line, char *Format, ...)
{
#if SGUI_USE_LOG_MESSAGE
#if 0
/* 指定系统:Linux */
#elif SGUI_USE_LINUX
    printf("[%s]", File);
    printf("[%u]", Line);
    va_list List;
    va_start(List, Format);
    vprintf(Format, List);
    va_end(List);
    printf("\n");
/* 指定系统:unknown */
/* 继续添加其他平台 */
/* ... */
#else
#endif
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * SGui_Alloc(uint32_t Size, int8_t *File, uint32_t Line)
{
#if 0
/* 指定系统:Linux */
#elif SGUI_USE_LINUX
    void * Pointer = malloc(Size);
/* 指定系统:unknown */
/* 继续添加其他平台 */
/* ... */
#else
    void * Pointer = NULL;
#endif
    
#if SGUI_USE_MEMORY_CHECK
    SGUI_LOGMESSAGE("SGui_Alloc: \n[%s][%u][%p][%u]\n", File, Line, Pointer, Size);
#endif
    
    return Pointer;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_Free(void *Pointer, int8_t *File, uint32_t Line)
{
#if SGUI_USE_MEMORY_CHECK
    SGUI_LOGMESSAGE("SGui_Free: \n[%s][%u][%p]\n", File, Line, Pointer);
#endif

#if 0
/* 指定系统:Linux */
#elif SGUI_USE_LINUX
    free(Pointer);
/* 指定系统:unknown */
/* 继续添加其他平台 */
/* ... */
#else
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 全局化1ms定时器实例单例 */
static void (*SGui_TimerMSHandler)(void) = NULL;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_TimerCallBackRegister(void (*Handler)(void))
{
    SGui_TimerMSHandler = Handler;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if 0
/* 指定系统:Linux */
#elif SGUI_USE_LINUX
static void SGui_TimerSignalHandler(int signal)
{
    //SIGALRM:      以系统真实的时间来计算
    //SIGVTALRM:    以该进程在用户态下花费的时间来计算
    //SIGPROF:      以该进程在用户态下和内核态下所费的时间来计算
    if (signal == SIGALRM &&
        SGui_TimerMSHandler != NULL)
        SGui_TimerMSHandler();
}
/* 指定系统:unknown */
/* 继续添加其他平台 */
/* ... */
#else
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_TimerInit(void)
{
#if 0
/* 指定系统:Linux */
#elif SGUI_USE_LINUX
    // note:
    // struct itimerval 
    // {
    //      struct timeval it_interval; //计时间隔
    //      struct timeval it_value;    //延时时间(信号延时触发)
    // };
    // struct timeval 
    // {
    //      time_t      tv_sec;         //秒
    //      suseconds_t tv_usec;        //微妙, 1/1'000'000秒
    // };
    // 函数原型：
    // int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
    // which:
    //       ITIMER_REAL(它送出SIGALRM信号):         以系统真实的时间来计算
    //       ITIMER_VIRTUAL(它送出SIGVTALRM信号):    以该进程在用户态下花费的时间来计算
    //       ITIMER_PROF(它送出SIGPROF信号):         以该进程在用户态下和内核态下所费的时间来计算
    // old_value：通常用不上,设置为NULL,存储上一次触发记录
    // new_value: it_interval为计时间隔,it_value为延时时长
    // it_value为0是不会触发信号的,所以要能触发信号,it_value得大于0
    // it_interval为零,只会延时,不会定时
    struct itimerval old_value = {0};
    struct itimerval new_value = {0};
    //将信号关联指定的回调
    signal(SIGALRM, SGui_TimerSignalHandler);
    //设置间隔触发时间
    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_usec = 1000;//1ms
    new_value.it_value.tv_sec = 0;
    new_value.it_value.tv_usec = 1;//不能为0
    //启用定时器
    setitimer(ITIMER_REAL, &new_value, &old_value);
/* 指定系统:unknown */
/* 继续添加其他平台 */
/* ... */
#else
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_TimerDeinit(void)
{
#if 0
/* 指定系统:Linux */
#elif SGUI_USE_LINUX
/* 指定系统:unknown */
/* 继续添加其他平台 */
/* ... */
#else
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通配辅助函数:拷贝函数 */
void SGui_Copy(void *Stream1, void *Stream2, uint32_t Length)
{
#if 0
    // memcpy(Stream1, Stream2, Length);
#else
    
#define SGui_Copy_Unit(Type) {                      \
        *((Type *)Stream1) = *((Type *)Stream2);    \
        Stream1 += sizeof(Type);                    \
        Stream2 += sizeof(Type);                    \
        Length  -= sizeof(Type);                    \
}

#define SGui_Copy_Type(Type) {                      \
    while (Length >= sizeof(Type)) {                \
        *((Type *)Stream1) = *((Type *)Stream2);    \
        Stream1 += sizeof(Type);                    \
        Stream2 += sizeof(Type);                    \
        Length  -= sizeof(Type);                    \
    }                                               \
}

    /* 至少保证一个流是平台字节对齐 */
    uint8_t Alignment1 = (uint64_t)(Stream1) % sizeof(void *);
    uint8_t Alignment2 = (uint64_t)(Stream2) % sizeof(void *);
    
    while (Alignment1 != 0 && Alignment2 != 0) {
        Alignment1--;
        Alignment2--;
        SGui_Copy_Unit(uint8_t);
    }

    SGui_Copy_Type(uint64_t);
    SGui_Copy_Type(uint32_t);
    SGui_Copy_Type(uint16_t);
    SGui_Copy_Type(uint8_t);
    
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通配辅助函数:交换函数 */
void SGui_Swap(void *Stream1, void *Stream2, uint32_t Length)
{
#if 0
    //
#else
    
#define SGui_Swap_Unit(Type) {                  \
        Type Temp_1 = (*((Type *)Stream1));     \
        Type Temp_2 = (*((Type *)Stream2));     \
        *((Type *)Stream1) = Temp_2;            \
        *((Type *)Stream2) = Temp_1;            \
        Stream1 += sizeof(Type);                \
        Stream2 += sizeof(Type);                \
        Length  -= sizeof(Type);                \
}

#define SGui_Swap_Type(Type) {                  \
    while (Length >= sizeof(Type)) {            \
        Type Temp_1 = (*((Type *)Stream1));     \
        Type Temp_2 = (*((Type *)Stream2));     \
        *((Type *)Stream1) = Temp_2;            \
        *((Type *)Stream2) = Temp_1;            \
        Stream1 += sizeof(Type);                \
        Stream2 += sizeof(Type);                \
        Length  -= sizeof(Type);                \
    }                                           \
}

    /* 至少保证一个流是平台字节对齐 */
    uint8_t Alignment1 = (uint64_t)(Stream1) % sizeof(void *);
    uint8_t Alignment2 = (uint64_t)(Stream2) % sizeof(void *);
    
    while (Alignment1 != 0 && Alignment2 != 0) {
        Alignment1--;
        Alignment2--;
        SGui_Swap_Unit(uint8_t);
    }
    
    SGui_Swap_Type(uint64_t);
    SGui_Swap_Type(uint32_t);
    SGui_Swap_Type(uint16_t);
    SGui_Swap_Type(uint8_t);
    
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/