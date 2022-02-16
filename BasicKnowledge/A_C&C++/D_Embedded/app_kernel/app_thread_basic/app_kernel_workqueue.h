#ifndef APP_SYSTEM_WORKQUEUE_H
#define APP_SYSTEM_WORKQUEUE_H

#ifndef NULL
#define NULL 0
#endif

typedef struct app_system_workqueue_container {
    struct app_system_workqueue_container *next;
    void * (*handler)(void *data_body); //利用空类型指针完成通配
    void *data_body;                    //利用空类型指针导入非定向数据
} workqueue_container_t;

typedef struct app_system_workqueue_set {
    struct app_system_workqueue_container *head;
    struct app_system_workqueue_container *tail;
    unsigned int number;
} workqueue_set_t;

//创建一个工作队列
#define APP_SYSTEM_WORKQUEUE_DEFINE(name)    \
    workqueue_set_t name = { .head   = NULL, \
                             .tail   = NULL, \
                             .number = 0,    \
                           }

//收集工作者到工作队列
void app_system_workqueue_collection(workqueue_set_t *set, workqueue_container_t *container);
//从工作队列释放一个工作者并督促其完成工作
void app_system_workqueue_dispatch(workqueue_set_t *set, workqueue_container_t **container);

#endif
