/*
 *实现目标:
 *    以数据结构容器设计思想为基础
 *    设计一个用于线程间数据交互的管道
 *    它需要保证完成数据的基本交互
 *    融洽接受各种数据类型且保证兼容不定长度的数据集合
 */
#define  NULL 0
#include "app_kernel_pipe.h"

//将一个项加入到管道
void app_kernel_pipe_enqueue(pipe_set_t *set, pipe_container_t *container)
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

//将一个项移除出管道
void app_kernel_pipe_dequeue(pipe_set_t *set, pipe_container_t **container)
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
    }
}
