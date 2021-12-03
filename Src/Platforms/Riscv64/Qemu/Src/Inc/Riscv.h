/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: riscv define
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-3      JasonHu           Init
 */

#ifndef __PLATFROM_RISCV__
#define __PLATFROM_RISCV__

#include <Configure.h>

#ifdef CONFIG_CPU_64BITS
#define STORE                   sd
#define LOAD                    ld
#define REGBYTES                8
#else
#error "not support 32bit!"
#endif

#endif  /* __PLATFROM_RISCV__ */
