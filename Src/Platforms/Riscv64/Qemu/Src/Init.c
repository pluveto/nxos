/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Init Riscv64 Qemu platfrom 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#include <XBook.h>
#include <Platforms/Init.h>
#include <Mods/Console/Console.h>

INTERFACE int PlatformInit(void)
{
    COUT Str("Hello, QEMU Riscv64!") Endln;
    U32 i = 0;
    for (;;)
    {
        i++;
    }
    return 0;
}
