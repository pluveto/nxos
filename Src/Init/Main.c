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

#include <Platforms/Init.h>
#include <Mods/Console/Console.h>

PUBLIC int OS_Main(void)
{
    /* platfrom init */
    if (PlatformInit() != OS_EOK)
    {
        COUT Str("Platfrom init failed!") Endln PANIC();
    }
    
    SPIN("OS_Main");
    return 0;
}
