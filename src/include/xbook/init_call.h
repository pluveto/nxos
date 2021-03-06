/**
 * Copyright (c) 2018-2022, BookOS Development Team
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

#include <xbook.h>

typedef void (*NX_InitCallHandler)(void);
typedef void (*NX_ExitCallHandler)(void);

#define __DEFINE_INITCALL(level, fn, id) \
	NX_PRIVATE const NX_InitCallHandler __NX_initCall##fn##Id \
	__attribute__((__used__, __section__(".initCall" level ".text"))) = fn

#define __DEFINE_EXITCALL(level, fn, id) \
	NX_PRIVATE const NX_ExitCallHandler __NX_ExitCall##fn##Id \
	__attribute__((__used__, __section__(".exitCall" level ".text"))) = fn

#define NX_INITCALL(fn)		        __DEFINE_INITCALL("0", fn, 0)
#define NX_EXITCALL(fn)		        __DEFINE_EXITCALL("0", fn, 0)

#define NX_INIT_TEST(fn)            __DEFINE_INITCALL("9", fn, 9)

NX_PUBLIC void NX_CallInvoke(NX_InitCallHandler start[], NX_InitCallHandler end[]);

NX_PUBLIC void NX_InitCallInvoke(void);
NX_PUBLIC void NX_ExitCallInvoke(void);

NX_PUBLIC void NX_CallsInit(void);

#endif  /* __XBOOK_INIT_CALL__ */
