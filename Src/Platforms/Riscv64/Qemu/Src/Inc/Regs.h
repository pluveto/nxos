/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: arch registers 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-31     JasonHu           Init
 */

#ifndef __PLATFROM_REGS__
#define __PLATFROM_REGS__

#include <XBook.h>

#define SSTATUS_SPP (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5) // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1L << 4) // User Previous Interrupt Enable
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_UIE (1L << 0)  // User Interrupt Enable
#define SSTATUS_FS (1L << 13)  // Float support
#define SSTATUS_XS (1L << 14)  // Accelerator support

INLINE U64 SstatusRead()
{
    U64 x;
    CASM("csrr %0, sstatus" : "=r" (x) );
    return x;
}

INLINE void SstatusWrite(U64 x)
{
    CASM("csrw sstatus, %0" : : "r" (x));
}

#endif  /* __PLATFROM_REGS__ */
