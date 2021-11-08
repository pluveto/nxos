/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Thread context 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-16     JasonHu           Init
 */

.code32
.text

/**
 * void HAL_ContextSwitchNext(Addr nextSP);
 */
.globl HAL_ContextSwitchNext
HAL_ContextSwitchNext:
    movl 0x4(%esp), %eax    // get thread "nextSP"
    movl (%eax), %esp       // restore sp
    
    popl %ebp
    popl %ebx
    popl %edi
    popl %esi
    ret

/**
 * void HAL_ContextSwitchPrevNext(Addr prevSP, Addr nextSP);
 */
.globl HAL_ContextSwitchPrevNext
HAL_ContextSwitchPrevNext:
    pushl %esi
    pushl %edi
    pushl %ebx
    pushl %ebp

    movl 0x14(%esp), %eax   // get "prevSP"
    movl %esp, (%eax)       // save sp

    movl 0x18(%esp), %eax   // get "nextSP"
    movl (%eax), %esp       // restore sp

    popl %ebp
    popl %ebx
    popl %edi
    popl %esi
    ret
