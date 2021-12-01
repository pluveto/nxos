/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Thread context 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-01     JasonHu           Init
 */

.text

/**
 * void HAL_ContextSwitchNext(Addr nextSP);
 */
.globl HAL_ContextSwitchNext
HAL_ContextSwitchNext:
    ret

/**
 * void HAL_ContextSwitchPrevNext(Addr prevSP, Addr nextSP);
 */
.globl HAL_ContextSwitchPrevNext
HAL_ContextSwitchPrevNext:
    ret
