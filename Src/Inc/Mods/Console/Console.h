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

#define Endln CON_NEWLINE

PUBLIC void ConsoleOutChar(char ch);
PUBLIC void ConsoleOutStr(const char *str);

void Printf(const char *fmt, ...);

#endif  /* __MODS_CONSOLE_HEADER__ */