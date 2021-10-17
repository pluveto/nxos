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
#include <Utils/Memory.h>

#include <I386.h>
#include <DirectUart.h>
#include <Segment.h>
#include <Gate.h>
#include <Interrupt.h>
#include <TSS.h>
#include <Clock.h>
#include <Context.h>

#define CONFIG_THREAD_TEST

#ifdef CONFIG_THREAD_TEST
#define STACK_SZ 1024

U8 threadStack1[STACK_SZ];
U8 threadStack2[STACK_SZ];

U8 *threadSP1;
U8 *threadSP2;

void thread_entry1(void *arg)
{
    Cout("Thread 1..." $x(arg) Endln);
    
    while (1)
    {
        Cout("Hello 1" Endln);
        HAL_ContextSwitchPrevNext((UBase)&threadSP1, (UBase)&threadSP2);
    }    
}

void thread_entry2(void *arg)
{
    Cout("Thread 2..." $x(arg) Endln);

    while (1)
    {
        Cout("Hello 2" Endln);
        HAL_ContextSwitchPrevNext((UBase)&threadSP2, (UBase)&threadSP1);
    }
}

PRIVATE void ThreadTest(void)
{
    threadSP1 = HAL_ContextInit(thread_entry1, (void *) 0x12345678, threadStack1, NULL);
    threadSP2 = HAL_ContextInit(thread_entry2, (void *) 0x12345678, threadStack2, NULL);
    HAL_ContextSwitchNext((UBase)&threadSP1);
}
#endif

IMPORT UBase __bssStart;
IMPORT UBase __bssEnd;

PRIVATE void ClearBSS(void)
{
    Zero(&__bssStart, &__bssEnd - &__bssStart);
}

INTERFACE OS_Error PlatformInit(void)
{
    ClearBSS();
    
    HAL_DirectUartInit();
    
    Cout("Hello, PC32!" Endln);

    CPU_InitGate();
    CPU_InitSegment();
    CPU_InitTSS();
    CPU_InitInterrupt();
    
    if (HAL_InitClock() != OS_EOK)
    {
        Cout("Init clock failed!" Endln);
        return OS_ERROR;
    }
    
    char *s = "hello";
    int a = 12345678;
    int b = 0x12345678;
    Cout("Cout:" $s(s) " 16:" $x(0x12345678) " 10:" $d(0x12345678) "a:" $d(a) "b:" $X(b) " End." "\n");

#ifdef CONFIG_THREAD_TEST
    ThreadTest();
#endif

    SPIN("test");
    

    // HAL_InterruptEnable();

    return OS_EOK;
}
