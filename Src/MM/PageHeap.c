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
#include <Mods/Console/Console.h>
#include <Utils/Memory.h>

PRIVATE PageHeap pageHeap;
PRIVATE SpanMark *spanMark;
PRIVATE void *spanBaseAddr;

PRIVATE void *PageAllocVirtual(Size count)
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
PRIVATE void MarkSpan(void *span, Size count)
{
    Addr dis = (Addr)span - (Addr)spanBaseAddr;
    Size idx = dis >> PAGE_SHIFT;

    SpanMark *mark = spanMark + idx;
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
PRIVATE void ClearSpan(void *span, Size count)
{
    Addr dis = (Addr)span - (Addr)spanBaseAddr;
    Size idx = dis >> PAGE_SHIFT;

    SpanMark *mark = spanMark + idx;

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
    Size dis = (Addr)page - (Addr)spanBaseAddr;
    Size idx = dis >> PAGE_SHIFT;

    SpanMark *mark = spanMark + idx;
    return (void *)((Addr)page - mark->idx * PAGE_SIZE);
}

PUBLIC Size PageToSpanCount(void *span)
{
    Size dis = (Addr)span - (Addr)spanBaseAddr;
    Size idx = dis >> PAGE_SHIFT;

    SpanMark *mark = spanMark + idx;
    return mark->count;
}

/**
 * alloc span from heap, if no free page, alloc from buddy system
 */
PUBLIC void *PageHeapAlloc(Size count)
{
    if (!count)
    {
        Cout("alloc page count is 0!" Endln);
        return NULL;
    }
    else if (count > PAGE_HEAP_MAX_PAGES)
    {
        Cout("alloc page count beyond " $d(PAGE_HEAP_MAX_PAGES) Endln);
        return NULL;
    }
    
    int isLargeSpan = 0;
    List *listHead;
    Span *spanNode = NULL;
    Span *spanNodeBest = NULL;

    if (count >= SMALL_SPAN_PAGES_MAX)    /* alloc from large list */
    {
        isLargeSpan = 1;
        listHead = &pageHeap.largeSpanFreeList;
    }
    else    /* alloc from normal list */
    {
        listHead = &pageHeap.spanFreeList[count];
    }

    if (ListEmpty(listHead))
    {
        /* alloc from buddy system */
        void *span = PageAllocVirtual(count);
        if (span == NULL)
        {
            Cout("no enough memroy to allocate for " $d(count) "pages!" Endln);
            return NULL;
        }
        MarkSpan(span, count);
        return span;
    }
    
    if (isLargeSpan)
    {
        /* use best fit to alloc a span */
        ListForEachOwner (spanNode, &pageHeap.largeSpanFreeList, list)
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
        spanNodeBest = ListFirstOwner(&pageHeap.spanFreeList[count], Span, list);
    }
    /* del span node from list */
    ListDelInit(&spanNodeBest->list);
    /* return base addr as spin */
    return (void *)spanNodeBest;
}

PUBLIC void PageHeapFree(void *page)
{
    if (page == NULL)
    {
        Cout("free NULL page!" Endln);
        return;
    }

    void *span = PageToSpan(page);
    Size count = PageToSpanCount(page);

    if (!count)
    {
        Cout("span count is 0!" Endln);
        return;
    }

    List *listHead;
    Size maxThresold = 0;

    if (count >= SMALL_SPAN_PAGES_MAX)    /* free to large list */
    {
        maxThresold = LARGE_SPAN_FREE_THRESHOLD_MAX;
        listHead = &pageHeap.largeSpanFreeList;
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
        listHead = &pageHeap.spanFreeList[count];
    }

    if (ListLength(listHead) >= maxThresold)    /* directly free */
    {
        ClearSpan(span, count);
        PageFreeVirtual(span);
    }
    else    /* add to list for cache */
    {
        Span *spanNode = (Span *)span;
        spanNode->pageCount = count;
        ListAdd(&spanNode->list, listHead);
    }
}

PRIVATE void PageHeapLarge(void)
{
    void *span = PageHeapAlloc(128);
    Cout("span: " $p(span) Endln);
    PageHeapFree(span);
    span = PageHeapAlloc(128);
    Cout("span: " $p(span) Endln);
    PageHeapFree(span);

    span = PageHeapAlloc(256);
    Cout("span: " $p(span) Endln);
    PageHeapFree(span + PAGE_SIZE);
    
    span = PageHeapAlloc(256);
    Cout("span: " $p(span) Endln);
    PageHeapFree(span + PAGE_SIZE);

    void *table[100];
    
    int i;
    for (i = 0; i < 100; i++)
    {
        table[i] = PageHeapAlloc(128 + i);
        Cout("alloc span: " $p(table[i]) Endln);
        Set(table[i], 0x5a, PAGE_SIZE * 128 + i);
    }

    for (i = 0; i < 100; i++)
    {
        Cout("free span: " $p(table[i]) Endln);
        PageHeapFree(table[i]);
    }
    
}

PRIVATE void PageHeapSmall(void)
{
    void *span = PageHeapAlloc(1);
    Cout("span: " $p(span) Endln);
    PageHeapFree(span);
    span = PageHeapAlloc(1);
    Cout("span: " $p(span) Endln);
    PageHeapFree(span);

    span = PageHeapAlloc(10);
    Cout("span: " $p(span) Endln);
    PageHeapFree(span);
    span = PageHeapAlloc(10);
    Cout("span: " $p(span) Endln);
    PageHeapFree(span);

    void *table[128];
    
    int i;
    for (i = 1; i <= 128; i++)
    {
        table[i] = PageHeapAlloc(i);
        Cout("alloc span: " $p(table[i]) Endln);
        Set(table[i], 0x5a, PAGE_SIZE * i);
    }

    for (i = 1; i <= 128; i++)
    {
        Cout("free span: " $p(table[i]) Endln);
        PageHeapFree(table[i]);
    }

    for (i = 1; i <= 128; i++)
    {
        table[i] = PageHeapAlloc(i);
        Cout("alloc span: " $p(table[i]) Endln);
        Set(table[i], 0x5a, PAGE_SIZE * i);
    }

    for (i = 1; i <= 128; i++)
    {
        Cout("free span: " $p(table[i]) Endln);
        PageHeapFree(table[i]);
    }
}

PRIVATE void PageHeapOnePage(void)
{
    void *table[1024];
    int i;
    for (i = 0; i < 1024; i++)
    {
        table[i] = PageHeapAlloc(1);
        Cout("alloc span: " $p(table[i]) Endln);
        Set(table[i], 0x5a, PAGE_SIZE);
    }

    for (i = 0; i < 1024; i++)
    {
        Cout("free span: " $p(table[i]) Endln);
        PageHeapFree(table[i]);
    }
    for (i = 0; i < 1024; i++)
    {
        table[i] = PageHeapAlloc(1);
        Cout("alloc span: " $p(table[i]) Endln);
        Set(table[i], 0x5a, PAGE_SIZE);
    }

    for (i = 0; i < 1024; i++)
    {
        Cout("free span: " $p(table[i]) Endln);
        PageHeapFree(table[i]);
    }
}

PRIVATE void PageHeapTest(void)
{
    Cout("PageHeap test" Endln);

    PageHeapLarge();
    PageHeapSmall();
    PageHeapOnePage();
}

PUBLIC void PageHeapInit(void)
{
    int i;
    for (i = 0; i < sizeof(pageHeap.spanFreeList) / sizeof(pageHeap.spanFreeList[0]); i++)
    {
        ListInit(&pageHeap.spanFreeList[i]);
    }
    ListInit(&pageHeap.largeSpanFreeList);
    spanBaseAddr = PageZoneGetBase(PZ_NORMAL);
    Cout("span base addr:" $p(spanBaseAddr) Endln);

    Size pages = PageZoneGetPages(PZ_NORMAL);
    /* alloc span mark array */
    Size spanMarkPages = DIV_ROUND_UP(pages * sizeof(SpanMark), PAGE_SIZE);
    Cout("span mark used page:" $d(spanMarkPages) Endln);

    spanMark = PageAllocVirtual(spanMarkPages);
    if (spanMark == NULL)
    {
        PANIC("alloc page for span mark failed!");
    }
    Zero(spanMark, spanMarkPages * PAGE_SIZE);

    // PageHeapTest();
}
