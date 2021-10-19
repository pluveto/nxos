/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Platfrom header
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-18     JasonHu           Init
 */

#ifndef __PLATFORM_I386__
#define __PLATFORM_I386__

#include <XBook.h>

#define MEM_KERNEL_BASE 0x00000000
#define MEM_KERNEL_SZ (8 * SZ_MB)

#define MEM_DMA_BASE (MEM_KERNEL_BASE + MEM_KERNEL_SZ)
#define MEM_DMA_SIZE (8 * SZ_MB)

#define MEM_NORMAL_BASE (MEM_DMA_BASE + MEM_DMA_SIZE)

#endif /* __PLATFORM_I386__ */
