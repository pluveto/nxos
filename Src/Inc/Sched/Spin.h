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
    Atomic value;
    U32 magic;  /* magic for spin init */
};
typedef struct Spin Spin;

#define STATIC_SPIN_UNLOCKED(name) Spin name = {ATOMIC_INIT_VALUE(0), SPIN_MAGIC}
#define STATIC_SPIN_LOCKED(name) Spin name = {ATOMIC_INIT_VALUE(1), SPIN_MAGIC}

PUBLIC OS_Error SpinInit(Spin *lock);
PUBLIC OS_Error SpinLock(Spin *lock, Bool forever);
PUBLIC OS_Error SpinUnlock(Spin *lock);
PUBLIC OS_Error SpinLockIRQ(Spin *lock, UArch *level);
PUBLIC OS_Error SpinUnlockIRQ(Spin *lock, UArch level);

#endif /* __SCHED_SPIN___ */
