/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Sched for NXOS
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-8      JasonHu           Init
 */

#ifndef __XBOOK_SCHED___
#define __XBOOK_SCHED___

#include <XBook.h>

PUBLIC void SchedToFirstThread(void);
PUBLIC void SchedWithInterruptDisabled(void);
PUBLIC void SchedYield(void);
PUBLIC void ReSchedCheck(void);

#endif /* __XBOOK_SCHED___ */
