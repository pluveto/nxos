/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Page heap manage
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-24     JasonHu           Init
 */

#ifndef __MM_PAGE_HEAP__
#define __MM_PAGE_HEAP__

#include <Utils/List.h>
#include <XBook/Atomic.h>

struct NX_PageSpan
{
    NX_List list;  /* NOTICE: list must be first member here */
    NX_USize pageCount; /* page count for this span */
};
typedef struct NX_PageSpan NX_PageSpan;

NX_PUBLIC void NX_PageHeapInit(void);
NX_PUBLIC void *NX_PageHeapAlloc(NX_USize count);
NX_PUBLIC NX_Error NX_PageHeapFree(void *page);

NX_PUBLIC void *NX_PageToSpan(void *page);
NX_PUBLIC NX_USize NX_PageToSpanCount(void *span);

#endif /* __MM_PAGE_HEAP__ */
