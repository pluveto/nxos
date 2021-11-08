/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Alloc memory from heap
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-7      JasonHu           Init
 */

#ifndef __MM_ALLOC__
#define __MM_ALLOC__

#include <MM/HeapCache.h>

#define MemAlloc(size) HeapAlloc(size)
#define MemFree(ptr) HeapFree(ptr)
#define MemFreeSafety(ptr) HeapFreeSatety(ptr)

#endif /* __MM_ALLOC__ */
