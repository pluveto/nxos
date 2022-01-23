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

#include <mm/barrier.h>

NX_PRIVATE void HAL_MemBarrier(void)
{
    __sync_synchronize();
}

NX_PRIVATE void HAL_MemBarrierRead(void)
{
    NX_CASM("fence":::"memory");
}

NX_PRIVATE void HAL_MemBarrierWrite(void)
{
    NX_CASM("fence":::"memory");
}

NX_PRIVATE void HAL_MemBarrierInstruction(void)
{
    NX_CASM("fence.i":::"memory");
}

NX_INTERFACE struct NX_MemBarrierOps NX_MemBarrierOpsInterface = 
{
    .barrier            = HAL_MemBarrier,
    .barrierRead        = HAL_MemBarrierRead,
    .barrierWrite       = HAL_MemBarrierWrite,
    .barrierInstruction = HAL_MemBarrierInstruction,
};
