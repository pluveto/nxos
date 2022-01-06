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

#define NX_MemAlloc(size) NX_HeapAlloc(size)
#define NX_MemFree(ptr) NX_HeapFree(ptr)
#define NX_MemFreeSafety(ptr) NX_HeapFreeSatety(ptr)

#endif /* __MM_ALLOC__ */
