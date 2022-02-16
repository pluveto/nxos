/**
 * Copyright (c) 2018-2022, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Process
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2022-1-8       JasonHu           Init
 */

#include <sched/process.h>
#include <arch/process.h>
#include <sched/thread.h>
#include <mm/alloc.h>
#include <xbook/debug.h>
#include <utils/memory.h>
#include <mm/mmu.h>
#include <mm/page.h>
#include <arch/mmu.h>
#include <mods/fs/romfs.h>

#define NX_LOG_NAME "process"
#define NX_LOG_LEVEL NX_LOG_INFO
#include <utils/log.h>

#include <xbook/debug.h>

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

    if(NX_VmspaceInit(&process->vmspace,
        ARCH_USER_SPACE_VADDR,
        ARCH_USER_SPACE_TOP,
        ARCH_USER_IMAGE_VADDR,
        ARCH_USER_IMAGE_TOP,
        ARCH_USER_HEAP_VADDR,
        ARCH_USER_HEAP_TOP,
        ARCH_USER_MAP_VADDR,
        ARCH_USER_MAP_TOP,
        ARCH_USER_STACK_VADDR,
        ARCH_USER_STACK_TOP) != NX_EOK)
    {
        NX_MemFree(process);
        return NX_NULL;
    }

    if (NX_ProcessInitUserSpace(process, ARCH_USER_SPACE_VADDR, ARCH_USER_SPACE_SIZE) != NX_EOK)
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
    
    NX_ASSERT(process->vmspace.mmu.table != NX_NULL);
    NX_MemFree(process->vmspace.mmu.table);

    NX_MemFree(process);
    return NX_EOK;
}

NX_PRIVATE void ProcessThreadEntry(void *arg)
{
    NX_Thread *thread = NX_ThreadSelf();
    NX_LOG_I("Process %s/%d running...", thread->name, thread->tid);
    /* Jump into userspace to run app */
    NX_ProcessExecuteUser((void *)ARCH_USER_IMAGE_VADDR, (void *)ARCH_USER_STACK_TOP, thread->stackBase + thread->stackSize, NX_NULL);
}

/**
 * Load code & data for process image
 */
NX_PRIVATE NX_Error NX_ProcessLoadImage(NX_Process *process, char *path)
{
    NX_ASSERT(process->vmspace.mmu.table);
    NX_RomfsFile *file = NX_NULL;
    NX_Error err;
    NX_Offset len;
    void *addr;

    err = NX_RomfsOpen(path, 0, &file);
    if (err != NX_EOK)
    {
        NX_LOG_E("process load file %s failed with err %d!", path, err);
        return NX_ENOSRCH;
    }

    err = NX_RomfsSeek(file, 0, NX_ROMFS_SEEK_END, &len);
    if (err != NX_EOK)
    {
        NX_LOG_E("seek file %s failed with err %d!", path, err);
        NX_RomfsClose(file);
        return NX_ENOSRCH;
    }

    NX_LOG_D("process execute file %s size is %d\n", path, len);

    err = NX_RomfsSeek(file, 0, NX_ROMFS_SEEK_SET, NX_NULL);
    if (err != NX_EOK)
    {
        NX_LOG_E("seek file %s failed with err %d!", path, err);
        NX_RomfsClose(file);
        return NX_ENOSRCH;
    }

    /* map code & data memory */
    addr = NX_MmuMapPage(&process->vmspace.mmu, ARCH_USER_IMAGE_VADDR, 4096, ARCH_PAGE_ATTR_USER);
    if (addr == NX_NULL)
    {
        NX_RomfsClose(file);
        return NX_ENOMEM;
    }

    /* read file */
    NX_Addr vaddr = ARCH_USER_IMAGE_VADDR;
    NX_Addr paddr;
    NX_Addr vaddrSelf;
    NX_USize chunk;
    NX_USize chunkRead;
    
    while (len > 0)
    {
        paddr = (NX_Addr)NX_MmuVir2Phy(&process->vmspace.mmu, vaddr);
        NX_ASSERT(paddr);
        vaddrSelf = NX_Phy2Virt(paddr);

        chunk = (len < NX_PAGE_SIZE) ? len : NX_PAGE_SIZE;

        NX_RomfsRead(file, (void *)vaddrSelf, chunk, &chunkRead);

        if (chunkRead != chunk)
        {
            NX_LOG_E("read file failed!");
            break;
        }

        len -= chunk;
        vaddr += chunk;
    }

    /* close file */
    NX_RomfsClose(file);

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

    /* map user stack */
    void *ustack = NX_MmuMapPage(&process->vmspace.mmu, ARCH_USER_STACK_TOP - NX_PAGE_SIZE, NX_PAGE_SIZE, ARCH_PAGE_ATTR_USER);
    if (ustack == NX_NULL)
    {
        /* TODO: unmap process image */
        NX_ProcessDestroy(process);
        NX_ThreadDestroy(thread);
        return NX_ENOMEM;
    }

    ProcessAppendThread(process, thread);

    if (NX_ThreadRun(thread) != NX_EOK)
    {
        NX_MmuUnmapPage(&process->vmspace.mmu, ARCH_USER_STACK_TOP - NX_PAGE_SIZE, NX_PAGE_SIZE);
        /* TODO: unmap process image */
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
