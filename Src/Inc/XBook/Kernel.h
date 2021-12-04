/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Kernel header
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-19     JasonHu           Add kernel
 */

#ifndef __XBOOK_KERNEL__
#define __XBOOK_KERNEL__

#include <Configure.h>

#ifndef CONFIG_KVADDR_OFFSET
#error "Please config KVADDR_OFFSET"
#else
#define KVADDR_OFFSET CONFIG_KVADDR_OFFSET
#endif

#endif  /* __XBOOK_KERNEL__ */
