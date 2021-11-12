/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: tick spin lock
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-13     JasonHu           Init
 */

#ifndef __SPIN_LOCK___
#define __SPIN_LOCK___

#include <XBook.h>
#include <XBook/Atomic.h>
#include <XBook/HAL.h>

#define SPIN_LOCK_MAGIC 0x5a5af0f0

struct SpinLock
{
    HAL_Atomic currentTick;
    HAL_Atomic nextTick;
    U32 magic;
};
typedef struct SpinLock SpinLock;

INLINE OS_Error SpinLockInit(SpinLock *lock)
{
    if (lock == NULL)
    {
        return OS_EINVAL;
    }
    if (lock->magic == SPIN_LOCK_MAGIC)
    {
        return OS_EFAULT;
    }

    HAL_AtomicSet(&lock->currentTick, 0);
    HAL_AtomicSet(&lock->nextTick, 0);
    lock->magic = SPIN_LOCK_MAGIC;
    return OS_EOK;
}

INLINE OS_Error SpinLockLock(SpinLock *lock)
{
    if (lock == NULL || lock->magic != SPIN_LOCK_MAGIC)
    {
        return OS_EFAULT;
    }

    /* get a tick for me */
    VOLATILE int myTick = HAL_AtomicSwap(&lock->nextTick, HAL_AtomicGet(&lock->nextTick) + 1);   /* return old tick value */

    /* wait my tick equal to current tick to try get lock */
    while (HAL_AtomicGet(&lock->currentTick) != myTick)
    {
        /* do nothing to wait. */
    }
    /* current tick equal to my tick */
    return OS_EOK;
}

INLINE OS_Error SpinLockUnlock(SpinLock *lock)
{
    if (lock == NULL || lock->magic != SPIN_LOCK_MAGIC)
    {
        return OS_EFAULT;
    }
    /* increase current tick to release lock */
    HAL_AtomicInc(&lock->currentTick);
    return OS_EOK;
}

#endif /* __SPIN_LOCK___ */
