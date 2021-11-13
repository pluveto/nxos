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

#define SPIN_LOCK_MAGIC 0x5a5af0f0
#define SPIN_LOCK_VALUE 1


struct Spin
{
    HAL_Atomic value;
    U32 magic;
};
typedef struct Spin Spin;

INLINE OS_Error SpinInit(Spin *lock)
{
    if (lock == NULL)
    {
        return OS_EINVAL;
    }
    if (lock->magic == SPIN_LOCK_MAGIC)
    {
        return OS_EFAULT;
    }

    HAL_AtomicSet(&lock->value, 0);
    lock->magic = SPIN_LOCK_MAGIC;
    return OS_EOK;
}

INLINE OS_Error SpinLock(Spin *lock, Bool forever)
{
    if (lock == NULL || lock->magic != SPIN_LOCK_MAGIC)
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
    if (lock == NULL || lock->magic != SPIN_LOCK_MAGIC)
    {
        return OS_EFAULT;
    }
    HAL_AtomicSet(&lock->value, 0);
    return OS_EOK;
}

#endif /* __SPIN_LOCK___ */
