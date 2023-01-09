/*实现目标:
 *    倒计时相关的功能组件
 *注意:
 *    倒计时功能本地有且只有一个实例
 *    因为不需要设定多个倒计时策略
 */

#define APP_OS_LOG_LOCAL_STATUS     1
#define APP_OS_LOG_LOCAL_LEVEL      2   /* 0:DEBUG,1:INFO,2:WARN,3:ERROR,4:NONE */

#include "app_thread_interface.h"

static app_mutex_t app_module_countdown_mutex = {0};
static app_module_countdown_t app_module_countdown = {0};

/*@brief        设置倒计时
 *@param[in]    countdown 倒计时实例
 */
void app_module_countdown_set(app_module_countdown_t *countdown)
{
    app_mutex_take(&app_module_countdown_mutex);
    app_module_countdown = *countdown;
    app_mutex_give(&app_module_countdown_mutex);
}

/*@brief        获取倒计时
 *@param[out]   countdown 倒计时实例
 */
void app_module_countdown_get(app_module_countdown_t *countdown)
{
    app_mutex_take(&app_module_countdown_mutex);
    *countdown = app_module_countdown;
    app_mutex_give(&app_module_countdown_mutex);
}

/*@brief 复位倒计时
 */
void app_module_countdown_reset(void)
{
    app_module_countdown_t countdown = {0};
    app_mutex_take(&app_module_countdown_mutex);
    app_module_countdown = countdown;
    app_mutex_give(&app_module_countdown_mutex);
}

/*@brief 启动倒计时
 */
void app_module_countdown_start(void)
{
    /* 在这里调用一个接口,启动时钟源发送秒表事件功能,如果有必要 */
    /* ...... */
    app_mutex_take(&app_module_countdown_mutex);
    app_module_countdown.onoff = true;
    app_mutex_give(&app_module_countdown_mutex);
}

/*@brief 停止倒计时
 */
void app_module_countdown_stop(void)
{
    /* 在这里调用一个接口,关闭时钟源发送秒表事件功能,如果有必要 */
    /* ...... */
    app_mutex_take(&app_module_countdown_mutex);
    app_module_countdown.onoff = false;
    app_mutex_give(&app_module_countdown_mutex);
}

/*@brief 倒计时模组初始化
 */
void app_module_countdown_ready(void)
{
    app_mutex_process(&app_module_countdown_mutex);
}

/*@brief 系统时钟转储到外存
 */
void app_module_countdown_dump(void)
{
    union {
        uint8_t buffer[0];
        struct {
            app_module_countdown_t countdown;
            uint32_t checksum32;
            uint32_t crc32;
        };
    } countdown_data = {};
    
    app_module_countdown_get(&countdown_data.countdown);
    countdown_data.crc32 = app_sys_crc32(countdown_data.buffer, sizeof(app_module_countdown_t));
    countdown_data.checksum32 = app_sys_checksum32(countdown_data.buffer, sizeof(app_module_countdown_t));
    app_module_source_write("mix_thread", "system countdown", countdown_data.buffer, sizeof(countdown_data));
}

/*@brief 系统时钟加载到内存
 */
void app_module_countdown_load(void)
{
    union {
        uint8_t buffer[0];
        struct {
            app_module_countdown_t countdown;
            uint32_t checksum32;
            uint32_t crc32;
        };
    } countdown_data = {};
    
    app_module_source_read("mix_thread", "system countdown", countdown_data.buffer, sizeof(countdown_data));
    uint32_t checksum32 = app_sys_checksum32(countdown_data.buffer, sizeof(app_module_countdown_t));
    uint32_t crc32 = app_sys_crc32(countdown_data.buffer, sizeof(app_module_countdown_t));
    if (checksum32 == countdown_data.checksum32 && crc32 == countdown_data.crc32)
        app_module_countdown_set(&countdown_data.countdown);
}

/*@brief 更新倒计时
 *       内部使用: 被mix custom线程使用
 */
void app_module_countdown_msec_update(void)
{
    app_mutex_take(&app_module_countdown_mutex);
    app_module_countdown_t countdown = app_module_countdown;
    app_mutex_give(&app_module_countdown_mutex);
    if (!countdown.onoff)
        return;
    /*  */
    uint32_t seconds = countdown.hour   * 60 * 60 +
                       countdown.minute * 60 +
                       countdown.second;
    /*  */
    if (countdown.msec >= APP_MODULE_STOPWATCH_MSEC) {
        countdown.msec -= APP_MODULE_STOPWATCH_MSEC;
    } else {
        if (seconds == 0) {
            /* 发送倒计时事件 */
            app_package_t package = {
                .send_tid = app_thread_id_unknown,
                .recv_tid = app_thread_id_mix_custom,
                .module   = app_thread_mix_custom_countdown,
                .event    = app_thread_mix_custom_countdown_expired,
                .dynamic  = false,
                .size     = 0,
                .data     = NULL,
            };
            app_thread_package_notify(&package);
            /* 本地归零 */
            app_module_countdown_t countdown_0 = {0};
            countdown = countdown_0;
        }
        if (seconds != 0) {
            seconds--;
            countdown.msec += 1000 - APP_MODULE_STOPWATCH_MSEC;
        }
    }
    /*  */
    countdown.second = seconds % 60;
    countdown.minute = seconds / 60 % 60;
    countdown.hour   = seconds / 60 / 60 % 60;
    app_mutex_take(&app_module_countdown_mutex);
    app_module_countdown = countdown;
    app_mutex_give(&app_module_countdown_mutex);
    
    #if APP_MODULE_CHECK
    APP_OS_LOG_INFO("countdown:%u:%u:%u:%u - %u\n",
                    countdown.hour,countdown.minute,countdown.second,
                    countdown.msec,countdown.onoff);
    #endif
}
