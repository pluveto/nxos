/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: auto init
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-2     JasonHu           Init
 */

#ifndef __XBOOK_INIT_CALL__
#define __XBOOK_INIT_CALL__

#include <XBook.h>

typedef void (*InitCallHandler)(void);
typedef void (*ExitCallHandler)(void);

#define INIT __attribute__ ((__section__ (".init.text")))
#define EXIT __attribute__ ((__section__ (".exit.text")))

#define __DEFINE_INITCALL(level, fn, id) \
	PRIVATE const InitCallHandler __initCall##fn##Id \
	__attribute__((__used__, __section__(".initCall" level ".text"))) = fn

#define __DEFINE_EXITCALL(level, fn, id) \
	PRIVATE const ExitCallHandler __exitCall##fn##Id \
	__attribute__((__used__, __section__(".exitCall" level ".text"))) = fn

#define INITCALL(fn)		        __DEFINE_INITCALL("0", fn, 0)
#define EXITCALL(fn)		        __DEFINE_EXITCALL("0", fn, 0)

#define INIT_TEST(fn)               __DEFINE_INITCALL("9", fn, 9)

PUBLIC void CallInvoke(InitCallHandler start[], InitCallHandler end[]);

PUBLIC void InitCallInvoke(void);
PUBLIC void ExitCallInvoke(void);

PUBLIC void CallsInit(void);

#endif  /* __XBOOK_INIT_CALL__ */
