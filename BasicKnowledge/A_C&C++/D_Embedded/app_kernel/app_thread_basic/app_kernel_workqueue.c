/*
 *实现目标:
 *    为了使得事件集处理集成化,设计工作队列
 *    工作队列会当获得对应的执行权限时
 *    释放里面的工作者并完成工作者所需完成的工作
 *    这是dispatch的原语实现方式
 */

#include "app_system_workqueue.h"

//收集工作者到工作队列
void app_system_workqueue_collection(workqueue_set_t *set, workqueue_container_t *container)
{
    if (set == NULL || container == NULL)
        return;
    if (set->number == 0) {
        set->head = container;
        set->tail = container;
    } else {
        set->tail->next = container;
        set->tail = container;
    }
    set->number += 1;
}

//从工作队列释放一个工作者并督促其完成工作
void app_system_workqueue_dispatch(workqueue_set_t *set, workqueue_container_t **container)
{
    if (set == NULL || container == NULL)
        return;
    *container = NULL;
    if (set->number != 0) {
        *container = set->head;
        set->head = set->head->next;
        set->number -= 1;
        if (set->number == 0) {
            set->head = NULL;
            set->tail = NULL;
        }
        //完成工作者的工作
        if ((*container)->handler != NULL)
            (*container)->handler((*container)->data_body);
    }
}
