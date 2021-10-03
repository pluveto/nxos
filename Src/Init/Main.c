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

PUBLIC int OS_Main(void)
{
    /* platfrom init */
    PlatformInit();
    
    return 0;
}
