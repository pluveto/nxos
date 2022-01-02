/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Hardware Abstraction Layer
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#ifndef __XBOOK_HAL__
#define __XBOOK_HAL__

#include <XBook.h>

/* Platform */
INTERFACE OS_Error PlatformInit(Uint coreId);

/**
 * stage2 means you can do:
 * 1. use MemAlloc/MemFree
 * 2. use Bind IRQ
 * 3. use Thread
 * 4. use Timer
 */
INTERFACE OS_Error PlatformStage2(void);

/* Conosle HAL */
INTERFACE void HAL_ConsoleOutChar(char ch);

#endif  /* __XBOOK_HAL__ */