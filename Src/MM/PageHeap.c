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

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "PageHeap"
#include <Utils/Debug.h>

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
        LOG_E("alloc page count is 0!");
        return NULL;
    }
    else if (count > PAGE_HEAP_MAX_PAGES)
    {
        LOG_E("alloc page count beyond " $d(PAGE_HEAP_MAX_PAGES));
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
            LOG_E("no enough memroy to allocate for " $d(count) "pages!");
            return NULL;
        }
        MarkSpan(span, count);
        return span;
    }
    
    if (isLargeSpan)
    {
        /* use best fit to alloc a span */
        ListForEachEntry (spanNode, &pageHeap.largeSpanFreeList, list)
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
        spanNodeBest = ListFirstEntry(&pageHeap.spanFreeList[count], Span, list);
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
        LOG_E("free NULL page!");
        return;
    }

    void *span = PageToSpan(page);
    Size count = PageToSpanCount(page);

    if (!count)
    {
        LOG_E("span count is 0!");
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

#ifdef CONFIG_PAGE_HEAP_TEST
PRIVATE void PageHeapLarge(void)
{
    void *span = PageHeapAlloc(128);
    LOG_D("span: " $p(span));
    PageHeapFree(span);
    span = PageHeapAlloc(128);
    LOG_D("span: " $p(span));
    PageHeapFree(span);

    span = PageHeapAlloc(256);
    LOG_D("span: " $p(span));
    PageHeapFree(span + PAGE_SIZE);
    
    span = PageHeapAlloc(256);
    LOG_D("span: " $p(span));
    PageHeapFree(span + PAGE_SIZE);

    void *table[100];
    
    int i;
    for (i = 0; i < 100; i++)
    {
        table[i] = PageHeapAlloc(128 + i);
        LOG_D("alloc span: " $p(table[i]));
        Set(table[i], 0x5a, PAGE_SIZE * 128 + i);
    }

    for (i = 0; i < 100; i++)
    {
        LOG_D("free span: " $p(table[i]));
        PageHeapFree(table[i]);
    }
    
}

PRIVATE void PageHeapSmall(void)
{
    void *span = PageHeapAlloc(1);
    LOG_D("span: " $p(span));
    PageHeapFree(span);
    span = PageHeapAlloc(1);
    LOG_D("span: " $p(span));
    PageHeapFree(span);

    span = PageHeapAlloc(10);
    LOG_D("span: " $p(span));
    PageHeapFree(span);
    span = PageHeapAlloc(10);
    LOG_D("span: " $p(span));
    PageHeapFree(span);

    void *table[128];
    
    int i;
    for (i = 1; i <= 128; i++)
    {
        table[i] = PageHeapAlloc(i);
        LOG_D("alloc span: " $p(table[i]));
        Set(table[i], 0x5a, PAGE_SIZE * i);
    }

    for (i = 1; i <= 128; i++)
    {
        LOG_D("free span: " $p(table[i]));
        PageHeapFree(table[i]);
    }

    for (i = 1; i <= 128; i++)
    {
        table[i] = PageHeapAlloc(i);
        LOG_D("alloc span: " $p(table[i]));
        Set(table[i], 0x5a, PAGE_SIZE * i);
    }

    for (i = 1; i <= 128; i++)
    {
        LOG_D("free span: " $p(table[i]));
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
        LOG_D("alloc span: " $p(table[i]));
        Set(table[i], 0x5a, PAGE_SIZE);
    }

    for (i = 0; i < 1024; i++)
    {
        LOG_D("free span: " $p(table[i]));
        PageHeapFree(table[i]);
    }
    for (i = 0; i < 1024; i++)
    {
        table[i] = PageHeapAlloc(1);
        LOG_D("alloc span: " $p(table[i]));
        Set(table[i], 0x5a, PAGE_SIZE);
    }

    for (i = 0; i < 1024; i++)
    {
        LOG_D("free span: " $p(table[i]));
        PageHeapFree(table[i]);
    }
}

PRIVATE void PageHeapTest(void)
{
    LOG_D("PageHeap test");

    PageHeapLarge();
    PageHeapSmall();
    PageHeapOnePage();
}
#endif /* CONFIG_PAGE_HEAP_TEST */

PUBLIC void PageHeapInit(void)
{
    int i;
    for (i = 0; i < sizeof(pageHeap.spanFreeList) / sizeof(pageHeap.spanFreeList[0]); i++)
    {
        ListInit(&pageHeap.spanFreeList[i]);
    }
    ListInit(&pageHeap.largeSpanFreeList);
    spanBaseAddr = PageZoneGetBase(PZ_NORMAL);
    LOG_I("span base addr:" $p(spanBaseAddr));

    Size pages = PageZoneGetPages(PZ_NORMAL);
    /* alloc span mark array */
    Size spanMarkPages = DIV_ROUND_UP(pages * sizeof(SpanMark), PAGE_SIZE);
    LOG_I("span mark used page:" $d(spanMarkPages));

    spanMark = PageAllocVirtual(spanMarkPages);
    if (spanMark == NULL)
    {
        PANIC("alloc page for span mark failed!");
    }
    Zero(spanMark, spanMarkPages * PAGE_SIZE);

#ifdef CONFIG_PAGE_HEAP_TEST
    PageHeapTest();
#endif /* CONFIG_PAGE_HEAP_TEST */
}
