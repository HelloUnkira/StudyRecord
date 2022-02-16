/*
 *实现目标:
 *    获得所有受管控线程传递的管道信息
 *    将管道中的全部信息摘出,选择性的转交给对应的线程
 *灾难的产生:
 *    明细信息的发源地与去处是一件过于繁杂的事宜
 *    控制线程最为直接的目的是要实现中心化管理
 *    这意味着它需要得到所有明细才能精准无误的
 *    对数据进行合理的分发,构建一个体系化的容器去
 *    保存这些数据容器是一项有挑战性的工作
 *    在实际情况下我们反而发现
 *    最基本的容器居然更能高效的完成指定的工作
 */

#include "app_threads_all_config.h"

static unsigned int current_number = 0;//当前被使用者数量
//建立一个管道集合,并确认为每一个线程分发一套传输管道
static pipe_set_t pipe_set_from[APP_THREAD_MAX_LENGTH] = {0};
static pipe_set_t pipe_set_to[APP_THREAD_MAX_LENGTH]   = {0};
//管理者需要管理的线程集合所使用的同步原语:
static semaphone_body_t semaphone_owner;
static semaphone_body_t semaphone_set[APP_THREAD_MAX_LENGTH];
static mutex_body_t     mutex_set[APP_THREAD_MAX_LENGTH];

void set_user_when_controller_init(unsigned int number)
{
    current_number = number;
}

void get_pipe(pipe_set_t **send,
              pipe_set_t **receive,
              unsigned int thread_id)
{
    if (send == NULL || receive == NULL)
        return;
    if (thread_id > current_number)
        return;
    *send    = &(pipe_set_from[thread_id]);
    *receive = &(pipe_set_to[thread_id]);
}

void get_synchronization_primitive(semaphone_body_t **semaphone,
                                   mutex_body_t **mutex,
                                   unsigned int thread_id)
{
    if (mutex == NULL || semaphone == NULL)
        return;
    if (thread_id > current_number)
        return;
    *semaphone = &(semaphone_set[thread_id]);
    *mutex     = &(mutex_set[thread_id]);
}

void create_synchronization_primitives(void)
{
    unsigned int i = 0;
    semaphone_create(&semaphone_owner);
    for (i = 0; i < current_number; i++) {
        semaphone_create(&(semaphone_set[i]));
        mutex_create(&(mutex_set[i]));
    }
}

void app_thread_controller_main(void)
{
    unsigned int i = 0, index = (~0);
    pipe_container_t *container = NULL; //包裹
    while (1) {
        //产生了包裹需要派送时
        semaphone_take(&semaphone_owner);
        //解析所有通道
        for (i = 0; i < current_number; i++) {
            //抢占互斥锁
            mutex_take(&(mutex_set[i]));
            //检查是否存在包裹
            while (pipe_set_from[i].number != 0) {
                //获取第一个包裹
                app_kernel_pipe_dequeue(&(pipe_set_from[i]), &container);
                index = container->receiver;
                if (index < current_number) {
                    //将包裹派送到指定线程
                    app_kernel_pipe_enqueue(&(pipe_set_to[index]), container);
                    //为指定线程发送信号量表明产生了数据
                    semaphone_give(&(semaphone_set[index]));
                } else {
                    //防止动态分配造成的内存泄漏,中止派发
                    app_kernel_pipe_enqueue(&(pipe_set_from[index]), container);
                    continue;
                }
            }
            //释放互斥锁
            mutex_release(&(mutex_set[i]));
        }
    }
}

void notify_controller_receive_package(void)
{
    semaphone_give(&semaphone_owner);
}
