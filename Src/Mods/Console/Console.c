/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Console output
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#include <Utils/Memory.h>
#include <Utils/Log.h>
#include <Utils/String.h>
#include <Utils/VarArgs.h>

/* Conosle HAL */
INTERFACE WEAK_SYM void HAL_ConsoleOutChar(char ch) {}

PUBLIC void ConsoleOutChar(char ch)
{
    HAL_ConsoleOutChar(ch);
}

PUBLIC void ConsoleOutStr(const char *str)
{
    char *p = (char *)str;
    while (*p)
    {
        HAL_ConsoleOutChar(*p++);
    }
}

void AssertionFailure(char *exp, char *file, char *baseFile, int line)
{
	LOG_E("\nAssert(%s) failed:\nfile: %s\nbase_file: %s\nln: %d", 
        exp, file, baseFile, line);
	while (1);
}

#ifdef CONFIG_PRINT_BUF_LEN
#define PRINT_BUF_LEN CONFIG_PRINT_BUF_LEN
#else
#define PRINT_BUF_LEN 256
#endif

void Printf(const char *fmt, ...)
{
	char buf[PRINT_BUF_LEN] = {0};
	VarList arg;
    VarStart(arg, fmt);
    VarStrPrintfN(buf, PRINT_BUF_LEN, fmt, arg);
	VarEnd(arg);
    ConsoleOutStr(buf);
}
