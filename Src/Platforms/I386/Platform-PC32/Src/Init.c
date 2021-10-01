/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Init PC32 platfrom 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-17      JasonHu           Init
 */

#include <XBook.h>
#include <Platforms/Init.h>
#include <I386.h>
#include <DirectUart.h>
#include <Mods/Console/Console.h>

INTERFACE int PlatformInit(void)
{
    HAL_DirectUartInit();

    COUT Char('A')Str("Hello, world!\n");
    
    int n = 123456789;
    COUT Int(n)Endln;
    COUT Hex(n)Endln;
    COUT HexBig(n)Endln;
    COUT Oct(n)Endln;
    COUT Bin(n)Endln;

    n = -123456789;
    COUT Int(n)Endln;
    COUT Hex(n)Endln;
    COUT HexBig(n)Endln;
    COUT Oct(n)Endln;
    COUT Bin(n)Endln;

    n = 0x1234abcd;
    COUT Int(n)Endln;
    COUT Hex(n)Endln;
    COUT HexBig(n)Endln;
    COUT Oct(n)Endln;
    COUT Bin(n)Endln;

    n = -0x1234abcd;
    COUT Int(n)Endln;
    COUT Hex(n)Endln;
    COUT HexBig(n)Endln;
    COUT Oct(n)Endln;
    COUT Bin(n)Endln;

    u32 i = 0;
    for (;;)
    {
        i++;
    }
    return 0;
}
