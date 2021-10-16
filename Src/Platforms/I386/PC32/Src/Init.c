/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Init PC32 platfrom 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-17      JasonHu           Init
 */

#include <XBook.h>
#include <Platforms/Init.h>
#include <Mods/Console/Console.h>
#include <Utils/List.h>

#include <I386.h>
#include <DirectUart.h>
#include <Segment.h>
#include <Gate.h>
#include <Interrupt.h>
#include <TSS.h>
#include <Clock.h>
#include <Context.h>

#define STACK_SZ 1024

char threadStack1[STACK_SZ];
char threadStack2[STACK_SZ];

char *threadSP1;
char *threadSP2;

void thread_entry1(void *arg)
{
    COUT Str("Thread 1...") Endln;
    COUT Hex(arg) Endln;

    while (1)
    {
        COUT Str("Hello 1") Endln;
        HAL_ContextSwitchPrevNext(&threadSP1, &threadSP2);
    }    
}

void thread_entry2(void *arg)
{
    COUT Str("Thread 2...") Endln;
    COUT Hex(arg) Endln;

    while (1)
    {
        COUT Str("Hello 2") Endln;
        HAL_ContextSwitchPrevNext(&threadSP2, &threadSP1);
    }
}

PRIVATE void ThreadTest(void)
{
    threadSP1 = HAL_ContextInit(thread_entry1, (void *) 0x12345678, threadStack1, NULL);
    threadSP2 = HAL_ContextInit(thread_entry2, (void *) 0x12345678, threadStack2, NULL);
    HAL_ContextSwitchNext(&threadSP1);
}

INTERFACE OS_Error PlatformInit(void)
{
    HAL_DirectUartInit();
    
    COUT Str("Hello, PC32!") Endln;

    CPU_InitGate();
    CPU_InitSegment();
    CPU_InitTSS();
    CPU_InitInterrupt();
    
    if (HAL_InitClock() != OS_EOK)
    {
        COUT Str("Init clock failed!") Endln;
        return OS_ERROR;
    }
    
    List listHead;

    List list1, list2;

    ListInit(&listHead);
    ListInit(&list1);
    ListInit(&list2);

    SPIN("test");
    // ThreadTest();

    // HAL_InterruptEnable();

    return OS_EOK;
}
