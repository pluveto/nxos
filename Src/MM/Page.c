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
#include <Utils/Debug.h>

PRIVATE BuddySystem *BuddySystemArray[PZ_NR]; 

/**
 * Init buddy memory allocator
 */
PUBLIC void PageInitZone(PageZone zone, void *mem, USize size)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR && size > 0);
    BuddySystemArray[zone] = BuddyCreate(mem, size);
    ASSERT(BuddySystemArray[zone] != NULL);
}

PUBLIC void *PageAllocInZone(PageZone zone, USize count)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR && count > 0);
    return BuddyAllocPage(BuddySystemArray[zone], count);
}

PUBLIC void PageFreeInZone(PageZone zone, void *ptr)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR && ptr != NULL);
    BuddyFreePage(BuddySystemArray[zone], ptr);
}

PUBLIC void *PageZoneGetBase(PageZone zone)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR);
    return BuddySystemArray[zone]->pageStart;
}

PUBLIC USize PageZoneGetPages(PageZone zone)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR);
    return (BuddySystemArray[zone]->maxPFN + 1);
}

PUBLIC void *PageZoneGetBuddySystem(PageZone zone)
{
    ASSERT(zone >= PZ_NORMAL && zone < PZ_NR);
    return BuddySystemArray[zone];
}
