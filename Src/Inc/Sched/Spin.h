/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: busy spin lock
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-13     JasonHu           Init
 */

#ifndef __SCHED_SPIN___
#define __SCHED_SPIN___

#include <XBook.h>
#include <XBook/Atomic.h>

#define SPIN_MAGIC 0x10000001
#define SPIN_LOCK_VALUE 1

struct Spin
{
    HAL_Atomic value;
    U32 magic;  /* magic for spin init */
};
typedef struct Spin Spin;

INLINE OS_Error SpinInit(Spin *lock)
{
    if (lock == NULL)
    {
        return OS_EINVAL;
    }
    if (lock->magic == SPIN_MAGIC)
    {
        return OS_EFAULT;
    }

    HAL_AtomicSet(&lock->value, 0);
    lock->magic = SPIN_MAGIC;
    return OS_EOK;
}

INLINE OS_Error SpinLock(Spin *lock, Bool forever)
{
    if (lock == NULL || lock->magic != SPIN_MAGIC)
    {
        return OS_EFAULT;
    }

    do
    {
        if (HAL_AtomicCAS(&lock->value, 0, SPIN_LOCK_VALUE) == 0)
        {
            break;
        }
        if (forever == FALSE)
        {
            return OS_ETIMEOUT;
        }
    } while (1);

    return OS_EOK;
}

INLINE OS_Error SpinUnlock(Spin *lock)
{
    if (lock == NULL || lock->magic != SPIN_MAGIC)
    {
        return OS_EFAULT;
    }
    HAL_AtomicSet(&lock->value, 0);
    return OS_EOK;
}

#endif /* __SCHED_SPIN___ */