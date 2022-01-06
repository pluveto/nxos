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

struct NX_Mutex
{
    NX_Spin lock;  /* lock for mutex value */
    NX_U32 magic;  /* magic for mutex init */  
};
typedef struct NX_Mutex NX_Mutex;

NX_PUBLIC NX_Error NX_MutexInit(NX_Mutex *mutex);
NX_PUBLIC NX_Error NX_MutexLock(NX_Mutex *mutex, NX_Bool forever);
NX_PUBLIC NX_Error NX_MutexUnlock(NX_Mutex *mutex);

#endif /* __SCHED_MUTEX___ */
