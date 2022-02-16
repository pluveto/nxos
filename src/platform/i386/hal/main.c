/**
 * Copyright (c) 2018-2022, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Platfrom main 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-18     JasonHu           Init
 */

#include <xbook.h>
#include <drivers/direct_uart.h>

#define NX_LOG_NAME "HAL_PlatformMain"
#include <utils/log.h>

#include <mm/vmspace.h>
#include <arch/process.h>
#include <arch/mmu.h>
#include <xbook/debug.h>
#include <sched/process.h>
#include <mm/page.h>

#define RECORD_NR 20

struct mapRecord
{
    void *addr;
    NX_USize size;
};

NX_PRIVATE int recordIdx = 0;
NX_PRIVATE struct mapRecord mapRecord[RECORD_NR];

NX_PRIVATE void AddRecord(void *addr, NX_USize size)
{
    mapRecord[recordIdx].addr = addr;
    mapRecord[recordIdx++].size = size;
}

NX_PRIVATE void VmspaceTest(void)
{
    NX_Error err;

    void *mapAddr;

    NX_Process *process = NX_ProcessCreate(0);
    NX_ASSERT(process != NX_NULL);

    /* map */
    err = NX_VmspaceMap(&process->vmspace, ARCH_USER_SPACE_VADDR + 0x1000, 4095 * 5, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    AddRecord(mapAddr, 4095 * 5);

    /* check map ok */
    void *mapPhy = NX_MmuVir2Phy(&process->vmspace.mmu, ARCH_USER_SPACE_VADDR + 0x2000);
    NX_LOG_I("map phy addr:%p", mapPhy);

    mapAddr = NX_NULL;
    err = NX_VmspaceMap(&process->vmspace, ARCH_USER_SPACE_VADDR + 0x2000, 4095 * 5, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err != NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    NX_LOG_I("map ret:%d", err);

    mapAddr = NX_NULL;
    err = NX_VmspaceMap(&process->vmspace, ARCH_USER_SPACE_VADDR + 0x10000, 4096, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    NX_LOG_I("map ret:%d", err);
    AddRecord(mapAddr, 4096);

    mapAddr = NX_NULL;
    err = NX_VmspaceMap(&process->vmspace, ARCH_USER_SPACE_VADDR + 0xa000, 4096, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    NX_LOG_I("map ret:%d", err);
    AddRecord(mapAddr, 4096);

    err = NX_VmspaceMap(&process->vmspace, ARCH_USER_SPACE_VADDR, 4095, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    AddRecord(mapAddr, 4095);

    err = NX_VmspaceMap(&process->vmspace, ARCH_USER_STACK_TOP - 4096, 4095, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    AddRecord(mapAddr, 4095);

    err = NX_VmspaceMap(&process->vmspace, 0, 4095, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    AddRecord(mapAddr, 4095);

    err = NX_VmspaceMap(&process->vmspace, 0, 4095, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    AddRecord(mapAddr, 4095);

    err = NX_VmspaceMap(&process->vmspace, 0xa2000000, 4096 * 10, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    AddRecord(mapAddr, 4096 * 10);

    err = NX_VmspaceMap(&process->vmspace, 0, 4095, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    AddRecord(mapAddr, 4095);

    mapAddr = NX_NULL;
    err = NX_VmspaceMap(&process->vmspace, 0, 0x40000000, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err != NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    NX_LOG_I("map ret:%d", err);

    mapAddr = NX_NULL;
    err = NX_VmspaceMapWithPhy(&process->vmspace, 0, 0xfe000000, 4096, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    NX_LOG_I("map ret:%d", err);
    AddRecord(mapAddr, 4096);

    mapAddr = NX_NULL;
    err = NX_VmspaceMapWithPhy(&process->vmspace, 0, (NX_Addr)NX_PageAlloc(1), 4096, ARCH_PAGE_ATTR_USER, 0, &mapAddr);
    NX_ASSERT(err == NX_EOK);
    NX_LOG_I("map addr:%p", mapAddr);
    NX_LOG_I("map ret:%d", err);
    AddRecord(mapAddr, 4096);

    int i;
    for (i = 0; i < recordIdx; i++)
    {
        NX_LOG_I("record addr: %p size: %p", mapRecord[i].addr, mapRecord[i].size);
    }

    NX_VmspaceListNodes(&process->vmspace);

    /* unmap */
    
    for (i = 0; i < recordIdx; i++)
    {
        err = NX_VmspaceUnmap(&process->vmspace, (NX_Addr)mapRecord[i].addr, mapRecord[i].size);
        NX_LOG_I("unmap %p ret:%d", mapRecord[i].addr, err);
    }

    NX_VmspaceListNodes(&process->vmspace);

}

NX_INTERFACE void HAL_PlatformMain(void)
{
    NX_LOG_I("PC32 platform main running...\n");

    /* do vmspace test */
    VmspaceTest();
}

NX_PUBLIC void HAL_DirectUartGetcHandler(char data)
{
    switch (data)
    {
    case '\r':
    case '\n':
        HAL_DirectUartPutc('\n');
        break;
    case '\b':
        HAL_DirectUartPutc('\b');
        HAL_DirectUartPutc(' ');
        HAL_DirectUartPutc('\b');
        break;
    default:
        HAL_DirectUartPutc(data);
        break;
    }
}
