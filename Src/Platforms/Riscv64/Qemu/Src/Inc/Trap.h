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

#include <XBook.h>
#include <Interrupt.h>

PUBLIC void CPU_InitTrap(Uint coreId);

PUBLIC void TrapFrameDump(HAL_TrapFrame *frame);

#endif  /* __RISCV_TRAP__ */
