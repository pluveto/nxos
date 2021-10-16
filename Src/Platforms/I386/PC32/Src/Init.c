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

#include <I386.h>
#include <DirectUart.h>
#include <Segment.h>
#include <Gate.h>
#include <Interrupt.h>
#include <TSS.h>
#include <Clock.h>

INTERFACE int PlatformInit(void)
{
    HAL_DirectUartInit();
    
    COUT Str("Hello, PC32!") Endln;

    CPU_InitGate();
    CPU_InitSegment();
    CPU_InitTSS();
    CPU_InitInterrupt();
    
    if (HAL_InitClock() != OS_EOK)
    {
        COUT Str("Init clock error!") Endln PANIC;
    }

    HAL_InterruptEnable();
    
    /*
    int a = 3;
    int b = a / 0;
    */
    U32 i = 0;
    for (;;)
    {
        i++;
    }
    return 0;
}
