/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Page heap alloctor
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-24     JasonHu           Init
 */

#include <MM/Page.h>
#include <MM/PageHeap.h>
#include <MM/Buddy.h>
#include <Utils/Memory.h>
#include <Sched/Mutex.h>

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "PageHeap"
#include <Utils/Debug.h>

PRIVATE PageHeap PageHeapObject;
PRIVATE SpanMark *SpanMarkMap;
PRIVATE void *SpanBaseAddr;
PRIVATE Mutex PageHeapLock;

PRIVATE void *PageAllocVirtual(USize count)
{
    void *ptr = PageAlloc(count);
    if (ptr == NULL)
    {
        return NULL;
    }
    return P2V(ptr);
}

PRIVATE void PageFreeVirtual(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    PageFree(V2P(ptr));
}

/**
 * mark span with count
 */
PRIVATE void MarkSpan(void *span, USize count)
{
    Addr dis = (Addr)span - (Addr)SpanBaseAddr;
    USize idx = dis >> PAGE_SHIFT;

    SpanMark *mark = SpanMarkMap + idx;
    int i;
    for (i = 0; i < count; i++)
    {
        mark->idx = i;
        mark->count = count;
        mark++;
    }
}

/**
 * clear span with count
 */
PRIVATE void ClearSpan(void *span, USize count)
{
    Addr dis = (Addr)span - (Addr)SpanBaseAddr;
    USize idx = dis >> PAGE_SHIFT;

    SpanMark *mark = SpanMarkMap + idx;

    int i;
    for (i = 0; i < count; i++)
    {
        mark->idx = 0;
        mark->count = 0;
        mark++;
    }
}

PUBLIC void *PageToSpan(void *page)
{
    USize dis = (Addr)page - (Addr)SpanBaseAddr;
    USize idx = dis >> PAGE_SHIFT;

    SpanMark *mark = SpanMarkMap + idx;
    return (void *)((Addr)page - mark->idx * PAGE_SIZE);
}

PUBLIC USize PageToSpanCount(void *span)
{
    USize dis = (Addr)span - (Addr)SpanBaseAddr;
    USize idx = dis >> PAGE_SHIFT;

    SpanMark *mark = SpanMarkMap + idx;
    return mark->count;
}

PRIVATE void *DoPageHeapAlloc(USize count)
{
    int isLargeSpan = 0;
    List *listHead;
    Span *spanNode = NULL;
    Span *spanNodeBest = NULL;
    Atomic *freeCount = NULL;

    if (count >= SMALL_SPAN_PAGES_MAX)    /* alloc from large list */
    {
        isLargeSpan = 1;
        listHead = &PageHeapObject.largeSpanFreeList;
        freeCount = &PageHeapObject.largeSpanFreeCount;
    }
    else    /* alloc from normal list */
    {
        listHead = &PageHeapObject.spanFreeList[count];
        freeCount = &PageHeapObject.spanFreeCount[count];
    }

    if (ListEmpty(listHead))
    {
        /* alloc from buddy system */
        void *span = PageAllocVirtual(count);
        if (span == NULL)
        {
            LOG_E("no enough memroy to allocate for %d pages!", count);
            return NULL;
        }
        MarkSpan(span, count);
        return span;
    }
    
    if (isLargeSpan)
    {
        /* use best fit to alloc a span */
        ListForEachEntry (spanNode, &PageHeapObject.largeSpanFreeList, list)
        {
            if (spanNodeBest == NULL)
            {
                spanNodeBest = spanNode;
            }
            if (spanNode != spanNodeBest && spanNode->pageCount < spanNodeBest->pageCount)
            {
                spanNodeBest = spanNode;
            }
        }
    }
    else
    {
        /* use first fit to alloc a span */
        spanNodeBest = ListFirstEntry(&PageHeapObject.spanFreeList[count], Span, list);
    }
    /* del span node from list */
    ListDelInit(&spanNodeBest->list);
    AtomicDec(freeCount);

    /* return base addr as spin */
    return (void *)spanNodeBest;
}

/**
 * alloc span from heap, if no free page, alloc from buddy system
 */
PUBLIC void *PageHeapAlloc(USize count)
{
    if (!count)
    {
        LOG_E("alloc page count is 0!");
        return NULL;
    }
    else if (count > PAGE_HEAP_MAX_PAGES)
    {
        LOG_E("alloc page count beyond %d", PAGE_HEAP_MAX_PAGES);
        return NULL;
    }
    
    MutexLock(&PageHeapLock, TRUE);
    void *ptr = DoPageHeapAlloc(count);
    MutexUnlock(&PageHeapLock);
    return ptr;
}

PRIVATE OS_Error DoPageHeapFree(void *page)
{
    void *span = PageToSpan(page);
    USize count = PageToSpanCount(page);
    Atomic *freeCount = NULL;

    if (!count)
    {
        LOG_E("span count is 0!");
        return OS_EPERM;
    }

    List *listHead;
    USize maxThresold = 0;

    if (count >= SMALL_SPAN_PAGES_MAX)    /* free to large list */
    {
        maxThresold = LARGE_SPAN_FREE_THRESHOLD_MAX;
        listHead = &PageHeapObject.largeSpanFreeList;
        freeCount = &PageHeapObject.largeSpanFreeCount;
    }
    else    /* free from normal list */
    {
        if (count == 1) /* one page special */
        {
            maxThresold = ONE_PAGE_SPAN_FREE_THRESHOLD_MAX;
        }
        else
        {
            maxThresold = SMALL_SPAN_FREE_THRESHOLD_MAX;
        }
        listHead = &PageHeapObject.spanFreeList[count];
        freeCount = &PageHeapObject.spanFreeCount[count];
    }

    if (AtomicGet(freeCount) >= maxThresold)    /* directly free */
    {
        ClearSpan(span, count);
        PageFreeVirtual(span);
    }
    else    /* add to list for cache */
    {
        Span *spanNode = (Span *)span;
        spanNode->pageCount = count;
        ListAdd(&spanNode->list, listHead);
        AtomicInc(freeCount);
    }
    return OS_EOK;
}

PUBLIC OS_Error PageHeapFree(void *page)
{
    if (page == NULL)
    {
        LOG_E("free NULL page!");
        return OS_EINVAL;
    }
    
    MutexLock(&PageHeapLock, TRUE);
    OS_Error err = DoPageHeapFree(page);
    MutexUnlock(&PageHeapLock);
    return err;
}

PUBLIC void PageHeapInit(void)
{
    int i;
    for (i = 0; i < sizeof(PageHeapObject.spanFreeList) / sizeof(PageHeapObject.spanFreeList[0]); i++)
    {
        ListInit(&PageHeapObject.spanFreeList[i]);
        AtomicSet(&PageHeapObject.spanFreeCount[i], 0);
    }
    ListInit(&PageHeapObject.largeSpanFreeList);
    AtomicSet(&PageHeapObject.largeSpanFreeCount, 0);

    SpanBaseAddr = PageZoneGetBase(PZ_NORMAL);
    LOG_I("span base addr: %p", SpanBaseAddr);

    USize pages = PageZoneGetPages(PZ_NORMAL);
    /* alloc span mark array */
    USize spanMarkPages = DIV_ROUND_UP(pages * sizeof(SpanMark), PAGE_SIZE);
    LOG_I("span mark used page: %d", spanMarkPages);

    SpanMarkMap = PageAllocVirtual(spanMarkPages);
    if (SpanMarkMap == NULL)
    {
        PANIC("alloc page for span mark failed!");
    }
    Zero(SpanMarkMap, spanMarkPages * PAGE_SIZE);

    MutexInit(&PageHeapLock);
}
