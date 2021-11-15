/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Log utils
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-31     JasonHu           Init
 */

#ifndef __UTILS_LOG__
#define __UTILS_LOG__

#include <XBook.h>

#ifdef CONFIG_DEBUG
#define LOG_ENABLE
#endif

#ifdef CONFIG_DEBUG_COLOR
#define LOG_COLOR
#endif

/* log level */
#define LOG_ERROR   0
#define LOG_WARNING 1
#define LOG_INFO    2
#define LOG_DBG     3

#ifdef LOG_NAME
#ifndef LOG_MOD_NAME
#define LOG_MOD_NAME LOG_NAME
#endif
#else
#ifndef LOG_MOD_NAME
#define LOG_MOD_NAME "LOG"
#endif
#endif /* LOG_NAME */

#ifdef LOG_ENABLE
#include <Mods/Console/Console.h>
#include <Mods/Time/Clock.h>

#ifdef LOG_LEVEL
#ifndef LOG_MOD_LEVEL
#define LOG_MOD_LEVEL LOG_LEVEL
#endif
#else
#ifdef CONFIG_LOG_LEVEL
#define LOG_MOD_LEVEL CONFIG_LOG_LEVEL
#else
#define LOG_MOD_LEVEL LOG_WARNING
#endif /* CONFIG_LOG_LEVEL */
#endif /* LOG_LEVEL */

/**
 * terminal color
 * BLACK    30
 * RED      31
 * GREEN    32
 * YELLOW   33
 * BLUE     34
 * PURPLE   35
 * CYAN     36
 * WHITE    37
 */
#ifdef LOG_COLOR
#define __LOG_COLOR(n) Printf("\033[%sm", #n)
#define __LOG_BEGIN(logName, color) Printf("\033[%sm[%s-%s] ", #color, logName, LOG_MOD_NAME)
#define __LOG_END Printf("\033[0m" CON_NEWLINE)
#else
#define __LOG_COLOR(n)
#define __LOG_BEGIN(logName, color) Printf("[%s-%s] ", logName, LOG_MOD_NAME)
#define __LOG_END Printf(CON_NEWLINE)
#endif /* LOG_COLOR */


#ifdef CONFIG_DEBUG_TIMELINE
#define LOG_TIMELINE Printf("[%10d.%03d] ", ClockTickGetMilliSecond() / 1000, ClockTickGetMilliSecond() % 1000);
#else
#define LOG_TIMELINE
#endif

#define LOG_LINE(logName, color, ...) \
    do \
    { \
        Uint _level = HAL_InterruptSaveLevel(); \
        LOG_TIMELINE \
        __LOG_BEGIN(logName, color); \
        Printf(__VA_ARGS__); \
        __LOG_END; \
        HAL_InterruptRestoreLevel(_level); \
    } \
    while (0)
#define __LOG_RAW(...) LOG_TIMELINE \
        Printf(__VA_ARGS__)

#else
#define LOG_LINE(logName, color, fmt, ...)
#define __LOG_RAW(fmt, ...)
#endif /* LOG_ENABLE */

/**
 * Log api
 */
#if (LOG_MOD_LEVEL >= LOG_DBG)
#define LOG_D(...)      LOG_LINE("DBG", 0, __VA_ARGS__)
#else
#define LOG_D(...)
#endif

#if (LOG_MOD_LEVEL >= LOG_INFO)
#define LOG_I(...)      LOG_LINE("INFO", 32, __VA_ARGS__)
#else
#define LOG_I(...)
#endif

#if (LOG_MOD_LEVEL >= LOG_WARNING)
#define LOG_W(...)      LOG_LINE("WARN", 33, __VA_ARGS__)
#else
#define LOG_W(...)
#endif

#if (LOG_MOD_LEVEL >= LOG_ERROR)
#define LOG_E(...)      LOG_LINE("ERR", 31, __VA_ARGS__)
#else
#define LOG_E(...)
#endif

#define LOG_RAW(...)         __LOG_RAW(__VA_ARGS__)

#endif  /* __UTILS_LOG__ */
