/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: HAL Memory Barrier
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-9      JasonHu           Init
 */

#include <MM/Barrier.h>

NX_PRIVATE void HAL_MemBarrier(void)
{
    NX_CASM("": : :"memory");
}

NX_PRIVATE void HAL_MemBarrierRead(void)
{
    NX_CASM("lfence": : :"memory");
}

NX_PRIVATE void HAL_MemBarrierWrite(void)
{
    NX_CASM("sfence": : :"memory");
}

NX_PRIVATE void HAL_MemBarrierInstruction(void)
{
    NX_CASM("": : :"memory");
}

NX_INTERFACE struct NX_MemBarrierOps NX_MemBarrierOpsInterface = 
{
    .barrier             = HAL_MemBarrier,
    .barrierRead         = HAL_MemBarrierRead,
    .barrierWrite        = HAL_MemBarrierWrite,
    .barrierInstruction  = HAL_MemBarrierInstruction,
};
