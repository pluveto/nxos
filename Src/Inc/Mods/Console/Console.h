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
#include <XBook/HAL.h>

#define CON_NEWLINE "\n"

#define Endln CON_NEWLINE

PUBLIC void ConsoleOutChar(char ch);
PUBLIC void ConsoleOutStr(const char *str);

#define Cout(x, ...) \
        do { \
            Uint _level = HAL_InterruptSaveLevel(); \
            char *_args[] = {x, ##__VA_ARGS__}; \
            int _i; \
            for (_i = 0; _i < sizeof(_args)/sizeof(_args[0]); _i++) \
            { \
                Printf("%s", _args[_i]); \
            } \
            HAL_InterruptRestoreLevel(_level); \
        } while (0)

void Printf(const char *fmt, ...);

#endif  /* __MODS_CONSOLE_HEADER__ */