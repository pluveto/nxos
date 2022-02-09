/**
 * Copyright (c) 2018-2022, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Platfrom main 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-18     JasonHu           Init
 */

#include <xbook.h>
#include <drivers/direct_uart.h>

#define NX_LOG_NAME "HAL_PlatformMain"
#include <utils/log.h>
#include <sched/process.h>

NX_INTERFACE void HAL_PlatformMain(void)
{
    NX_LOG_I("PC32 platform main running...\n");

    int i;
    for (i = 0; i < 256; i++)
    {
        NX_Error err = NX_ProcessExecute("nxbase_test", "nxbase_test.bin", 0);
        NX_LOG_I("CREATE: %d", err);
    }
}

NX_PUBLIC void HAL_DirectUartGetcHandler(char data)
{
    switch (data)
    {
    case '\r':
    case '\n':
        HAL_DirectUartPutc('\n');
        break;
    case '\b':
        HAL_DirectUartPutc('\b');
        HAL_DirectUartPutc(' ');
        HAL_DirectUartPutc('\b');
        break;
    default:
        HAL_DirectUartPutc(data);
        break;
    }
}
