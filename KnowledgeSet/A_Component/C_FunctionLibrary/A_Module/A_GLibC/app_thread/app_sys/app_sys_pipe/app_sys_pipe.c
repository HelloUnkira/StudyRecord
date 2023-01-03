/*实现目标:
 *    一个用于线程间数据交互的管道
 *    它需要保证完成数据的基本交互
 *    注意:加入中断临界区的主要原因是防止
 *         中断打断正在线程环境执行的敏感操作及其
 *         中断打断正在中断环境执行的敏感操作
 *         导致错误管道资源包管理错误
 *         我们在设计上极可能减少资源包内容
 *         使得在临界区的时间尽可能缩短
 *         将更多的明细交付到线程环境解析
 */

#include "app_sys_interface.h"

/*@brief        获取管道资源包数量
 *@param[in]    pipe 管道实例
 *@retval       管道资源包数量
 */
uint32_t app_sys_pipe_package_num(app_pipe_t *pipe)
{
    uint32_t number = 0;
    /* 入界 */
    if (app_os_not_in_irq())
        app_mutex_take(&pipe->mutex);
    app_critical_enter();
    /* 资源检查 */
    number = pipe->number;
    /* 出界 */
    app_critical_exit();
    if (app_os_not_in_irq())
        app_mutex_give(&pipe->mutex);
    /* 通报 */
    return number;
}

/*@brief        交付一个包给管道
 *@param[in]    pipe     管道实例
 *@param[in]    package  事件资源包(栈资源,非堆资源或静态资源)
 */
void app_sys_pipe_give(app_pipe_t *pipe, app_package_t *package)
{
    app_package_t *package_new = NULL;
    /* 生成资源包, 转储消息资源资源 */
    package_new = app_mem_alloc(sizeof(app_package_t));
    package_new->near     = NULL;
    package_new->send_tid = package->send_tid;
    package_new->recv_tid = package->recv_tid;
    package_new->module   = package->module;
    package_new->event    = package->event;
    package_new->size     = package->size;
    package_new->data     = package->data;
    /* 入界 */
    if (app_os_not_in_irq())
        app_mutex_take(&pipe->mutex);
    app_critical_enter();
    /* 资源包加入到管道 */
    if (pipe->number == 0) {
        pipe->head = package_new;
        pipe->tail = package_new;
    } else {
        pipe->tail->near = package_new;
        pipe->tail = package_new;
    }
    pipe->number++;
    /* 出界 */
    app_critical_exit();
    if (app_os_not_in_irq())
        app_mutex_give(&pipe->mutex);
}

/*@brief        从管道提取一个包
 *@param[in]    pipe     管道实例
 *@param[out]   package  事件资源包(栈资源,非堆资源或静态资源)
 */
void app_sys_pipe_take(app_pipe_t *pipe, app_package_t *package)
{
    app_package_t *package_new = NULL;
    /* 入界 */
    if (app_os_not_in_irq())
        app_mutex_take(&pipe->mutex);
    app_critical_enter();
    /* 资源包提取出管道 */
    if (pipe->number != 0) {
        pipe->number--;
        package_new = pipe->head;
        pipe->head  = pipe->head->near;
        if (pipe->number == 0) {
            pipe->head = NULL;
            pipe->tail = NULL;
        }
    }
    /* 出界 */
    app_critical_exit();
    if (app_os_not_in_irq())
        app_mutex_give(&pipe->mutex);
    /* 转储消息资源资源, 销毁资源包 */
    package->near     = NULL;
    package->send_tid = package_new->send_tid;
    package->recv_tid = package_new->recv_tid;
    package->module   = package_new->module;
    package->event    = package_new->event;
    package->size     = package_new->size;
    package->data     = package_new->data;
    app_mem_free(package_new);
}
