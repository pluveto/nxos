/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Heap cache
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-25     JasonHu           Init
 */

#ifndef __MM_HEAP_CACHE__
#define __MM_HEAP_CACHE__

#include <XBook.h>
#include <XBook/Atomic.h>
#include <Utils/List.h>

/**
 * size class => span pages
 * ----------------------------
 * 8~1024 => 1 pages
 * 1024~8192 => 4 pages
 * 8192~36864 => 18 pages
 * 36864~104096 => 52 pages
 * 104096~262144 => 128 pages
 * N pages => N pages
 */

/**
 * size class:
 * ----------------------------
 * step 8B: 8, 16  
 * step 16B: 32, 48, 64, 80, 96, 112, 128
 * step M -> AlignDown((N / 8), 2^n)
 */

#define MAX_SIZE_CLASS_NR 97

#define MAX_SMALL_OBJECT_SIZE (256 * SZ_KB)
#define MAX_SMALL_SPAN_THRESOLD 8

#define MAX_MIDDLE_OBJECT_SIZE (1 * SZ_MB)
#define MAX_MIDDLE_OBJECT_THRESOLD 32

struct HeapCache
{
    List spanFreeList;
    List objectFreeList;
    USize classSize; /* heap cache size */
    Atomic spanFreeCount;
    Atomic objectFreeCount;
};
typedef struct HeapCache HeapCache;

struct SizeClass
{
    USize size;
    struct HeapCache cache;
};

/* small object struct */
struct SmallCacheObject
{
    List list;
};
typedef struct SmallCacheObject SmallCacheObject;

PUBLIC void HeapCacheInit(void);

PUBLIC void *HeapAlloc(USize size);
PUBLIC OS_Error HeapFree(void *object);
PUBLIC USize HeapGetObjectSize(void *object);

INLINE OS_Error __HeapFreeSatety(void **object)
{
    OS_Error err = HeapFree(*object);
    if (err == OS_EOK)
    {
        *object = NULL;
    }
    return err;
}

#define HeapFreeSatety(p) __HeapFreeSatety((void **)(&(p)))

#endif /* __MM_HEAP_CACHE__ */
