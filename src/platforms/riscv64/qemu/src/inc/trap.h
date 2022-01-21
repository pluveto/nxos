/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: riscv trap
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-3      JasonHu           Init
 */

#ifndef __RISCV_TRAP__
#define __RISCV_TRAP__

#include <xbook.h>
#include <interrupt.h>

NX_PUBLIC void CPU_InitTrap(NX_UArch coreId);

NX_PUBLIC void TrapFrameDump(HAL_TrapFrame *frame);

#endif  /* __RISCV_TRAP__ */
