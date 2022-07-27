#ifndef COROUTINE_H
#define COROUTINE_H
// C std lib
#include <stdint.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define COROUTINE_CRITICAL_ENTER(...)
#define COROUTINE_CRITICAL_EXIT(...)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * coroutine_ready(uint32_t size, void (*idle)(void), uint32_t icnt);
void * coroutine_commit(void *instance, void (*exec)(void *target, void *args), void *args);
void coroutine_schedule(void *instance, uint8_t lock);
void coroutine_yieldtask(void *target);
void coroutine_waketask(void *target);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
