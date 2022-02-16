#ifndef ALL_THREADS_ALL_CONFIG_H
#define ALL_THREADS_ALL_CONFIG_H

//注意此处的头文件包含
//app_system_pipe.h     :独立实现,管道
//app_kernel_package.h  :操作系统封装
//它们都是仅仅为本层提供的服务项,所以可以直接包含至此处
#include "app_kernel_pipe.h"
#include "app_kernel_package.h"
//本层是用于各个线程间兼容所存在
//用于将各个线程间以一个模板化的方式编修内部实现

//线程最大数量,它可以刚好是出去管理者线程以外其余线程之和
#define APP_THREAD_MAX_LENGTH 5

//从控制者获取自己的数据流管道
void get_pipe(pipe_set_t **send,
              pipe_set_t **receive,
              unsigned int thread_id);

void get_synchronization_primitive(semaphone_body_t **semaphone,
                                   mutex_body_t **mutex,
                                   unsigned int thread_id);

//通知控制者处理发送的包裹
void notify_controller_receive_package(void);

//支持静态裁剪的线程定制服务
#define THREAD_INTERRUPT



#ifdef  THREAD_INTERRUPT
extern unsigned int thread_interrupt_id;
#endif

#endif
