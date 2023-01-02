#ifndef APP_THREAD_ADAPTOR_LINUX_H
#define APP_THREAD_ADAPTOR_LINUX_H

#include "app_os_adaptor.h"
#include "app_thread_master.h"
#include "app_thread_mix_irq.h"
#include "app_thread_mix_custom.h"
#include "app_thread_all.h"

/* @服务例程适配<Start> */

void * app_thread_master_routine_adaptor(void *args)
{
    app_thread_master_routine();
}

void * app_thread_mix_irq_routine_adaptor(void *args)
{
    app_thread_mix_irq_routine();
}

void * app_thread_mix_custom_routine_adaptor(void *args)
{
    app_thread_mix_custom_routine();
}

/* @服务例程适配<End> */

/* @线程体<Start> */

app_thread_t app_thread_master = {
   .priority = 1,
   .routine  = app_thread_master_routine_adaptor,
   .args     = NULL,
};

app_thread_t app_thread_mix_irq = {
   .priority = 2,
   .routine  = app_thread_mix_irq_routine_adaptor,
   .args     = NULL,
};

app_thread_t app_thread_mix_custom = {
   .priority = 3,
   .routine  = app_thread_mix_custom_routine_adaptor,
   .args     = NULL,
};

/* @线程体<End> */

#endif
