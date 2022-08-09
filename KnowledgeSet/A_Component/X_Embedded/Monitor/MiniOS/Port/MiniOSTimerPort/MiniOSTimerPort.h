#ifndef MINI_OS_TIMER_PORT_H
#define MINI_OS_TIMER_PORT_H
//C std lib
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MINIOS_TIMER_USE_LINUX
#define MINIOS_TIMER_USE_UNKONWN
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#ifdef MINIOS_TIMER_USE_LINUX
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TimerPortConfigure(void (*Handler)(void));
void MiniOS_TimerPortInit(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 继续添加其余平台 */
//#ifdef MINIOS_TIMER_USE_UNKONWN
//#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif