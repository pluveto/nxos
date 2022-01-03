/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: spin lock
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-21     JasonHu           Init
 */

#include <Sched/Spin.h>
#include <IO/IRQ.h>
#include <Utils/Debug.h>

PUBLIC OS_Error SpinInit(Spin *lock)
{
    if (lock == NULL)
    {
        return OS_EINVAL;
    }
    if (lock->magic == SPIN_MAGIC)
    {
        return OS_EFAULT;
    }

    AtomicSet(&lock->value, 0);
    lock->magic = SPIN_MAGIC;
    lock->irqLevel = 0;
    AtomicSet(&lock->irqDepth, 0);
    return OS_EOK;
}

PUBLIC OS_Error SpinLock(Spin *lock, Bool forever)
{
    if (lock == NULL || lock->magic != SPIN_MAGIC)
    {
        return OS_EFAULT;
    }

    do
    {
        if (AtomicCAS(&lock->value, 0, SPIN_LOCK_VALUE) == 0)
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

PUBLIC OS_Error SpinUnlock(Spin *lock)
{
    if (lock == NULL || lock->magic != SPIN_MAGIC)
    {
        return OS_EFAULT;
    }
    AtomicSet(&lock->value, 0);
    return OS_EOK;
}

PUBLIC OS_Error SpinLockIRQ(Spin *lock)
{
    if (lock == NULL || lock->magic != SPIN_MAGIC)
    {
        return OS_EINVAL;
    }
    if (AtomicGet(&lock->irqDepth) == 0)
    {
        lock->irqLevel = INTR_SaveLevel();
    }
    AtomicInc(&lock->irqDepth);
    return SpinLock(lock, TRUE);
}

PUBLIC OS_Error SpinUnlockIRQ(Spin *lock)
{
    if (lock == NULL || lock->magic != SPIN_MAGIC)
    {
        return OS_EINVAL;
    }
    if (SpinUnlock(lock) != OS_EOK)
    {
        return OS_EFAULT;
    }
    AtomicDec(&lock->irqDepth);
    if (AtomicGet(&lock->irqDepth) == 0)
    {
        INTR_RestoreLevel(lock->irqLevel);
    }
    ASSERT(AtomicGet(&lock->irqDepth) >= 0);
    return OS_EOK;
}
