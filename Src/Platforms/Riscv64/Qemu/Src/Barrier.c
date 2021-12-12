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

PRIVATE void HAL_MemBarrier(void)
{
    __sync_synchronize();
}

PRIVATE void HAL_MemBarrierRead(void)
{
    CASM("fence":::"memory");
}

PRIVATE void HAL_MemBarrierWrite(void)
{
    CASM("fence":::"memory");
}

PRIVATE void HAL_MemBarrierInstruction(void)
{
    CASM("fence.i":::"memory");
}

INTERFACE struct MemBarrierOps MemBarrierOpsInterface = 
{
    .barrier            = HAL_MemBarrier,
    .barrierRead        = HAL_MemBarrierRead,
    .barrierWrite       = HAL_MemBarrierWrite,
    .barrierInstruction = HAL_MemBarrierInstruction,
};
