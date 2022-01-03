/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Log tools
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-12     JasonHu           Init
 */

#include <Utils/Log.h>
#include <Sched/Spin.h>

/* spin lock for log output */
PRIVATE STATIC_SPIN_UNLOCKED(LogOutLock);

PUBLIC OS_Error LogLineLock(UArch *level)
{
    return SpinLockIRQ(&LogOutLock);
}

PUBLIC OS_Error LogLineUnlock(UArch level)
{
    return SpinUnlockIRQ(&LogOutLock);
}
