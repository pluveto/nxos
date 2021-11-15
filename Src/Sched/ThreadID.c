/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Thread ID for NXOS
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-7      JasonHu           Init
 */

#include <Sched/ThreadID.h>
#include <MM/Alloc.h>
#include <Utils/Debug.h>
#include <Utils/Memory.h>

PRIVATE struct ThreadID threadID;

PUBLIC int ThreadIdAlloc(void)
{
    MutexLock(&threadID.idLock, TRUE);

    U32 nextID = threadID.nextID;
    do 
    {
        U32 idx = nextID / 32;
        U32 odd = nextID % 32;
        if (!(threadID.maps[idx] & (1 << odd)))
        {
            /* mark id used */
            threadID.maps[idx] |= (1 << odd);
            /* set next id */
            threadID.nextID = (nextID + 1) % MAX_THREAD_NR;
            break;
        }
        nextID = (nextID + 1) % MAX_THREAD_NR;
    } while (nextID != threadID.nextID);

    /* nextID == threadID.nextID means no id free */
    int id = (nextID != threadID.nextID) ? nextID : -1;
    MutexUnlock(&threadID.idLock);
    return id;
}

PUBLIC void ThreadIdFree(int id)
{
    if (id < 0 || id >= MAX_THREAD_NR)
    {
        return;
    }
    
    MutexLock(&threadID.idLock, TRUE);
    U32 idx = id / 32;
    U32 odd = id % 32;
    ASSERT(threadID.maps[idx] & (1 << odd));
    threadID.maps[idx] &= ~(1 << odd);   /* clear id */
    MutexUnlock(&threadID.idLock);  
}

PUBLIC void ThreadsInitID(void)
{
    threadID.maps = MemAlloc(MAX_THREAD_NR / 8);
    ASSERT(threadID.maps != NULL);
    Zero(threadID.maps, MAX_THREAD_NR / 8);
    threadID.nextID = 0;
    MutexInit(&threadID.idLock);
}
