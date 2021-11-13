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
#include <XBook/HAL.h>
#include <Sched/Sched.h>

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

PUBLIC OS_Error MutexLock(Mutex *mutex, Bool forever)
{
    if (mutex == NULL || mutex->magic != MUTEX_MAGIC)
    {
        return OS_EFAULT;
    }

    do
    {
        /* disable interrupt for locking per cpu */
        Uint level = HAL_InterruptSaveLevel();

        /* spin lock for mutex */
        if (SpinLock(&mutex->lock, FALSE) == OS_EOK)
        {
            HAL_InterruptRestoreLevel(level);
            break;
        }
        else
        {
            /* restore interrupt for unlocking per cpu */
            HAL_InterruptRestoreLevel(level);

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
