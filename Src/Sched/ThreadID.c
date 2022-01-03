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
#include <XBook/Debug.h>
#include <Utils/Memory.h>

PRIVATE struct ThreadID ThreadIdObject;

PUBLIC int ThreadIdAlloc(void)
{
    MutexLock(&ThreadIdObject.idLock, TRUE);

    U32 nextID = ThreadIdObject.nextID;
    do 
    {
        U32 idx = nextID / 32;
        U32 odd = nextID % 32;
        if (!(ThreadIdObject.maps[idx] & (1 << odd)))
        {
            /* mark id used */
            ThreadIdObject.maps[idx] |= (1 << odd);
            /* set next id */
            ThreadIdObject.nextID = (nextID + 1) % MAX_THREAD_NR;
            break;
        }
        nextID = (nextID + 1) % MAX_THREAD_NR;
    } while (nextID != ThreadIdObject.nextID);

    /* nextID == ThreadIdObject.nextID means no id free */
    int id = (nextID != ThreadIdObject.nextID) ? nextID : -1;
    MutexUnlock(&ThreadIdObject.idLock);
    return id;
}

PUBLIC void ThreadIdFree(int id)
{
    if (id < 0 || id >= MAX_THREAD_NR)
    {
        return;
    }
    
    MutexLock(&ThreadIdObject.idLock, TRUE);
    U32 idx = id / 32;
    U32 odd = id % 32;
    ASSERT(ThreadIdObject.maps[idx] & (1 << odd));
    ThreadIdObject.maps[idx] &= ~(1 << odd);   /* clear id */
    MutexUnlock(&ThreadIdObject.idLock);  
}

PUBLIC void ThreadsInitID(void)
{
    ThreadIdObject.maps = MemAlloc(MAX_THREAD_NR / 8);
    ASSERT(ThreadIdObject.maps != NULL);
    Zero(ThreadIdObject.maps, MAX_THREAD_NR / 8);
    ThreadIdObject.nextID = 0;
    MutexInit(&ThreadIdObject.idLock);
}
