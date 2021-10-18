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

PUBLIC void PageInitZone(PageZone zone, void *mem, Size size);
PUBLIC void *PageAllocInZone(PageZone zone, Size count);
PUBLIC void PageFreeInZone(PageZone zone, void *ptr);

#define PageAlloc(size) PageAllocInZone(PZ_NORMAL, size)
#define PageFree(ptr) PageFreeInZone(PZ_NORMAL, ptr)

#endif /* __MM_PAGE__ */
