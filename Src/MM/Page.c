/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Page alloctor
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-18     JasonHu           Init
 */

#include <MM/Buddy.h>
#include <MM/Page.h>
#include <Assert.h>

PRIVATE BuddySystem *buddySystem[PZ_NR]; 

/**
 * Init buddy memory allocator
 */
PUBLIC void PageInitZone(PageZone zone, void *mem, Size size)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR && size > 0);
    buddySystem[zone] = BuddyCreate(mem, size);
    ASSERT(buddySystem[zone] != NULL);
}

PUBLIC void *PageAllocInZone(PageZone zone, Size count)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR && count > 0);
    return BuddyAllocPage(buddySystem[zone], count);
}

PUBLIC void PageFreeInZone(PageZone zone, void *ptr)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR && ptr != NULL);
    BuddyFreePage(buddySystem[zone], ptr);
}

PUBLIC void *PageZoneGetBase(PageZone zone)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR);
    return buddySystem[zone]->pageStart;
}

PUBLIC Size PageZoneGetPages(PageZone zone)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR);
    return (buddySystem[zone]->maxPFN + 1);
}

PUBLIC void *PageZoneGetBuddySystem(PageZone zone)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR);
    return buddySystem[zone];
}
