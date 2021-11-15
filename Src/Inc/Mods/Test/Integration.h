/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Integration test
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-16     JasonHu           Init
 */

#ifndef __MODS_TEST_INTEGRATION__
#define __MODS_TEST_INTEGRATION__

#include <XBook.h>

#ifdef CONFIG_ENABLE_TEST_INTEGRATION

struct Integration
{
    char *integrationName;
    OS_Error (*func)(void);
};
typedef struct Integration Integration;

#define __INTEGRATION_TEST(name)  OS_Error __Integration##name##Test(void)

#define INTEGRATION_TEST(name) \
    PRIVATE __INTEGRATION_TEST(name); \
    USED PRIVATE const Integration __integrationTest \
    SECTION("IntegrationTable") = \
    { \
        .integrationName = #name, \
        .func = __Integration##name##Test, \
    }; \
    PRIVATE __INTEGRATION_TEST(name) \
    
PUBLIC void IntegrationInit(void);

#endif /* CONFIG_ENABLE_TEST_INTEGRATION */

#endif  /* __MODS_TEST_INTEGRATION__ */

