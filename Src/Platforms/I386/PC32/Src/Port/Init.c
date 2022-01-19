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
#include <Segment.h>
#include <Gate.h>
#include <Interrupt.h>
#include <TSS.h>
#include <PageZone.h>
#include <Platform.h>

#define NX_LOG_LEVEL NX_LOG_INFO
#define NX_LOG_NAME "INIT"
#include <Utils/Log.h>

#include <Drivers/DirectUart.h>

NX_INTERFACE NX_Error HAL_PlatformInit(NX_UArch coreId)
{
    HAL_ClearBSS();
    
    HAL_DirectUartInit();
    
    NX_Printf("hello, world!\na=%x b=%d c=%c e=%s\n", 0x1234abcd, 123456789, 'A', "hello");
    
    NX_LOG_I("Hello, PC32!");

    CPU_InitGate();
    CPU_InitSegment();
    CPU_InitTSS();
    CPU_InitInterrupt();
    
    HAL_PageZoneInit();

    return NX_EOK;
}

NX_INTERFACE NX_Error HAL_PlatformStage2(void)
{
    HAL_DirectUartStage2();
    
    return NX_EOK;
}
