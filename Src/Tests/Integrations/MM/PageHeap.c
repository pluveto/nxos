/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Page heap test
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-16     JasonHu           Init
 */

#include <Mods/Test/Integration.h>
#include <MM/PageHeap.h>
#include <MM/Page.h>
#define NX_LOG_NAME "PageHeap"
#include <Utils/Log.h>
#include <XBook/Debug.h>
#include <Utils/Memory.h>

#ifdef CONFIG_NX_TEST_INTEGRATION_PAGE_HEAP

NX_PRIVATE void NX_PageHeapLarge(void)
{
    void *span = NX_PageHeapAlloc(128);
    NX_LOG_D("span: %p", span);
    NX_PageHeapFree(span);
    span = NX_PageHeapAlloc(128);
    NX_LOG_D("span: %p", span);
    NX_PageHeapFree(span);

    span = NX_PageHeapAlloc(256);
    NX_LOG_D("span: %p", span);
    NX_PageHeapFree(span + NX_PAGE_SIZE);
    
    span = NX_PageHeapAlloc(256);
    NX_LOG_D("span: %p", span);
    NX_PageHeapFree(span + NX_PAGE_SIZE);

    void *table[100];
    
    int i;
    for (i = 0; i < 100; i++)
    {
        table[i] = NX_PageHeapAlloc(128 + i);
        NX_LOG_D("alloc span: %p", table[i]);
        NX_MemSet(table[i], 0x5a, NX_PAGE_SIZE * 128 + i);
    }

    for (i = 0; i < 100; i++)
    {
        NX_LOG_D("free span: %p", table[i]);
        NX_PageHeapFree(table[i]);
    }
    
}

NX_PRIVATE void NX_PageHeapSmall(void)
{
    void *span = NX_PageHeapAlloc(1);
    NX_LOG_D("span: %p", span);
    NX_PageHeapFree(span);
    span = NX_PageHeapAlloc(1);
    NX_LOG_D("span: %p", span);
    NX_PageHeapFree(span);

    span = NX_PageHeapAlloc(10);
    NX_LOG_D("span: %p", span);
    NX_PageHeapFree(span);
    span = NX_PageHeapAlloc(10);
    NX_LOG_D("span: %p", span);
    NX_PageHeapFree(span);

    void *table[128];
    
    int i;
    for (i = 0; i < 128; i++)
    {
        table[i] = NX_PageHeapAlloc(i + 1);
        NX_ASSERT(table[i] != NX_NULL);
        NX_LOG_D("alloc span:%d-> %p", i, table[i]);
        NX_MemSet(table[i], 0x5a, NX_PAGE_SIZE * (i + 1));
    }

    for (i = 0; i < 128; i++)
    {
        NX_LOG_D("free span:%d-> %p", i, table[i]);
        NX_PageHeapFree(table[i]);
    }

    for (i = 0; i < 128; i++)
    {
        table[i] = NX_PageHeapAlloc(i + 1);
        NX_ASSERT(table[i] != NX_NULL);
        NX_LOG_D("alloc span:%d-> %p", i, table[i]);
        NX_MemSet(table[i], 0x5a, NX_PAGE_SIZE * (i + 1));
    }

    for (i = 0; i < 128; i++)
    {
        NX_LOG_D("free span:%d-> %p", i, table[i]);
        NX_PageHeapFree(table[i]);
    }
}

NX_PRIVATE void NX_PageHeapOnePage(void)
{
    void *table[1024];
    int i;
    for (i = 0; i < 1024; i++)
    {
        table[i] = NX_PageHeapAlloc(1);
        NX_LOG_D("alloc span: %p", table[i]);
        NX_MemSet(table[i], 0x5a, NX_PAGE_SIZE);
    }

    for (i = 0; i < 1024; i++)
    {
        NX_LOG_D("free span: %p", table[i]);
        NX_PageHeapFree(table[i]);
    }
    for (i = 0; i < 1024; i++)
    {
        table[i] = NX_PageHeapAlloc(1);
        NX_LOG_D("alloc span: %p", table[i]);
        NX_MemSet(table[i], 0x5a, NX_PAGE_SIZE);
    }

    for (i = 0; i < 1024; i++)
    {
        NX_LOG_D("free span: %p", table[i]);
        NX_PageHeapFree(table[i]);
    }
}

NX_INTEGRATION_TEST(NX_PageHeapTest)
{
    NX_LOG_D("===>PageHeapLarge");
    NX_PageHeapLarge();
    NX_LOG_D("===>PageHeapSmall");
    NX_PageHeapSmall();
    NX_LOG_D("===>PageHeapOnePage");
    NX_PageHeapOnePage();
    return NX_EOK;
}

#endif
