/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Heap Cache
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-15     JasonHu           Init
 */

#include <Mods/Test/Integration.h>
#include <MM/HeapCache.h>
#define LOG_NAME "HeapCache"
#include <Utils/Log.h>

#ifdef CONFIG_TEST_INTEGRATION_HEAP_CACHE

PRIVATE void LargeObjectTest(void)
{
    /* large object */
    void *p = HeapAlloc(2 * SZ_MB);
    if (p == NULL)
    {
        LOG_E("alloc failed!");
        return;
    }
    HeapFree(p);
    p = HeapAlloc(2 * SZ_MB);
    if (p == NULL)
    {
        LOG_E("alloc failed!");
        return;
    }
    HeapFree(p);   
}

PRIVATE void MiddleObjectTest(void)
{
    /* middle object */
    void *p = HeapAlloc(512 * SZ_KB);
    if (p == NULL)
    {
        LOG_E("alloc failed!");
        return;
    }
    HeapFree(p);
    
    p = HeapAlloc(512 * SZ_KB);
    if (p == NULL)
    {
        LOG_E("alloc failed!");
        return;
    }
    HeapFree(p);

    void *buf[50];
    int i, j;
    for (i = 256 + 1, j = 0; i < 1024; i += 16, j++)
    {
        buf[j] = HeapAlloc(i * SZ_KB);
        LOG_D("alloc: %p", buf[j]);
    }

    for (i = 256 + 1, j = 0; i < 1024; i += 16, j++)
    {
        HeapFree(buf[j]);
        LOG_D("free: %p", buf[j]);
    }
}

PRIVATE void SmallObjectTest(void)
{
    /* small object */
    void *p = HeapAlloc(8);
    p = HeapAlloc(9);
    LOG_D("Alloc & Free: %p", p);
    HeapFree(p);
    p = HeapAlloc(16);
    LOG_D("Alloc & Free: %p", p);
    HeapFree(p);
    p = HeapAlloc(20);
    LOG_D("Alloc & Free: %p", p);
    HeapFree(p);
    p = HeapAlloc(28);
    LOG_D("Alloc & Free: %p", p);
    HeapFree(p);
    p = HeapAlloc(32);
    LOG_D("Alloc & Free: %p", p);
    HeapFree(p);
    p = HeapAlloc(48);
    LOG_D("Alloc & Free: %p", p);
    HeapFree(p);
    p = HeapAlloc(63);
    LOG_D("Alloc & Free: %p", p);
    HeapFree(p);
    p = HeapAlloc(72);
    LOG_D("Alloc & Free: %p", p);
    HeapFree(p);
    p = HeapAlloc(120);
    LOG_D("Alloc & Free: %p", p);
    HeapFree(p);
    p = HeapAlloc(250);

    int i, j;
    for (i = 0; i < 20; i++)
    {
        p = HeapAlloc(512);
        LOG_D("Alloc & Free: %p", p);
        HeapFree(p);
    }
    
    for (i = 0; i < 20; i++)
    {
        p = HeapAlloc(2048);
        LOG_D("Alloc & Free: %p", p);
        HeapFree(p);
    }
    
    for (i = 1; i < 4096; i+=8)
    {
        for (j = 0; j < 10; j++)
        {
            p = HeapAlloc(i);
            LOG_D("Alloc & Free: %p USize: %d", p, HeapGetObjectSize(p));
            HeapFree(p);
        }
    }
}

INTEGRATION_TEST(HeapCache)
{
    LargeObjectTest();
    MiddleObjectTest();
    SmallObjectTest();
    return OS_EOK;
}

#endif
