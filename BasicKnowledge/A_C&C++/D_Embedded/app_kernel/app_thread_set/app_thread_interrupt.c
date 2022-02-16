/*
 *中断线程:
 *    实现目标,也行中断产生时并不仅仅只需要做简单的处理
 *    也许中断需要进行如下事务,比如:控制UI,发送事件,拷贝数据
 *    等等,并且长时的中断响应也并不是很符合设计风格,它应该简短
 *    设计中断线程摘取明细,它将中断产生的某些请求
 *    通过线程中转传导到对应处理程序不失为一种策略
 *    同样,管道兼具着工作者线程的能力,可以向相当部分事宜
 *    迁移到非中断环境下去执行,如果是不必要的去占用中断时间的话
 */

#include "app_threads_all_config.h"
static pipe_set_t *send;
static pipe_set_t *receive;
static semaphone_body_t *semaphone;
static mutex_body_t     *mutex;

void app_thread_interrupt_main(void)
{
    pipe_container_t *container = NULL; //包裹
    //1.获取数据流管道和同步原语
    get_pipe(&send, &receive, thread_interrupt_id);
    get_synchronization_primitive(&semaphone, &mutex, thread_interrupt_id);
    //2.初始化中断所需部分数据

    while (1) {
        semaphone_take(semaphone);
        mutex_take(mutex);
        //处理接收到的包裹
        while (receive->number != 0) {
            app_kernel_pipe_dequeue(receive, &container);
            //处理该包裹
            //销毁该包裹
        }
        mutex_release(mutex);
    }
}

void app_thread_interrupt_send_package(void)
{
    mutex_take(mutex);
    //创建的一个包裹的生命期间必须跨线程!!!!!
    //它必须能坚持到被处理程序处理,所以不断变化的包裹应该
    //使用动态分配的内存空间
    //而变化固定的包裹可以选用静态分配的内存空间
    pipe_container_t *container = NULL; //包裹


    app_kernel_pipe_enqueue(send, &container);
    mutex_release(mutex);
}
