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
#include <Assert.h>
#include <HAL.h>
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
#include <MM/buddy.h>
#include <MM/Page.h>
#include <Page.h>
#include <Platfrom.h>

IMPORT void PlatfromTest(void);

IMPORT Uint __bssStart;
IMPORT Uint __bssEnd;

PRIVATE void ClearBSS(void)
{
    Zero(&__bssStart, &__bssEnd - &__bssStart);
}

#define GRUB2_READ_MEMORY_BYTES_ADDR (0x000001000)

PRIVATE void PageInit(void)
{    
    U32 memSize = *(U32 *)GRUB2_READ_MEMORY_BYTES_ADDR;
    if (memSize == 0)
    {
        PANIC("Get Memory Size Failed!");
    }
    Cout("Memory Size: " $x(memSize) " Bytes " $d(memSize / SZ_MB) " MB" Endln);
    U32 normalSize = memSize - MEM_DMA_SIZE - MEM_KERNEL_SZ;
    /* init page zone */
    PageInitZone(PZ_DMA, (void *)MEM_DMA_BASE, MEM_DMA_SIZE);
    PageInitZone(PZ_NORMAL, (void *)MEM_NORMAL_BASE, normalSize);
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
    PageInit();
    
    PlatfromTest();
    
    SPIN("test");
    // HAL_InterruptEnable();

    return OS_EOK;
}
