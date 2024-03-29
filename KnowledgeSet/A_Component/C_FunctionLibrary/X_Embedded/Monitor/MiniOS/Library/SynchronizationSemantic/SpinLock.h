#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "AtomicLock.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define SpinLock    AtomicValue
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool Spin_TryLock(SpinLock *Lock)
{
    return Atomic_CC(Lock, 0, 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool Spin_TryUnlock(SpinLock *Lock)
{
    return Atomic_CC(Lock, 1, 0);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void Spin_Lock(SpinLock *Lock, void (*Block)(void))
{
    while (!Atomic_CC(Lock, 0, 1)) if (Block) Block();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void Spin_UnLock(SpinLock *Lock, void (*Block)(void))
{
    while (!Atomic_CC(Lock, 1, 0)) if (Block) Block();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
