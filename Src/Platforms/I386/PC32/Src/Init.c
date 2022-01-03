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
#include <Clock.h>
#include <Page.h>
#include <Platform.h>

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "INIT"
#include <Utils/Log.h>

#include <Drivers/DirectUart.h>


PUBLIC void ClearBSS(void);

INTERFACE OS_Error PlatformInit(UArch coreId)
{
    ClearBSS();
    
    HAL_DirectUartInit();
    
    Printf("hello, world!\na=%x b=%d c=%c e=%s\n", 0x1234abcd, 123456789, 'A', "hello");
    
    LOG_I("Hello, PC32!");

    CPU_InitGate();
    CPU_InitSegment();
    CPU_InitTSS();
    CPU_InitInterrupt();
    
    PageInit();

    return OS_EOK;
}

INTERFACE OS_Error PlatformStage2(void)
{
    if (HAL_InitClock() != OS_EOK)
    {
        LOG_E("Init clock failed!");
        return OS_ERROR;
    }
    HAL_DirectUartStage2();
    
    return OS_EOK;
}
