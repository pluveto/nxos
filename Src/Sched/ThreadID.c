/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: NX_Thread ID for NXOS
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-7      JasonHu           Init
 */

#include <Sched/ThreadID.h>
#include <MM/Alloc.h>
#include <XBook/Debug.h>
#include <Utils/Memory.h>

NX_PRIVATE struct NX_ThreadID ThreadIdObject;

NX_PUBLIC int NX_ThreadIdAlloc(void)
{
    NX_MutexLock(&ThreadIdObject.idLock, NX_True);

    NX_U32 nextID = ThreadIdObject.nextID;
    do 
    {
        NX_U32 idx = nextID / 32;
        NX_U32 odd = nextID % 32;
        if (!(ThreadIdObject.maps[idx] & (1 << odd)))
        {
            /* mark id used */
            ThreadIdObject.maps[idx] |= (1 << odd);
            /* set next id */
            ThreadIdObject.nextID = (nextID + 1) % NX_MAX_THREAD_NR;
            break;
        }
        nextID = (nextID + 1) % NX_MAX_THREAD_NR;
    } while (nextID != ThreadIdObject.nextID);

    /* nextID == ThreadIdObject.nextID means no id free */
    int id = (nextID != ThreadIdObject.nextID) ? nextID : -1;
    NX_MutexUnlock(&ThreadIdObject.idLock);
    return id;
}

NX_PUBLIC void NX_ThreadIdFree(int id)
{
    if (id < 0 || id >= NX_MAX_THREAD_NR)
    {
        return;
    }
    
    NX_MutexLock(&ThreadIdObject.idLock, NX_True);
    NX_U32 idx = id / 32;
    NX_U32 odd = id % 32;
    NX_ASSERT(ThreadIdObject.maps[idx] & (1 << odd));
    ThreadIdObject.maps[idx] &= ~(1 << odd);   /* clear id */
    NX_MutexUnlock(&ThreadIdObject.idLock);  
}

NX_PUBLIC void NX_ThreadsInitID(void)
{
    ThreadIdObject.maps = NX_MemAlloc(NX_MAX_THREAD_NR / 8);
    NX_ASSERT(ThreadIdObject.maps != NX_NULL);
    NX_MemZero(ThreadIdObject.maps, NX_MAX_THREAD_NR / 8);
    ThreadIdObject.nextID = 0;
    NX_MutexInit(&ThreadIdObject.idLock);
}
