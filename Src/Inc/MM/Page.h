/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Buddy system from Ginkgo OS
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-17     JasonHu           Init
 */

#ifndef __MM_PAGE__
#define __MM_PAGE__

#include <XBook.h>
#include <Page.h>   /* Platfrom page */

#ifndef CONFIG_PAGE_SHIFT
#define PAGE_SHIFT CONFIG_PAGE_SHIFT
#else
#define PAGE_SHIFT 12
#endif

#define PAGE_SIZE   (1U << PAGE_SHIFT)
#define PAGE_MASK   (PAGE_SIZE - 1UL)

#define PAGE_UMASK  (~PAGE_MASK)
#define PAGE_ADDR_MASK PAGE_UMASK

#define PAGE_ALIGNUP(value) (((value) + PAGE_MASK) & PAGE_UMASK)
#define PAGE_ALIGNDOWN(value) ((value) & PAGE_UMASK)

PUBLIC void PageInitZone(PageZone zone, void *mem, Size size);
PUBLIC void *PageAllocInZone(PageZone zone, Size count);
PUBLIC void PageFreeInZone(PageZone zone, void *ptr);
PUBLIC void *PageZoneGetBase(PageZone zone);
PUBLIC Size PageZoneGetPages(PageZone zone);

PUBLIC void *PageZoneGetBuddySystem(PageZone zone);

#define PageAlloc(size) PageAllocInZone(PZ_NORMAL, size)
#define PageFree(ptr) PageFreeInZone(PZ_NORMAL, ptr)

#define P2V(addr) ((addr) + CONFIG_KERNEL_VSTART)
#define V2P(addr) ((addr) - CONFIG_KERNEL_VSTART)

#endif /* __MM_PAGE__ */
