#ifndef MINI_OS_TIMER_H
#define MINI_OS_TIMER_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MiniOS_TimerHandle      void *
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TimerStop(MiniOS_TimerHandle Handle);
bool MiniOS_TimerStart(MiniOS_TimerHandle Handle);
void MiniOS_TimerDestroy(MiniOS_TimerHandle Handle);
MiniOS_TimerHandle MiniOS_TimerCreate(void   (*Callback)(void *Parameter),
                                      void    *Parameter,
                                      uint32_t Peroid,
                                      bool     Reload);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
