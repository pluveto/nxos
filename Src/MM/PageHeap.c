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

PUBLIC void *SpanToPage(void *span)
{
    Size dis = (Addr)span - (Addr)spanBaseAddr;
    Size idx = dis >> PAGE_SHIFT;

    SpanMark *mark = spanMark + idx;
    return (void *)((Addr)span - mark->idx * PAGE_SIZE);
}

PUBLIC Size SpanToPageCount(void *span)
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
    
    if (count > PAGE_HEAP_MAX_PAGES)
    {
        Cout("alloc page count beyond " $d(PAGE_HEAP_MAX_PAGES) Endln);
        return NULL;
    }
    
    if (count >= SMALL_SPAN_PAGES_MAX)    /* alloc from large list */
    {
        if (ListEmpty(&pageHeap.largeSpanFreeList))
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
        else
        {
            /* use best fit to alloc a span */
            Span *spanNodeBest = NULL;
            
            Span *spanNode;
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
            /* del span node from list */
            ListDelInit(&spanNodeBest->list);

            /* return base addr as spin */
            return (void *)spanNodeBest;
        }
    }
    else    /* alloc from normal list */
    {
        if (ListEmpty(&pageHeap.spanFreeList[count]))
        {
            /* alloc from big free list first  */
            void *span = PageAllocVirtual(count);
            if (span == NULL)
            {
                Cout("no enough memroy to allocate for " $d(count) "pages!" Endln);
                return NULL;
            }
            MarkSpan(span, count);
            return span;
        }
        else
        {
            /* use first fit to alloc a span */
            Span *spanNodeFirst = ListFirstOwner(&pageHeap.spanFreeList[count], Span, list);
            ListDelInit(&spanNodeFirst->list);

            /* return base addr as spin */
            return (void *)spanNodeFirst;
        }
    }
    return NULL;
}

PUBLIC void PageHeapFree(void *span)
{
    if (span == NULL)
    {
        Cout("free NULL span!" Endln);
        return;
    }
    /* find ptr in system */
    void *page = SpanToPage(span);
    Size count = SpanToPageCount(span);

    if (!count)
    {
        Cout("span count is 0!" Endln);
        return;
    }

    if (count >= SMALL_SPAN_PAGES_MAX)    /* free to large list */
    {
        /* large list too long, free to buddy system directly */
        if (ListLength(&pageHeap.largeSpanFreeList) >= LARGE_SPAN_FREE_THRESHOLD_MAX)
        {
            ClearSpan(page, count);
            PageFreeVirtual(page);
        }
        else
        {
            /* add to large list */
            Span *spanNode = (Span *)page;
            spanNode->pageCount = count;
            ListAdd(&spanNode->list, &pageHeap.largeSpanFreeList);
        }
    }
    else    /* free from normal list */
    {
        /* free list too long, free to buddy system directly */
        if (ListLength(&pageHeap.spanFreeList[count]) >= SMALL_SPAN_FREE_THRESHOLD_MAX)
        {
            ClearSpan(page, count);
            PageFreeVirtual(page);
        }
        else
        {
            /* add to free list */
            Span *spanNode = (Span *)page;
            spanNode->pageCount = count;
            ListAdd(&spanNode->list, &pageHeap.spanFreeList[count]);
        }
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

PRIVATE void PageHeapTest(void)
{
    Cout("PageHeap test" Endln);

    PageHeapLarge();
    PageHeapSmall();
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

    PageHeapTest();
}
