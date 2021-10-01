/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Console modules header
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#ifndef __MODS_CONSOLE_HEADER__
#define __MODS_CONSOLE_HEADER__

#include <XBook.h>

#define CON_NEWLINE "\n"

PUBLIC void ConsoleOutChar(char ch);
PUBLIC void ConsoleOutStr(char *str);
PUBLIC void ConsoleOutInt(long n, int radix, int small);

#define __Char(ch) ConsoleOutChar(ch)
#define __Str(str) ConsoleOutStr(str)
#define __Bin(n) ConsoleOutInt(n, 2, 0)
#define __Oct(n) ConsoleOutInt(n, 8, 0)
#define __Int(n) ConsoleOutInt(n, 10, 0)
#define __Hex(n) ConsoleOutInt(n, 16, 1)
#define __HexBig(n) ConsoleOutInt(n, 16, 0)

#define Char(ch) __Char(ch);
#define Str(str) __Str(str);
#define Bin(n) __Bin(n);
#define Oct(n) __Oct(n);
#define Int(n) __Int(n);
#define Hex(n) __Hex(n);
#define HexBig(n) __HexBig(n);

#define Endln __Str(CON_NEWLINE);

/**
 * Console output prefix
 */
#define COUT


#endif  /* __MODS_CONSOLE_HEADER__ */