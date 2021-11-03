/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Init call
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-2      JasonHu           Init
 */

#include <Utils/Log.h>
#include <Utils/InitCall.h>

IMPORT InitCallHandler __initCallStart[];
IMPORT InitCallHandler __initCallEnd[];
IMPORT InitCallHandler __exitCallStart[];
IMPORT InitCallHandler __exitCallEnd[];

PUBLIC void CallInvoke(InitCallHandler start[], InitCallHandler end[])
{
	InitCallHandler *func =  &(*start);
	for (;func < &(*end); func++)
    {
		(*func)();
    }
}

PUBLIC void InitCallInvoke(void)
{
    CallInvoke(__initCallStart, __initCallEnd);
}

PUBLIC void ExitCallInvoke(void)
{
    CallInvoke(__exitCallStart, __exitCallEnd);
}
