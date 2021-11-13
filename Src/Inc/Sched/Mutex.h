/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: mutex lock
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-13     JasonHu           Init
 */

#ifndef __SCHED_MUTEX___
#define __SCHED_MUTEX___

#include <XBook.h>
#include <Sched/Spin.h>

struct Mutex
{
    Spin lock;  /* lock for mutex value */
    U32 magic;  /* magic for mutex init */  
};
typedef struct Mutex Mutex;

PUBLIC OS_Error MutexInit(Mutex *mutex);
PUBLIC OS_Error MutexLock(Mutex *mutex, Bool forever);
PUBLIC OS_Error MutexUnlock(Mutex *mutex);

#endif /* __SCHED_MUTEX___ */
