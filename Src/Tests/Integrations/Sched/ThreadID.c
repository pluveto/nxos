/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Thread ID test
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-16     JasonHu           Init
 */

#include <Sched/ThreadID.h>
#include <Utils/Debug.h>
#include <Mods/Test/Integration.h>

#ifdef CONFIG_TEST_INTEGRATION_THREAD_ID

INTEGRATION_TEST(TestThreadID)
{
    int i;
    for (i = 0; i < 32; i++)
    {
        int id = ThreadIdAlloc();
        LOG_D("alloc id: %d", id);
        ThreadIdFree(id);
    }
    
    for (i = 0; i < 4; i++)
    {
        int id = ThreadIdAlloc();
        LOG_D("alloc id: %d", id);
        ThreadIdFree(id);
    }
    return OS_EOK;
}

#endif
