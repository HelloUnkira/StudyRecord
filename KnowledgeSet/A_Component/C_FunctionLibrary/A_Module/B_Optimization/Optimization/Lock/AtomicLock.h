#ifndef ATOMIC_LOCK_H
#define ATOMIC_LOCK_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "CriticalZone.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 定义原子量(这里是为了方便,实际上需要用位图去做原子量集合) */
#define AtomicValue     bool
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 比较与配置(设置,重置)原子量 */
static inline bool Atomic_CC(AtomicValue *Atomic, AtomicValue OldValue, AtomicValue NewValue)
{
    bool Result = false;
    CriticalZoneEnter(AtomicLock);
    
    /* 核心的状态切换步骤 */
    if (*Atomic == OldValue) {
        *Atomic  = NewValue;
         Result  = true;
    }
    
    CriticalZoneExit(AtomicLock);
    return Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
