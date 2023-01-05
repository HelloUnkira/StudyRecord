
#include <stdio.h>
#include "app_thread_interface.h"

/* 闹钟组测试开始 */
void app_module_alarm_test(void)
{

}



/* 闹钟组测试结束 */



static uint8_t app_thread_mix_work_arg = 0;
static void app_thread_mix_work_routine(void *parameter)
{
    (*(uint8_t *)parameter)++;
    printf("misc_work_routine exec:%d\n", *(uint8_t *)parameter);
}

app_thread_mix_custom_work_t app_thread_mix_custom_work_entry = {
    .routine   =  app_thread_mix_work_routine,
    .parameter = &app_thread_mix_work_arg,
};
app_thread_mix_irq_work_t app_thread_mix_irq_work_entry = {
    .routine   =  app_thread_mix_work_routine,
    .parameter = &app_thread_mix_work_arg,
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
    if (signal == SIGALRM) {
        /* test clock */
        if (count % 1000 == 0) {
            app_package_t package = {
                .send_tid = app_thread_id_unknown,
                .recv_tid = app_thread_id_mix_custom,
                .module   = app_thread_mix_custom_clock,
                .event    = app_thread_mix_custom_clock_timestamp_update,
                .dynamic  = false,
                .size     = 0,
                .data     = NULL,
            };
            app_thread_package_notify(&package);
        }
        /* test alarm group */
        if (count == 1000) {
            static app_module_alarm_t array[5] = {0};
            uint32_t alarm_group_id = app_module_alarm_group_register(array, 5);
            app_module_alarm_t alarm1 = {.clock_base.utc = 2, .onoff = 1, .snooze = 1,
                                         .field_month = 0b00000000100, .field_week = 0b1000000,};
            app_module_alarm_t alarm2 = {.clock_base.utc = 10, .onoff = 1, .snooze = 1,
                                         .field_month = 0b00000000100, .field_week = 0b1000000,};
            app_module_clock_to_dtime(&alarm1.clock_base);
            app_module_clock_to_dtime(&alarm2.clock_base);
            app_module_clock_to_week(&alarm1.clock_base);
            app_module_clock_to_week(&alarm2.clock_base);

            app_module_alarm_add(alarm_group_id, &alarm1);
            app_module_alarm_add(alarm_group_id, &alarm2);
        }
        /* test stopwatch */
        if (count % APP_MODULE_STOPWATCH_MSEC == 0) {
            app_package_t package = {
                .send_tid = app_thread_id_unknown,
                .recv_tid = app_thread_id_mix_custom,
                .module   = app_thread_mix_custom_stopwatch,
                .event    = app_thread_mix_custom_stopwatch_msec_update,
                .dynamic  = false,
                .size     = 0,
                .data     = NULL,
            };
            app_thread_package_notify(&package);
        }
        if (count == 1000) {
            app_module_stopwatch_reset();
            app_module_stopwatch_start();
        }
        /* test countdown */
        if (count % APP_MODULE_STOPWATCH_MSEC == 0) {
            app_package_t package = {
                .send_tid = app_thread_id_unknown,
                .recv_tid = app_thread_id_mix_custom,
                .module   = app_thread_mix_custom_countdown,
                .event    = app_thread_mix_custom_countdown_msec_update,
                .dynamic  = false,
                .size     = 0,
                .data     = NULL,
            };
            app_thread_package_notify(&package);
        }
        if (count == 1000) {
            app_module_countdown_reset();
            app_module_countdown_t countdown = {.hour = 0, .minute = 0, .second = 17};
            app_module_countdown_set(&countdown);
            app_module_countdown_start();
        }
        /* package... */
        #if 0
        if (count % 1000 == 0) {
            app_package_t package = {
                .send_tid = app_thread_id_unknown,
                .recv_tid = app_thread_id_mix_custom,
                .module   = app_thread_mix_custom_work,
                .event    = 0,
                .dynamic  = false,
                .size     = sizeof(app_thread_mix_custom_work_t),
                .data     = &app_thread_mix_custom_work_entry,
            };
            app_thread_package_notify(&package);
        }
        if (count % 1000 == 0) {
            app_package_t package = {
                .send_tid = app_thread_id_unknown,
                .recv_tid = app_thread_id_mix_custom,
                .module   = app_thread_mix_irq_work,
                .event    = 0,
                .dynamic  = false,
                .size     = sizeof(app_thread_mix_irq_work_t),
                .data     = &app_thread_mix_irq_work_entry,
            };
            app_thread_package_notify(&package);
        }
        #endif
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
#elif APP_OS_IS_FREERTOS
#error "arch os not adaptor yet"
#else
#error "unknown os"
#endif

int main(int argc, char *argv[])
{
    /* 启动APP调度策略 */
    app_thread_set_work_now();
    
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