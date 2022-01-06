/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Clock for system
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-3      JasonHu           Init
 */

#ifndef __PLATFROM_CLOCK__
#define __PLATFROM_CLOCK__

#include <XBook.h>

NX_PUBLIC void HAL_InitClock(void);
NX_PUBLIC void HAL_ClockHandler(void);

#endif /* __PLATFROM_CLOCK__ */
