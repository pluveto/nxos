/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Platfrom main 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-18     JasonHu           Init
 */

#include <XBook.h>
#include <Drivers/DirectUart.h>

#define LOG_NAME "PlatformMain"
#include <Utils/Log.h>

INTERFACE void PlatformMain(void)
{
    LOG_I("PC32 platform main running...\n");
}

PUBLIC void HAL_DirectUartGetcHandler(char data)
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
