/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Buddy system
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-17     JasonHu           Update code style
 */

#include "BuddyCommon.h"
#include <MM/Buddy.h>

PRIVATE void BuddyAddPage(BuddySystem* system, Page* page)
{
    ASSERT(system && page);

    int order = page->order;
    page->flags |= PAGE_FLAG_IS_FREE;

    ListAddTail(&page->list, &system->pageBuddy[order]);
    system->bitmap |= 1UL << order;
    system->count[order]++;
}

PRIVATE void BuddyDelPage(BuddySystem* system, Page* page)
{
    ASSERT(system && page);

    int order = page->order;
    ListDel(&page->list);
    page->flags &= ~PAGE_FLAG_IS_FREE;

    if (ListEmpty(&system->pageBuddy[order]))
        system->bitmap &= ~(1UL << order);
    system->count[order]--;
}

PRIVATE void *PageToPtr(BuddySystem* system, Page* page)
{
    PtrDiff diff = page - system->map;
    return &ArraryCast(system->pageStart, PAGE_SIZE)[diff];
}

PUBLIC Page* PageFromPtr(BuddySystem* system, void *ptr)
{
    ASSERT(0 == ((USize)ptr % PAGE_SIZE));
    PtrDiff diff = ArraryCast(ptr, PAGE_SIZE) - ArraryCast(system->pageStart, PAGE_SIZE);
    return &system->map[diff];
}

PRIVATE USize PageToPFN(BuddySystem* system, Page* page)
{
    ASSERT(system);

    PtrDiff diff = page - system->map;
    return diff;
}

USED PRIVATE Page* PageFromPFN(BuddySystem* system, USize pfn)
{
    ASSERT(system);
    return &system->map[pfn];
}

PRIVATE int IsValidPFN(BuddySystem* system, USize pfn)
{
    ASSERT(system);

    return pfn <= system->maxPFN;
}

PRIVATE int DoPageFree(Page* page)
{
    return (page->flags & PAGE_FLAG_IS_FREE);
}

PRIVATE BuddySystem* BuddyCreateFromMemory(void *mem)
{
    ASSERT(mem);
    BuddySystem* system = mem;
    int order;
    for (order = 0; order <= MAX_PAGE_ORDER; order++)
    {
        ListInit(&system->pageBuddy[order]);
        system->count[order] = 0UL;
    }
    system->bitmap = 0UL;
    system->pageStart = NULL;

    return system;
}

PUBLIC BuddySystem* BuddyCreate(void *mem, USize size)
{
    ASSERT(mem && size);
    LOG_I("mem: 0x%p size: 0x%p", mem, size);
    if (!(mem && size))
    {
        return NULL;
    }

    // Alloc BuddySystem
    BuddySystem* system = BuddyCreateFromMemory(mem);
    mem += sizeof(BuddySystem);
    size -= sizeof(BuddySystem);

    USize page_count = size >> PAGE_SHIFT;

    // Alloc page map
    USize map_size = BuddyAlignUp(page_count * sizeof(Page), PAGE_SIZE);
    page_count -= map_size / PAGE_SIZE;
    mem += map_size;
    size -= map_size;

    // Alloc page
    USize mem_diff = (TypeCast(PtrDiff, BuddyAlignPtr(mem, PAGE_SIZE)) - TypeCast(PtrDiff, mem));
    mem += mem_diff;
    size -= mem_diff;
    page_count = size >> PAGE_SHIFT;

    system->pageStart = mem;
    system->maxPFN = page_count - 1;

    BUDDY_ASSERT(((USize)mem & PAGE_MASK) == 0, "must align to PAGE_SIZE");

    USize i;
    for (i = 0; i < page_count; i++)
    {
        system->map[i].order = PAGE_INVALID_ORDER;
    }

    int order = PAGE_INVALID_ORDER;

    USize count;
    for (i = 0, count = page_count; count; i += 1UL << order, count -= 1UL << order)
    {
        order = BuddyFlsSizet(count);
        ASSERT(order >= 0);

        if (order > MAX_PAGE_ORDER)
            order = MAX_PAGE_ORDER;

        Page* p = &system->map[i];
        ListInit(&p->list);
        p->order = order;
        BuddyAddPage(system, p);
    }

    return system;
}

PRIVATE Page* BuddyLocateFree(BuddySystem* system, int order)
{
    ASSERT(system && (order >= 0));
    ASSERT(ListEmpty(&system->pageBuddy[order]) == (system->count[order] == 0));

    if (ListEmpty(&system->pageBuddy[order]))
    {
        USize bitmap = system->bitmap & (~0UL << order);
        if (!bitmap)
        {
            LOG_E("Cannot find free page!");
            return NULL;
        }

        order = BuddyFfsSizet(bitmap);
        ASSERT(order >= 0);
    }

    return ListLastEntry(&system->pageBuddy[order], Page, list);
}

PRIVATE void PageSplit(BuddySystem* system, Page* page, int order)
{
    ASSERT(system && page && (order >= 0));

    int i;
    for (i = page->order; i > order; i--)
    {
        int new = i - 1;
        Page* buddy = &page[1UL << new];
        buddy->order = new;
        BuddyAddPage(system, buddy);
    }
    page->order = order;
}

PRIVATE Page* PageMerge(BuddySystem* system, Page* page)
{
    ASSERT(system && page);

    int order = page->order;
    USize pfn = PageToPFN(system, page);

    for (; order <= MAX_PAGE_ORDER;)
    {
        USize buddyPFN = pfn ^ (1UL << order);
        Page* buddy = page + (buddyPFN - pfn);

        if (!IsValidPFN(system, buddyPFN))
            break;
        if (buddy->order != order)
            break;
        if (!DoPageFree(buddy))
            break;

        BuddyDelPage(system, buddy);

        USize privatePFN = buddyPFN | pfn;
        Page* private = page + (privatePFN - pfn);
        private->order = PAGE_INVALID_ORDER;

        USize combinedPFN = buddyPFN & pfn;
        page = page + (combinedPFN - pfn);
        pfn = combinedPFN;
        order++;
    }

    page->order = order;
    return page;
}

PRIVATE void *PagePrepareUsed(BuddySystem* system, Page* page, int order)
{
    ASSERT(system && page && (order >= 0));

    BuddyDelPage(system, page);

    if (page->order > order)
    {
        PageSplit(system, page, order);
    }

    return PageToPtr(system, page);
}

PUBLIC void *BuddyAllocPage(BuddySystem* system, USize count)
{
    ASSERT(system && count);

    if (count == 0UL)
    {
        LOG_E("count == 0");
        return NULL;
    }

    int order = BuddyFlsSizet(count + ((1UL << BuddyFlsSizet(count)) - 1));

    Page* page = BuddyLocateFree(system, order);

    if (!page)
    {
        LOG_E("Failed to call BuddyLocateFree");
        return NULL;
    }
    return PagePrepareUsed(system, page, order);
}

PUBLIC void BuddyFreePage(BuddySystem* system, void *ptr)
{
    ASSERT(system && ptr);

    if (ptr)
    {
        Page* page = PageFromPtr(system, ptr);

        if (DoPageFree(page))
        {
            LOG_E("Double free!");
            return;
        }
        page = PageMerge(system, page);
        BuddyAddPage(system, page);
    }
}