
#include <stdio.h>
#include "app_thread_interface.h"

static void misc_work_routine(void *parameter)
{
    printf("misc_work_routine exec\n");
}

app_thread_mix_custom_work_t misc_work = {
    .routine   = misc_work_routine,
    .parameter = NULL,
};

#if 0
#elif APP_OS_IS_LINUX

#include <signal.h>
#include <sys/time.h>

static void software_timer_handler(int signal)
{
    //SIGALRM:      以系统真实的时间来计算
    //SIGVTALRM:    以该进程在用户态下花费的时间来计算
    //SIGPROF:      以该进程在用户态下和内核态下所费的时间来计算
    static uint32_t count = 0;count++;
    if (signal == SIGALRM && (count % 1000 == 0)) {
        /* package */
        if (count % 1000 == 0) {
            app_package_t package = {
                .send_tid = app_thread_id_unknown,
                .recv_tid = app_thread_id_mix_custom,
                .module   = 1,
                .size     = 0,
                .data     = NULL,
            };
            app_thread_package_notify(&package);
        }
        /* package */
        if (count % 3000 == 0) {
            app_package_t package = {
                .send_tid = app_thread_id_unknown,
                .recv_tid = app_thread_id_mix_custom,
                .module   = app_thread_mix_custom_work,
                .size     = sizeof(app_thread_mix_custom_work_t),
                .data     = &misc_work,
            };
            app_thread_package_notify(&package);
        }
        /* package */
        if (count % 2000 == 0) {
            app_package_t package = {
                .send_tid = app_thread_id_unknown,
                .recv_tid = app_thread_id_mix_irq,
                .module = 1,
                .size   = 0,
                .data   = NULL,
            };
            app_thread_package_notify(&package);
        }
        
        
        /* package... */
    }
}

void software_timer_ready(void)
{
    /* 指定系统:Linux */
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
    signal(SIGALRM, software_timer_handler);
    //设置间隔触发时间
    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_usec = 1000;//1ms
    new_value.it_value.tv_sec = 0;
    new_value.it_value.tv_usec = 1;//不能为0
    //启用定时器
    setitimer(ITIMER_REAL, &new_value, &old_value);
}

#elif APP_OS_IS_ZEPHYR
#error "arch os not adaptor yet"
#else
#error "unknown os"
#endif

int main(int argc, char *argv[])
{
    /* 启动APP调度策略 */
    app_thread_work();
    
    /* 测试中我们在主线程使用软件定时器信号量发送包裹 */
    #if 0
    #elif APP_OS_IS_LINUX
    software_timer_ready();
    #elif APP_OS_IS_ZEPHYR
    #error "arch os not adaptor yet"
    #else
    #error "unknown os"
    #endif
    
    while (true)
        sleep(1);
    
    return 0;
}
