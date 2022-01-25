/**
 * Copyright (c) 2018-2022, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Page alloctor
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-18     JasonHu           Init
 */

#include <mm/buddy.h>
#include <mm/page.h>
#include <utils/log.h>
#include <xbook/debug.h>

NX_PRIVATE NX_BuddySystem *BuddySystemArray[NX_PAGE_ZONE_NR]; 

/**
 * Init buddy memory allocator
 */
NX_PUBLIC void NX_PageInitZone(NX_PageZone zone, void *mem, NX_USize size)
{
    NX_ASSERT(zone >= NX_PAGE_ZONE_NORMAL && zone < NX_PAGE_ZONE_NR && size > 0);
    BuddySystemArray[zone] = NX_BuddyCreate(mem, size);
    NX_ASSERT(BuddySystemArray[zone] != NX_NULL);
}

NX_PUBLIC void *NX_PageAllocInZone(NX_PageZone zone, NX_USize count)
{
    NX_ASSERT(zone >= NX_PAGE_ZONE_NORMAL && zone < NX_PAGE_ZONE_NR && count > 0);
    return NX_BuddyAllocPage(BuddySystemArray[zone], count);
}

NX_PUBLIC NX_Error NX_PageFreeInZone(NX_PageZone zone, void *ptr)
{
    NX_ASSERT(zone >= NX_PAGE_ZONE_NORMAL && zone < NX_PAGE_ZONE_NR && ptr != NX_NULL);
    return NX_BuddyFreePage(BuddySystemArray[zone], ptr);
}

NX_PUBLIC NX_Error NX_PageIncreaseInZone(NX_PageZone zone, void *ptr)
{
    NX_ASSERT(zone >= NX_PAGE_ZONE_NORMAL && zone < NX_PAGE_ZONE_NR && ptr != NX_NULL);
    return NX_BuddyIncreasePage(BuddySystemArray[zone], ptr);
}

NX_PUBLIC void *NX_PageZoneGetBase(NX_PageZone zone)
{
    NX_ASSERT(zone >= NX_PAGE_ZONE_NORMAL && zone < NX_PAGE_ZONE_NR);
    return BuddySystemArray[zone]->pageStart;
}

NX_PUBLIC NX_USize NX_PageZoneGetPages(NX_PageZone zone)
{
    NX_ASSERT(zone >= NX_PAGE_ZONE_NORMAL && zone < NX_PAGE_ZONE_NR);
    return (BuddySystemArray[zone]->maxPFN + 1);
}

NX_PUBLIC void *NX_PageZoneGetBuddySystem(NX_PageZone zone)
{
    NX_ASSERT(zone >= NX_PAGE_ZONE_NORMAL && zone < NX_PAGE_ZONE_NR);
    return BuddySystemArray[zone];
}
