/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Configure for each platform
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-18      JasonHu           Init
 */

#ifndef __PLATFORM_PAGE__
#define __PLATFORM_PAGE__

#include <XBook.h>

/**
 * Must define & init PageZone in each paltfrom 
 */
enum PageZone
{
    PZ_NORMAL = 0,
    PZ_DMA,
    PZ_USER,
    PZ_NR,  /* Must be last member */
};
typedef enum PageZone PageZone;

#endif  /* __PLATFORM_PAGE__ */
