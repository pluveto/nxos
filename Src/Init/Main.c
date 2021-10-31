/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Init OS 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 */

#include <HAL.h>
#include <Utils/Debug.h>

PUBLIC int OS_Main(void)
{
    /* platfrom init */
    if (PlatformInit() != OS_EOK)
    {
        PANIC("Platfrom init failed!" Endln);
    }
    
    SPIN("OS_Main");
    return 0;
}
