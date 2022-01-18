/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Process
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2022-1-8       JasonHu           Init
 */

#include <Sched/Process.h>
#include <Sched/Thread.h>
#include <MM/Alloc.h>
#include <XBook/Debug.h>
#include <Utils/Memory.h>
#include <Utils/Log.h>

NX_PRIVATE void ProcessAppendThread(NX_Process *process, NX_Thread *thread)
{
    NX_UArch level;
    NX_SpinLockIRQ(&process->lock, &level);
    NX_AtomicInc(&process->threadCount);
    NX_ListAdd(&thread->processList, &process->threadPoolListHead);
    thread->resource.process = process;
    NX_SpinUnlockIRQ(&process->lock, level);
}

NX_PRIVATE void ProcessDeleteThread(NX_Process *process, NX_Thread *thread)
{
    NX_UArch level;
    NX_SpinLockIRQ(&process->lock, &level);
    NX_AtomicDec(&process->threadCount);
    NX_ListDel(&thread->processList);
    thread->resource.process = NX_NULL;
    NX_SpinUnlockIRQ(&process->lock, level);
}

NX_PUBLIC NX_Process *NX_ProcessCreate(NX_U32 flags)
{
    NX_Process *process = NX_MemAlloc(sizeof(NX_Process));
    if (process == NX_NULL)
    {
        return NX_NULL;
    }

    if (NX_ProcessInitUserSpace(process) != NX_EOK)
    {
        NX_MemFree(process);
        return NX_NULL;
    }
    
    process->flags = flags;

    NX_AtomicSet(&process->threadCount, 0);
    NX_ListInit(&process->threadPoolListHead);

    NX_SpinInit(&process->lock);

    return process;
}

NX_PUBLIC NX_Error NX_ProcessDestroy(NX_Process *process)
{
    if (process == NX_NULL)
    {
        return NX_EINVAL;
    }
    
    NX_ASSERT(process->pageTable != NX_NULL);
    NX_MemFree(process->pageTable);

    NX_MemFree(process);
    return NX_EOK;
}

NX_PRIVATE void ProcessThreadEntry(void *arg)
{
    NX_Thread *thread = NX_ThreadSelf();
    NX_LOG_I("Process %s/%d running...", thread->name, thread->tid);
    /* Jump into userspace to run app */
}

/**
 * Load code & data for process image
 */
NX_PUBLIC NX_Error NX_ProcessLoadImage(NX_Process *process, char *path)
{
    /* open file */

    /* read file */

    /* close file */
    return NX_EOK;
}

/**
 * execute a process with image
 */
NX_PUBLIC NX_Error NX_ProcessExecute(char *name, char *path, NX_U32 flags)
{
    if (name == NX_NULL || path == NX_NULL)
    {
        return NX_EINVAL;
    }

    /* TODO: check path exist */


    NX_Process *process = NX_ProcessCreate(flags);
    if (process == NX_NULL)
    {
        return NX_ENOMEM;
    }

    NX_Thread *thread = NX_ThreadCreate(name, ProcessThreadEntry, NX_NULL);

    if (thread == NX_NULL)
    {
        NX_ProcessDestroy(process);
        return NX_ENOMEM;
    }

    if (NX_ProcessLoadImage(process, path) != NX_EOK)
    {
        NX_ProcessDestroy(process);
        NX_ThreadDestroy(thread);
        return NX_EIO;
    }

    ProcessAppendThread(process, thread);

    if (NX_ThreadRun(thread) != NX_EOK)
    {
        NX_ProcessDestroy(process);
        NX_ThreadDestroy(thread);
        return NX_EFAULT;
    }
    return NX_EOK;
}

NX_PUBLIC void NX_ProcessExit(int exitCode)
{
    NX_Thread *thread, *next;
    NX_Thread *cur = NX_ThreadSelf();
    NX_Process *process = cur->resource.process;
    NX_ASSERT(process != NX_NULL);
    
    process->exitCode = exitCode;

    /* terminate all thread on list */
    NX_ListForEachEntrySafe(thread, next, &process->threadPoolListHead, processList)
    {
        if (thread != cur)
        {
            NX_ThreadTerminate(thread);
        }
    }

    /* wait other threads exit done */
    while (NX_AtomicGet(&process->threadCount) > 1)
    {
        NX_ThreadYield();
    }

    /* exit this thread */
    NX_ThreadExit();
    NX_PANIC("NX_ProcessExit should never be here!");
}

NX_PUBLIC void NX_ThreadExitProcess(NX_Thread *thread, NX_Process *process)
{
    NX_ASSERT(process != NX_NULL && thread != NX_NULL);
    ProcessDeleteThread(process, thread);

    if (NX_AtomicGet(&process->threadCount) == 0)
    {
        /* thread exit need to free process in the last */
        thread->resource.process = process;
    }
}
