/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <ucontext.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "coroutine.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define COROUTINE_READY     0
#define COROUTINE_WAITE     1
#define COROUTINE_BLOCK     2
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct task {
    void      *next;
    void     (*exec)(void *target, void *args);
    void      *args;
    void      *owner;
    uint8_t   *stack;
    uint32_t   stat;
    ptrdiff_t  size;
    ptrdiff_t  capa;
    ucontext_t ucon;
} task_t;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct coroutine {
    void     (*idle)(void);
    uint32_t   icnt;
    uint8_t   *stack;
    uint32_t   size;
    task_t    *head;
    task_t    *tail;
    uint32_t   tnum;
    ucontext_t ucon;
} coroutine_t;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void task_exec(void *target)
{
    task_t *task = target;
    task->exec(task, task->args);
    free(task->stack);
    free(task);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * coroutine_ready(uint32_t size, void (*idle)(void), uint32_t icnt)
{
    coroutine_t *coroutine = malloc(sizeof(coroutine_t));
    coroutine->stack = malloc(sizeof(uint8_t) * size);
    coroutine->size = size;
    coroutine->idle = idle;
    coroutine->icnt = icnt;
    coroutine->head = NULL;
    coroutine->tail = NULL;
    coroutine->tnum = 0;
    return coroutine;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * coroutine_commit(void *instance, void (*exec)(void *target, void *args), void *args)
{
    /* Step1:创建该任务节点 */
    task_t *task = malloc(sizeof(task_t));
    task->next  = NULL;
    task->exec  = exec;
    task->args  = args;
    task->owner = instance;
    task->stack = NULL;
    task->capa  = 0;
    task->size  = 0;
    task->stat  = COROUTINE_READY;
    coroutine_t *coroutine = instance;
    /* Step2:将任务节点加入到队列尾,返回任务节点 */
    COROUTINE_CRITICAL_ENTER();
    if (coroutine->tail == NULL)
        coroutine->head = task;
    if (coroutine->tail != NULL)
        coroutine->tail->next = task;
    coroutine->tail = task;
    coroutine->tnum++;
    COROUTINE_CRITICAL_EXIT();
    return task;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void coroutine_schedule(void *instance, uint8_t lock)
{
    task_t *task = NULL;
    coroutine_t *coroutine = instance;
    /* 开始主循环 */
    do {
        static uint32_t cnt = 0;
        if (coroutine->tnum == 0) {
            /* 当CPU可能空闲滴答一定时间时,可以考虑低功耗了 */
            if (cnt++ > coroutine->icnt) {
                if (coroutine->idle != NULL)
                    coroutine->idle();
                cnt = 0;
            }
            continue;
        }
        task = NULL;
        /* 操作任务队列时,需要进入临界区 */
        COROUTINE_CRITICAL_ENTER();
        /* 迭代任务节点列表,寻找一个目标为:COROUTINE_READY */
        if (coroutine->head->stat == COROUTINE_READY) {
            task = coroutine->head;
            coroutine->head = task->next;
            if (coroutine->head == NULL)
                coroutine->tail = NULL;
            coroutine->tnum--;
        } else {
            task_t *current = NULL;
            for (current = coroutine->head; current->next != NULL; current = current->next)
                if (((task_t *)current->next)->stat == COROUTINE_READY) {
                    task = current->next;
                    current->next = task->next;
                    coroutine->tnum--;
                }
        }
        /* 操作任务队列时,需要退出临界区 */
        COROUTINE_CRITICAL_ENTER();
        /* 抓取到状态为:COROUTINE_READY的节点 */
        if (task != NULL) {
            getcontext(&task->ucon);
            task->ucon.uc_stack.ss_sp = coroutine->stack;
            task->ucon.uc_stack.ss_size = coroutine->size;
            task->ucon.uc_link = &coroutine->ucon;
            makecontext(&task->ucon, (void (*)(void))task_exec, 1, task);
            swapcontext(&coroutine->ucon, &task->ucon);
            continue;
        }
        task = NULL;
        /* 操作任务队列时,需要进入临界区 */
        COROUTINE_CRITICAL_ENTER();
        /* 迭代任务节点列表,寻找一个目标为:COROUTINE_WAITE */
        if (coroutine->head->stat == COROUTINE_WAITE) {
            task = coroutine->head;
            coroutine->head = task->next;
            if (coroutine->head == NULL)
                coroutine->tail = NULL;
            coroutine->tnum--;
        } else {
            task_t *current = NULL;
            for (current = coroutine->head; current->next != NULL; current = current->next)
                if (((task_t *)current->next)->stat == COROUTINE_WAITE) {
                    task = current->next;
                    current->next = task->next;
                    coroutine->tnum--;
                }
        }
        /* 操作任务队列时,需要退出临界区 */
        COROUTINE_CRITICAL_ENTER();
        /* 抓取到状态为:COROUTINE_WAITE的节点 */
        if (task != NULL) {
            memcpy(coroutine->stack + coroutine->size - task->size, task->stack, task->size);
            swapcontext(&coroutine->ucon, &task->ucon);
            continue;
        }
    } while (lock != 0);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void coroutine_yieldtask(void *target)
{
    task_t *task = target;
    coroutine_t *coroutine = task->owner;
    if (task->stat == COROUTINE_BLOCK)
        return;
    task->stat = COROUTINE_BLOCK;
    task->next = NULL;
    /* 将任务节点加入到队列尾,返回任务节点 */
    COROUTINE_CRITICAL_ENTER();
    if (coroutine->tail == NULL)
        coroutine->head = task;
    if (coroutine->tail != NULL)
        coroutine->tail->next = task;
    coroutine->tail = task;
    coroutine->tnum++;
    COROUTINE_CRITICAL_EXIT();
    /* 保存本地栈信息 */
    uint8_t dummy = 0;  //声明一个占位符,目的是获取此处地址
    if (task->capa < coroutine->stack + coroutine->size - &dummy) {
        task->capa = coroutine->stack + coroutine->size - &dummy;
        task->stack = realloc(task->stack, sizeof(uint8_t) * task->capa);
    }
    task->size = coroutine->stack + coroutine->size - &dummy;
    memcpy(task->stack, &dummy, task->size);
    /* 上下文切换 */
    swapcontext(&task->ucon, &coroutine->ucon);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void coroutine_waketask(void *target)
{
    task_t *task = target;
    coroutine_t *coroutine = task->owner;
    task_t *current = NULL;
    for (current = coroutine->head; current != NULL; current = current->next)
        if (current == task)
            if (task->stat == COROUTINE_BLOCK) {
                task->stat  = COROUTINE_WAITE;
                break;
            }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
