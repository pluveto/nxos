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

/* Minim physical memory size */
#define MEM_MIN_SIZE (510 * SZ_MB)

/* Minim physical memory size */
#define MEM_KERNEL_TOP (512 * SZ_MB)

/**
 * Physical memory layout:
 *
 * +------------------------+ <- MAX PHY SIZE (TOP to 4GB)
 * | L3 PAGES               |
 * +------------------------+ <- KERNEL PAGES (TOP to 512MB)
 * | L3(K) & L1 & L2  PAGES |
 * +------------------------+ <- 0x01000000 (16MB)
 * | DMA                    |
 * +------------------------+ <- 0x00800000 (8MB)
 * | KERNEL                 |
 * +------------------------+ <- 0x00000000
 */

/**
 * Virtual memory layout:
 * 
 * @: delay map
 * 
 * +------------------------+ <- 0xFFFFFFFF (4GB)
 * | @USER                  |
 * +------------------------+ <- 0x20000000 (512MB)
 * | @KMAP                  |
 * +------------------------+ <- 0x10000000 (256MB)
 * | KHEAP                  |
 * +------------------------+ <- 0x01000000 (16MB)
 * | DMA                    |
 * +------------------------+ <- 0x00800000 (8MB)
 * | KCODE & KDATA & KBSS   |
 * +------------------------+ <- 0x00000000
 */

#endif /* __PLATFORM_I386__ */
