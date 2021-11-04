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

/**
 * Maximum number of pages in small span
 */
#define SMALL_SPAN_PAGES_MAX  128

/**
 * max allowed pages to allocate by system.
 */
#define PAGE_HEAP_MAX_PAGES  2048

/**
 * max threshold for free large span.
 * if free large list span count beyond thresold,
 * free span will directly free to buddy system.
 */
#define LARGE_SPAN_FREE_THRESHOLD_MAX  32

/**
 * max threshold for free small span.
 * if free small list span count beyond thresold,
 * free span will directly free to buddy system.
 */
#define SMALL_SPAN_FREE_THRESHOLD_MAX  64

/**
 * max threshold for free one page span.
 * one page span will used more 
 */
#define ONE_PAGE_SPAN_FREE_THRESHOLD_MAX  1024

/**
 * span mark use to covert page to span
 */
struct SpanMark
{
    U16 count;  /* span page count */
    U16 idx;    /* page idx int span */
};
typedef struct SpanMark SpanMark;

struct Span
{
    List list;  /* NOTICE: list must be first member here */
    Size pageCount; /* page count for this span */
};
typedef struct Span Span;

struct PageHeap
{
    List spanFreeList[SMALL_SPAN_PAGES_MAX];  /* list head for span list */
    List largeSpanFreeList;     /* list for large span page */
};
typedef struct PageHeap PageHeap;

PUBLIC void PageHeapInit(void);
PUBLIC void *PageHeapAlloc(Size count);
PUBLIC OS_Error PageHeapFree(void *page);

PUBLIC void *PageToSpan(void *page);
PUBLIC Size PageToSpanCount(void *span);

#endif /* __MM_PAGE_HEAP__ */
