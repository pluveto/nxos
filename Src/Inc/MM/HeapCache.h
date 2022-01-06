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

struct NX_HeapCache
{
    NX_List spanFreeList;
    NX_List objectFreeList;
    NX_USize classSize; /* heap cache size */
    NX_Atomic spanFreeCount;
    NX_Atomic objectFreeCount;
};
typedef struct NX_HeapCache NX_HeapCache;

struct NX_SizeClass
{
    NX_USize size;
    struct NX_HeapCache cache;
};

/* small object struct */
struct NX_SmallCacheObject
{
    NX_List list;
};
typedef struct NX_SmallCacheObject NX_SmallCacheObject;

NX_PUBLIC void NX_HeapCacheInit(void);

NX_PUBLIC void *NX_HeapAlloc(NX_USize size);
NX_PUBLIC NX_Error NX_HeapFree(void *object);
NX_PUBLIC NX_USize NX_HeapGetObjectSize(void *object);

NX_INLINE NX_Error __HeapFreeSatety(void **object)
{
    NX_Error err = NX_HeapFree(*object);
    if (err == NX_EOK)
    {
        *object = NX_NULL;
    }
    return err;
}

#define NX_HeapFreeSatety(p) __HeapFreeSatety((void **)(&(p)))

#endif /* __MM_HEAP_CACHE__ */
