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
#include <I386.h>
#include <DirectUart.h>

int PlatformInit(void)
{
    HAL_DirectUartInit();

    HAL_DirectUartPutc('A');
    u32 i = 0;
    for (;;) 
    {
        HAL_DirectUartPutc('0' + i);
        i++;
        if (i > 9)
        {
            i = 0;
        }
    }
    
    return 0;
}