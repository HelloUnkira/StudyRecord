
/* 核心唯一依赖项 */
#include "SGuiPort.h"

#if SGUI_USE_LINUX
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

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

//全局化唯一1ms定时器实例
static SGuiTimerMSHandler ms_handler = NULL;

void SGuiTimerCfg(SGuiTimerMSHandler handler)
{
    //设置回调
    ms_handler = handler;
}

static void SGuiTimerSignalHandler(int signal)
{
    switch (signal) {
    //以系统真实的时间来计算
    case SIGALRM:
        if (ms_handler != NULL)
            ms_handler();
        break;
    //以该进程在用户态下花费的时间来计算
    case SIGVTALRM:
        break;
    //以该进程在用户态下和内核态下所费的时间来计算
    case SIGPROF:
        break;
    }
}

void SGuiTimerInit(void)
{
    struct itimerval old_value = {0};
    struct itimerval new_value = {0};
    //将信号关联指定的回调
    signal(SIGALRM, SGuiTimerSignalHandler);
    //设置间隔触发时间
    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_usec = 1000;//1ms
    new_value.it_value.tv_sec = 0;
    new_value.it_value.tv_usec = 1;//不能为0
    //启用定时器
    setitimer(ITIMER_REAL, &new_value, &old_value);
}

/* 继续添加其余平台
#elif SGUI_USE_UNKNOWN
*/
#else

#endif
