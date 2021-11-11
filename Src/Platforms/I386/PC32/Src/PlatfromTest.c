/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Platfrom test
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-18      JasonHu           Init
 */

#include <XBook.h>
#include <XBook/HAL.h>
#include <Context.h>
#include <MM/Page.h>
#include <Platfrom.h>
#include <Utils/Memory.h>

#define LOG_LEVEL LOG_DBG
#define LOG_NAME "TEST"
#include <Utils/Debug.h>

#ifdef CONFIG_THREAD_TEST
#define STACK_SZ 1024

U8 threadStack1[STACK_SZ];
U8 threadStack2[STACK_SZ];

U8 *threadSP1;
U8 *threadSP2;

void thread_entry1(void *arg)
{
    LOG_D("Thread 1..." $x(arg));
    
    while (1)
    {
        LOG_D("Hello 1");
        HAL_ContextSwitchPrevNext((Uint)&threadSP1, (Uint)&threadSP2);
    }    
}

void thread_entry2(void *arg)
{
    LOG_D("Thread 2..." $x(arg));

    while (1)
    {
        LOG_D("Hello 2");
        HAL_ContextSwitchPrevNext((Uint)&threadSP2, (Uint)&threadSP1);
    }
}

PRIVATE void ThreadTest(void)
{
    threadSP1 = HAL_ContextInit(thread_entry1, (void *) 0x12345678, threadStack1, NULL);
    threadSP2 = HAL_ContextInit(thread_entry2, (void *) 0x12345678, threadStack2, NULL);
    HAL_ContextSwitchNext((Uint)&threadSP1);
}
#endif

#ifdef CONFIG_CONSOLE_TEST
PRIVATE void ConsoleTest(void)
{
    char *s = "hello";
    int a = 12345678;
    int b = 0x12345678;
    LOG_D(s, " 16:" $x(0x12345678) " 10:" $d(0x12345678) "a:" $d(a) "b:" $X(b) " End.");

    ASSERT(a == 12345678);   
}
#endif

#ifdef CONFIG_PAGE_TEST
PRIVATE void PageTest(void)
{
    /* alloc memory */
    void *bufs[16];

    int i;
    for (i = 0; i < 16; i++)
    {
        void *buf = PageAlloc(i + 1);
        Zero(buf, PAGE_SIZE);
        LOG_D("Alloc:" $x(buf));
        bufs[i] = buf; 
    }

    for (i = 0; i < 16; i++)
    {
        LOG_D("Free:" $x(bufs[i]));
        if (bufs[i])
        {
            PageFree(bufs[i]);
        }
    }
    for (i = 0; i < 16; i++)
    {
        void *buf = PageAlloc(i + 1);
        Zero(buf, PAGE_SIZE);
        LOG_D("Alloc:" $x(buf));
        bufs[i] = buf; 
    }

    for (i = 0; i < 16; i++)
    {
        void *buf = PageAllocInZone(PZ_DMA, i + 1);
        Zero(buf, PAGE_SIZE);
        LOG_D("Alloc:" $x(buf));
        bufs[i] = buf; 
    }

    for (i = 0; i < 16; i++)
    {
        LOG_D("Free:" $x(bufs[i]));
        if (bufs[i])
        {
            PageFreeInZone(PZ_DMA, bufs[i]);
        }
    }
    for (i = 0; i < 16; i++)
    {
        void *buf = PageAllocInZone(PZ_DMA, i + 1);
        Zero(buf, PAGE_SIZE);
        LOG_D("Alloc:" $x(buf));
        bufs[i] = buf; 
    }
}
#endif

PUBLIC void PlatfromTest(void)
{
#ifdef CONFIG_CONSOLE_TEST
    ConsoleTest();
#endif

#ifdef CONFIG_THREAD_TEST
    ThreadTest();
#endif

#ifdef CONFIG_PAGE_TEST
    PageTest();
#endif

}
