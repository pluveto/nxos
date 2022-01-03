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

#include <Utils/Demo.h>
#include <Utils/Log.h>
#include <Sched/Context.h>

#ifdef CONFIG_DEMO_HAL_CONTEXT

#define STACK_SZ 2048

U8 threadStack1[STACK_SZ];
U8 threadStack2[STACK_SZ];
U8 threadStack3[STACK_SZ];

U8 *threadSP1;
U8 *threadSP2;
U8 *threadSP3;

PRIVATE void thread_entry1(void *arg)
{
    LOG_D("Thread 1... %x", arg);
    
    while (1)
    {
        LOG_D("Hello 1");
        ContextSwitchPrevNext((UArch)&threadSP1, (UArch)&threadSP2);
    }
}

PRIVATE void thread_entry2(void *arg)
{
    LOG_D("Thread 2... %x", arg);

    while (1)
    {
        LOG_D("Hello 2");
        ContextSwitchPrevNext((UArch)&threadSP2, (UArch)&threadSP3);
    }
}

PRIVATE void thread_entry3(void *arg)
{
    LOG_D("Thread 3... %x", arg);

    while (1)
    {
        LOG_D("Hello 3");
        ContextSwitchPrevNext((UArch)&threadSP3, (UArch)&threadSP1);
    }
}

PUBLIC void Demo_HAL_Context(void)
{
    LOG_I("start demo: HAL_Context");
    threadSP1 = ContextInit(thread_entry1, NULL, (void *) 0x12345678, threadStack1 + STACK_SZ);
    threadSP2 = ContextInit(thread_entry2, NULL, (void *) 0x12345678, threadStack2 + STACK_SZ);
    threadSP3 = ContextInit(thread_entry3, NULL, (void *) 0x12345678, threadStack3 + STACK_SZ);
    
    LOG_I("demo: HAL_Context: switch");
    ContextSwitchNext((UArch)&threadSP1);
    LOG_I("end demo: HAL_Context");
}

#endif
