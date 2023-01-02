#ifndef APP_THREAD_ALL_H
#define APP_THREAD_ALL_H

/*@brief 线程组编号
 */
typedef enum {
    app_thread_id_mix_irq,
    app_thread_id_mix_custom,
    /* 继续添加其他线程 */
    app_thread_id_unknown,
    app_thread_id_all,
} app_thread_id;

/*@brief 初始化所有线程及其附属资源
 *       启动所有线程
 */
void app_thread_work(void);

#endif
