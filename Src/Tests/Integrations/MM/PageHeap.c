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
#define LOG_NAME "PageHeap"
#include <Utils/Log.h>
#include <Utils/Debug.h>
#include <Utils/Memory.h>

#ifdef CONFIG_TEST_INTEGRATION_PAGE_HEAP

PRIVATE void PageHeapLarge(void)
{
    void *span = PageHeapAlloc(128);
    LOG_D("span: %p", span);
    PageHeapFree(span);
    span = PageHeapAlloc(128);
    LOG_D("span: %p", span);
    PageHeapFree(span);

    span = PageHeapAlloc(256);
    LOG_D("span: %p", span);
    PageHeapFree(span + PAGE_SIZE);
    
    span = PageHeapAlloc(256);
    LOG_D("span: %p", span);
    PageHeapFree(span + PAGE_SIZE);

    void *table[100];
    
    int i;
    for (i = 0; i < 100; i++)
    {
        table[i] = PageHeapAlloc(128 + i);
        LOG_D("alloc span: %p", table[i]);
        Set(table[i], 0x5a, PAGE_SIZE * 128 + i);
    }

    for (i = 0; i < 100; i++)
    {
        LOG_D("free span: %p", table[i]);
        PageHeapFree(table[i]);
    }
    
}

PRIVATE void PageHeapSmall(void)
{
    void *span = PageHeapAlloc(1);
    LOG_D("span: %p", span);
    PageHeapFree(span);
    span = PageHeapAlloc(1);
    LOG_D("span: %p", span);
    PageHeapFree(span);

    span = PageHeapAlloc(10);
    LOG_D("span: %p", span);
    PageHeapFree(span);
    span = PageHeapAlloc(10);
    LOG_D("span: %p", span);
    PageHeapFree(span);

    void *table[128];
    
    int i;
    for (i = 0; i < 128; i++)
    {
        table[i] = PageHeapAlloc(i + 1);
        ASSERT(table[i] != NULL);
        LOG_D("alloc span:%d-> %p", i, table[i]);
        Set(table[i], 0x5a, PAGE_SIZE * (i + 1));
    }

    for (i = 0; i < 128; i++)
    {
        LOG_D("free span:%d-> %p", i, table[i]);
        PageHeapFree(table[i]);
    }

    for (i = 0; i < 128; i++)
    {
        table[i] = PageHeapAlloc(i + 1);
        ASSERT(table[i] != NULL);
        LOG_D("alloc span:%d-> %p", i, table[i]);
        Set(table[i], 0x5a, PAGE_SIZE * (i + 1));
    }

    for (i = 0; i < 128; i++)
    {
        LOG_D("free span:%d-> %p", i, table[i]);
        PageHeapFree(table[i]);
    }
}

PRIVATE void PageHeapOnePage(void)
{
    void *table[1024];
    int i;
    for (i = 0; i < 1024; i++)
    {
        table[i] = PageHeapAlloc(1);
        LOG_D("alloc span: %p", table[i]);
        Set(table[i], 0x5a, PAGE_SIZE);
    }

    for (i = 0; i < 1024; i++)
    {
        LOG_D("free span: %p", table[i]);
        PageHeapFree(table[i]);
    }
    for (i = 0; i < 1024; i++)
    {
        table[i] = PageHeapAlloc(1);
        LOG_D("alloc span: %p", table[i]);
        Set(table[i], 0x5a, PAGE_SIZE);
    }

    for (i = 0; i < 1024; i++)
    {
        LOG_D("free span: %p", table[i]);
        PageHeapFree(table[i]);
    }
}

INTEGRATION_TEST(PageHeapTest)
{
    LOG_D("===>PageHeapLarge");
    PageHeapLarge();
    LOG_D("===>PageHeapSmall");
    PageHeapSmall();
    LOG_D("===>PageHeapOnePage");
    PageHeapOnePage();
    return OS_EOK;
}

#endif
