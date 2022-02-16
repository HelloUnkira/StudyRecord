#ifndef APP_SYSTEM_PIPE_H
#define APP_SYSTEM_PIPE_H

typedef struct app_system_pipe_container {
    struct app_system_pipe_container *next;
    //使用主副键值对保证,在线程中通信时保证数据的精确传导
    unsigned char receiver;    //接收者线程
    unsigned char primary_key; //主键值对
    unsigned char vice_key;    //副键值对
    unsigned char state;       //包裹的状态,静态分配0,动态分配1
    void * data_body;          //动态创建的数据体
                               //选用动态体的决定是慎重的!
} pipe_container_t;

typedef struct app_system_pipe_set {
    //管道集合保证其数据的关联性,项是对它进行的动作
    struct app_system_pipe_container *head; //管道头(出队列使用)
    struct app_system_pipe_container *tail; //管道尾(入队列使用)
    unsigned int number;                    //管道项数
} pipe_set_t;

//将一个项加入到管道
void app_kernel_pipe_enqueue(pipe_set_t *set, pipe_container_t *container);
//将一个项移除出管道
void app_kernel_pipe_dequeue(pipe_set_t *set, pipe_container_t **container);

//同样的以最简化的方式只获取其需要用到的部分
//而不是写出全部的实现,且以线程不安全去换取其最简化的实现
//所以线程间通信时它必须要得到同步原语的保护

#endif
