/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: mutex for thread
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-13     JasonHu           Init
 */

#include <Sched/Mutex.h>
#include <Sched/Sched.h>
#include <Sched/Thread.h>
#include <IO/IRQ.h>

#define MUTEX_MAGIC 0x10000002

PUBLIC OS_Error MutexInit(Mutex *mutex)
{
    if (mutex == NULL)
    {
        return OS_EINVAL;
    }
    if (SpinInit(&mutex->lock) != OS_EOK)
    {
        return OS_EPERM;
    }
    mutex->magic = MUTEX_MAGIC;
    return OS_EOK;
}

/**
 * forever: if true lock mutex forever, or not return OS_ETIMEOUT if lock falied
 */
PUBLIC OS_Error MutexLock(Mutex *mutex, Bool forever)
{
    if (mutex == NULL || mutex->magic != MUTEX_MAGIC)
    {
        return OS_EFAULT;
    }

    do
    {
        /* disable interrupt for locking per cpu */
        UArch level = INTR_SaveLevel();

        /* spin lock for mutex */
        if (SpinLock(&mutex->lock, FALSE) == OS_EOK)
        {
            INTR_RestoreLevel(level);
            break;
        }
        else
        {
            /* restore interrupt for unlocking per cpu */
            INTR_RestoreLevel(level);

            /* checkout timeout */
            if (forever == FALSE)
            {
                return OS_ETIMEOUT;
            }
            /* yield to other thread */
            ThreadYield();
        }
    } while (1);

    return OS_EOK;
}

PUBLIC OS_Error MutexUnlock(Mutex *mutex)
{
    if (mutex == NULL || mutex->magic != MUTEX_MAGIC)
    {
        return OS_EFAULT;
    }
    SpinUnlock(&mutex->lock);
    return OS_EOK;
}
