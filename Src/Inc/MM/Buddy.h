/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Buddy system
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-17     JasonHu           Init
 */

#ifndef __MM_BUDDY__
#define __MM_BUDDY__

#include <XBook.h>
#include <Utils/List.h>
#include <MM/Page.h>

#define MAX_PAGE_ORDER 20

#define PAGE_SHIFT_TO_MASK(s) ((1UL << s) - 1)
#define PAGE_ORDER_MASK (PAGE_SHIFT_TO_MASK((MAX_PAGE_ORDER + PAGE_SHIFT)) - PAGE_MASK)
#define PAGE_INVALID_ORDER (-1)

/* page flags */
#define PAGE_FLAG_IS_FREE  0x01

struct Page
{
    List list;
    U32 flags;
    I32 order;
    USize sizeClass;         /* size class on this span */
    USize maxObjectsOnSpan;  /* max memory objects on this span */
};
typedef struct Page Page;

struct BuddySystem
{
    List pageBuddy[MAX_PAGE_ORDER + 1];
    USize count[MAX_PAGE_ORDER + 1];
    USize bitmap;    /* map order has free page */
    void *pageStart;    /* page start addr */
    USize maxPFN;
    Page map[0];    /* pages array */
};
typedef struct BuddySystem BuddySystem;

PUBLIC BuddySystem* BuddyCreate(void *mem, USize size);
PUBLIC void *BuddyAllocPage(BuddySystem* system, USize count);
PUBLIC void BuddyFreePage(BuddySystem* system, void *ptr);

PUBLIC Page* PageFromPtr(BuddySystem* system, void *ptr);

#endif /* __MM_BUDDY__ */
