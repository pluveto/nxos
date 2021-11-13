/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: variable args
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-13     JasonHu           Init
 */

#ifndef __UTILS_VAR_ARGS__
#define __UTILS_VAR_ARGS__

typedef __builtin_va_list   VarList;

/*
 * prepare to access variable args
 */
#define VarStart(v, l)		__builtin_va_start(v, l)

/*
 * the caller will get the value of current argument
 */
#define VarArg(v, l)		__builtin_va_arg(v, l)

/*
 * end for variable args
 */
#define VarEnd(v)			__builtin_va_end(v)

/*
 * copy variable args
 */
#define VarCopy(d, s)		__builtin_va_copy(d, s)

#endif  /* __UTILS_VAR_ARGS__ */
