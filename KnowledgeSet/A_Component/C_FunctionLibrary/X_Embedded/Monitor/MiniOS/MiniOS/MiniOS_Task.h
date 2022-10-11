#ifndef MINI_OS_TASK_H
#define MINI_OS_TASK_H
//C std lib
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 这是非公平的分时OS,其中许多组件是拼凑出来的,未重新实现 */
/* 因为它并不具备现场保护与还原,所以线程的实现方式也有点特别,平台依赖度(一个RTC) */
/* 同样的,它以任务体为最小执行单元,所以无需额外的锁保护与同步机制 */
/* 也即:它约等于RTOS失去了内核抢占与时间片轮转的功能从而降低了部分实时性 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 错误的任务句柄为NULL */
#define MiniOS_TaskHandle   void *
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TaskReady(void);
/* 线程的创建应在这俩个函数中间 */
void MiniOS_TaskExecute(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MiniOS_TaskHandle MiniOS_TaskCreate(void (*Handler)(uint32_t Event), uint8_t Priority);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TaskDestroy(MiniOS_TaskHandle Handle);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MiniOS_TaskToggle(MiniOS_TaskHandle Handle, uint32_t Event);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif