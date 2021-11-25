/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Heap Cache
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-25     JasonHu           Update code style
 */

#include <MM/HeapCache.h>
#include <Utils/Math.h>
#include <MM/PageHeap.h>
#include <MM/Buddy.h>
#include <MM/Page.h>
#include <Sched/Mutex.h>

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "HeapCache"
#include <Utils/Debug.h>

PRIVATE struct SizeClass CacheSizeAarray[MAX_SIZE_CLASS_NR];
PRIVATE HeapCache MiddleSizeCache;
PRIVATE Mutex HeapCacheLock;

PRIVATE Size AlignDownToPow2(Size size)
{
    Size n = 19;    /* pow(2, 19) -> 512kb */
    while (PowInt(2, n) > size)
    {
        n--;
    }
    return PowInt(2, n);
}

PRIVATE Size SizeToPageCount(Size size)
{
    if (size > 0 && size <= 1024)
    {
        return 1;
    }
    else if (size > 1024 && size <= 8192)
    {
        return 4;
    }
    else if (size > 8192 && size <= 36864)
    {
        return 18;
    }
    else if (size > 36864 && size <= 104096)
    {
        return 52;
    }
    else if (size > 104096 && size <= 262144)
    {
        return 128;
    }
    else
    {
        return DIV_ROUND_UP(size, PAGE_SIZE);
    }
}

PRIVATE void HeapCacheInitOne(HeapCache *cache, Size classSize)
{
    cache->classSize = classSize;
    ListInit(&cache->objectFreeList);
    ListInit(&cache->spanFreeList);
}

PRIVATE void HeapSizeClassInit(void)
{
    int n = 0;

    int i;
    for (i = 8; i <= 16; i+=8)
    {
        CacheSizeAarray[n].size = i;
        n++;
    }
    for (i = 32; i <= 128; i+=16)
    {
        CacheSizeAarray[n].size = i;
        n++;
    }
    for (i = 128 + 128 / 8; i <= MAX_SMALL_OBJECT_SIZE; )
    {
        CacheSizeAarray[n].size = i;
        i += AlignDownToPow2(i / 8);
        n++;
    }

    for (i = 0; i < MAX_SIZE_CLASS_NR; i++)
    {
        HeapCacheInitOne(&CacheSizeAarray[i].cache, CacheSizeAarray[i].size);
    }

    HeapCacheInitOne(&MiddleSizeCache, 0);
}

INLINE HeapCache *SizeToCache(Size size)
{
    ASSERT(size <= MAX_SMALL_OBJECT_SIZE);
    HeapCache *cache = NULL;
    U32 index = 0;

    for (; index < MAX_SIZE_CLASS_NR; index++)
    {
        if (CacheSizeAarray[index].size >= size)
        {
            cache = &CacheSizeAarray[index].cache;
            /* alloc in this cache */
            break;
        }
    }
    return cache;
}

PRIVATE void *DoHeapAlloc(Size size)
{
    /* size align up with 8 */
    size = ALIGN_UP(size, 8);

    Span *span = NULL;
    Size pageCount = SizeToPageCount(size);
    HeapCache *cache = NULL;
    SmallCacheObject *objectNode = NULL;

    if (size > MAX_SMALL_OBJECT_SIZE)   /* alloc big span */
    {
        if (size > MAX_MIDDLE_OBJECT_SIZE) /* alloc directly from span */
        {
            return PageHeapAlloc(DIV_ROUND_UP(size, PAGE_SIZE));
        }
        else    /* alloc from middle cache */
        {
            cache = &MiddleSizeCache;
        }
    }
    else
    {
        cache = SizeToCache(size);
    }

    if (ListEmpty(&cache->objectFreeList)) /* no object, need split from span */
    {
        Size objectCount;
        
        if (ListEmpty(&cache->spanFreeList))    /* no span, need alloc from page heap */
        {
            span = (Span *)PageHeapAlloc(pageCount);
            if (span == NULL)
            {
                LOG_E("no enough memory span!");
                return NULL;
            }
        }
        else    /* get span from span list */
        {
            span = ListFirstEntry(&cache->spanFreeList, Span, list);
            ListDelInit(&span->list);            
        }

        if (cache == &MiddleSizeCache)  /* return span if middle cache */
        {
            return (void *)span;
        }

        /* split to object free list */
        /* NOTICE: must get page node from span page */
        Page *pageNode = PageFromPtr(PageZoneGetBuddySystem(PZ_NORMAL), span);
        ASSERT(pageNode != NULL);

        objectCount = DIV_ROUND_DOWN(pageCount * PAGE_SIZE, size); 

        /* mark size class on the span */
        pageNode->sizeClass = size;
        pageNode->maxObjectsOnSpan = objectCount;

        /* split span to object */
        U8 *start = (U8 *)span;
        U32 i;
        for (i = 0; i < objectCount; i++)
        {
            objectNode = (SmallCacheObject *)start;
            ListAddTail(&objectNode->list, &cache->objectFreeList);
            start += size;
        }
    }
    /* get a object from list */
    objectNode = ListFirstEntry(&cache->objectFreeList, SmallCacheObject, list);
    ListDel(&objectNode->list); /* del from free list */
    return (void *)objectNode;
}

PUBLIC void *HeapAlloc(Size size)
{
    if (!size)
    {
        return NULL;
    }
    MutexLock(&HeapCacheLock, TRUE);
    void *ptr = DoHeapAlloc(size);
    MutexUnlock(&HeapCacheLock);
    return ptr;
}

PRIVATE OS_Error DoHeapFree(void *object)
{
    if (object == NULL) /* can't free NULL object */
    {
        return OS_EINVAL;
    }
    
    /* object to page, then to span */
    void *page = (void *)(((Addr) object) & PAGE_UMASK);
    void *span = PageToSpan(page);
    Size pageCount = PageToSpanCount(page);
    Size size = pageCount * PAGE_SIZE;

    /* according size to free */
    if (size > MAX_SMALL_OBJECT_SIZE)   /* free to big span */
    {
        if (size > MAX_MIDDLE_OBJECT_SIZE) /* free directly to span */
        {
            return PageHeapFree(span);
        }
        else    /* free to middle cache */
        {
            /* if len is too long, free to page heap */
            if (ListLength(&MiddleSizeCache.spanFreeList) + 1 >= MAX_MIDDLE_OBJECT_THRESOLD) 
            {
                return PageHeapFree(span);
            }
            else    /* free to span free list */
            {
                Span *spanNode = (Span *) span;
                ListAdd(&spanNode->list, &MiddleSizeCache.spanFreeList);
            }
            return OS_EOK;
        }
    }
    else    /* free to small cache */
    {
        /* NOTICE: must get page node from span page */
        Page *pageNode = PageFromPtr(PageZoneGetBuddySystem(PZ_NORMAL), span);
        ASSERT(pageNode != NULL);

        /* get class size from page */
        Size sizeClass = pageNode->sizeClass;
        /* get left objects */
        HeapCache *cache = SizeToCache(sizeClass);
 
        /* if objects in span is full, free all objects */
        if (ListLength(&cache->objectFreeList) + 1 >= pageNode->maxObjectsOnSpan)
        {
            /* empty object free list */
            ListInit(&cache->objectFreeList);

            /* free span to page heap */
            if (ListLength(&cache->spanFreeList) + 1 >= MAX_SMALL_SPAN_THRESOLD)
            {
                return PageHeapFree(span);
            }
            else    /* add span to span list */
            {
                Span *spanNode = (Span *) span;
                ListAdd(&spanNode->list, &cache->spanFreeList);
            }
        }
        else    /* free to object free list */
        {
            SmallCacheObject *objectNode = (SmallCacheObject *)object;
            ListAdd(&objectNode->list, &cache->objectFreeList);
        }
    }
    return OS_EOK;
}

PUBLIC OS_Error HeapFree(void *object)
{
    if (object == NULL) /* can't free NULL object */
    {
        return OS_EINVAL;
    }
    MutexLock(&HeapCacheLock, TRUE);
    OS_Error err = DoHeapFree(object);
    MutexUnlock(&HeapCacheLock);
    return err;
}

PUBLIC Size HeapGetObjectSize(void *object)
{
    if (object == NULL) /* can't free NULL object */
    {
        return 0;
    }
    MutexLock(&HeapCacheLock, TRUE);
    /* object to page, then to span */
    void *page = (void *)(((Addr) object) & PAGE_UMASK);
    void *span = PageToSpan(page);
    /* NOTICE: page must be span page */
    Page *pageNode = PageFromPtr(PageZoneGetBuddySystem(PZ_NORMAL), span);
    ASSERT(pageNode != NULL);
    MutexUnlock(&HeapCacheLock);
    /* get class size from page */
    return pageNode->sizeClass;
}

PUBLIC void HeapCacheInit(void)
{
    HeapSizeClassInit();
    MutexInit(&HeapCacheLock);
}
