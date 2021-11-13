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

// #define TEST_THREAD_ID

PRIVATE struct ThreadID threadID;

PUBLIC I32 ThreadIdAlloc(void)
{
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
            threadID.nextID = (nextID + 1) % MAX_THREAD_ID_NR;
            break;
        }
        nextID = (nextID + 1) % MAX_THREAD_ID_NR;
    } while (nextID != threadID.nextID);
    /* nextID == threadID.nextID means no id free */
    return (nextID != threadID.nextID) ? nextID : -1;
}

PUBLIC void ThreadIdFree(I32 id)
{
    if (id < 0 || id >= MAX_THREAD_ID_NR)
    {
        return;
    }
    U32 idx = id / 32;
    U32 odd = id % 32;
    ASSERT(threadID.maps[idx] & (1 << odd));
    threadID.maps[idx] &= ~(1 << odd);   /* clear id */     
}

#ifdef TEST_THREAD_ID
PRIVATE void TestThreadID(void)
{
    int i;
    for (i = 0; i < 32; i++)
    {
        I32 id = ThreadIdAlloc();
        LOG_D("alloc id: %d", id);
        ThreadIdFree(id);
    }
    for (i = 0; i < 32; i++)
    {
        I32 id = ThreadIdAlloc();
        LOG_D("alloc id: %d", id);
    }

    ThreadIdFree(1);
    ThreadIdFree(3);
    ThreadIdFree(5);
    ThreadIdFree(7);
    
    for (i = 0; i < 4; i++)
    {
        I32 id = ThreadIdAlloc();
        LOG_D("alloc id: %d", id);
        ThreadIdFree(id);
    }
}
#endif

PUBLIC void InitThreadID(void)
{
    threadID.maps = MemAlloc(MAX_THREAD_ID_NR / 8);
    ASSERT(threadID.maps != NULL);
    Zero(threadID.maps, MAX_THREAD_ID_NR / 8);
    threadID.nextID = 0;
#ifdef TEST_THREAD_ID
    TestThreadID();
#endif
}
