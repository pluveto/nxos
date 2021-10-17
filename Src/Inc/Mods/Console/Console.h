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
#include <HAL.h>

#define CON_NEWLINE "\n"

#define Endln CON_NEWLINE

#define MAX_INT_BUF_SZ  (64 + 1)
#define MAX_BUF_NR  64

PUBLIC void ConsoleOutChar(char ch);
PUBLIC void ConsoleOutStr(const char *str);
PUBLIC char *NumberToString(long n, int radix, int small);

/**
 * Console Output
 */
#define $(v) , (char *)v, 
#define $s(v) $(v)
#define $d(v) , NumberToString((I32)(v), 10, 0),
#define $x(v) , NumberToString((U32)(v), 16, 1),
#define $X(v) , NumberToString((U32)(v), 16, 0),
#define $b(v) , NumberToString((U32)(v), 2, 0),
#define $o(v) , NumberToString((U32)(v), 8, 0),

#define Cout(x, ...) \
        do { \
            char *args[] = {x, ##__VA_ARGS__}; \
            int i; \
            for (i = 0; i < sizeof(args)/sizeof(args[0]); i++) \
            { \
                ConsoleOutStr(args[i]); \
            } \
        } while (0)

/**
 * Spin here
 */
#define SPIN(str) \
        Cout(str, "FILE:" $s(__FILE__) "\nFUNCTION:" $s(__FUNCTION__) "\nLINE:" $d(__LINE__)); \
        for(;;)

/**
 * OS Panic
 */
#define PANIC(str) \
        HAL_InterruptDisable(); \
        Cout("!PANIC!" CON_NEWLINE); \
        SPIN(str)

#endif  /* __MODS_CONSOLE_HEADER__ */