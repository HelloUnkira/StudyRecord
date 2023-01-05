#ifndef APP_THREAD_ADAPTOR_FREERTOS_H
#define APP_THREAD_ADAPTOR_FREERTOS_H

#include "app_thread_master.h"
#include "app_thread_mix_irq.h"
#include "app_thread_mix_custom.h"

/* @线程体栈信息<Start> */

#define APP_THREAD_MASTER_STACK_SIZE  (1024)
static  StackType_t app_thread_master_stack[APP_THREAD_MASTER_STACK_SIZE];

#define APP_THREAD_MIX_IRQ_STACK_SIZE  (1024)
static  StackType_t app_thread_mix_irq_stack[APP_THREAD_MIX_IRQ_STACK_SIZE];

#define APP_THREAD_MIX_CUSTOM_STACK_SIZE  (1024)
static  StackType_t app_thread_mix_custom_stack[APP_THREAD_MIX_CUSTOM_STACK_SIZE];

/* @线程体栈信息<End> */

/* @服务例程适配<Start> */

void * app_thread_master_routine_adaptor(void *pvParameters)
{
    app_thread_master_routine();
}

void * app_thread_mix_irq_routine_adaptor(void *pvParameters)
{
    app_thread_mix_irq_routine();
}

void * app_thread_mix_custom_routine_adaptor(void *pvParameters)
{
    app_thread_mix_custom_routine();
}

/* @服务例程适配<End> */

/* @线程体<Start> */

app_thread_t app_thread_master = {
    .stack      = app_thread_master_stack,
    .stack_size = APP_THREAD_MASTER_STACK_SIZE,
    .priority   = 1,
    .task       = app_thread_master_routine_adaptor,
    .name       = "app_thread_master",
};

app_thread_t app_thread_mix_irq = {
    .stack      = app_thread_mix_irq_stack,
    .stack_size = APP_THREAD_MIX_IRQ_STACK_SIZE,
    .priority   = 2,
    .task       = app_thread_mix_irq_routine_adaptor,
    .name       = "app_thread_mix_irq",
};

app_thread_t app_thread_mix_custom = {
    .stack      = app_thread_mix_custom_stack,
    .stack_size = APP_THREAD_MIX_CUSTOM_STACK_SIZE,
    .priority   = 3,
    .task       = app_thread_mix_custom_routine_adaptor,
    .name       = "app_thread_mix_custom",
};

/* @线程体<End> */

#endif
