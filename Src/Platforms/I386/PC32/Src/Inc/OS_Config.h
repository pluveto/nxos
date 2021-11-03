/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Configure for each platform
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-17      JasonHu           Init
 */

#ifndef __OS_CONFIG__
#define __OS_CONFIG__

#define CONFIG_LOG_LEVEL LOG_DBG

/* OS normal config */
#define CONFIG_TICKS_PER_SECOND 100
#define CONFIG_IRQ_NAME_LEN 48

/* kernel virtual start addr */
#define CONFIG_KERNEL_VSTART 0x0000000

/* page shift */
#define CONFIG_PAGE_SHIFT 12

/* OS debug config */
#define CONFIG_DEBUG
#define CONFIG_DEBUG_COLOR
#define CONFIG_DEBUG_TIMELINE

/* OS test config */
/* #define CONFIG_PLATFROM_TEST */
/* #define CONFIG_THREAD_TEST */
/* #define CONFIG_CONSOLE_TEST */
/* #define CONFIG_PAGE_TEST */
/* #define CONFIG_PAGE_HEAP_TEST */
/* #define CONFIG_PAGE_CACHE_TEST */

#endif  /* __OS_CONFIG__ */
