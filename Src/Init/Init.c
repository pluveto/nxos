/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Init OS 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-15     JasonHu           Init
 */

#include <XBook/InitCall.h>
#include <Sched/Thread.h>
#include <Utils/Debug.h>

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

PRIVATE void CallsEntry(void *arg)
{
    InitCallInvoke();
}

PUBLIC void CallsInit(void)
{
    Thread *thread = ThreadCreate("Calls", CallsEntry, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);
}
