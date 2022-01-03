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
#define LOG_NAME "InitCall"
#include <Utils/Log.h>
#include <XBook/Debug.h>

IMPORT InitCallHandler __InitCallStart[];
IMPORT InitCallHandler __InitCallEnd[];
IMPORT InitCallHandler __ExitCallStart[];
IMPORT InitCallHandler __ExitCallEnd[];

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
    CallInvoke(__InitCallStart, __InitCallEnd);
}

PUBLIC void ExitCallInvoke(void)
{
    CallInvoke(__ExitCallStart, __ExitCallEnd);
}

INTERFACE WEAK_SYM void PlatformMain(void)
{
    LOG_I("Deafult platform main running...\n");
}

PRIVATE void CallsEntry(void *arg)
{
    InitCallInvoke();
    PlatformMain();
}

PUBLIC void CallsInit(void)
{
    Thread *thread = ThreadCreate("Calls", CallsEntry, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);
}
