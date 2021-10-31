/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Console output
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#include <Mods/Console/Console.h>
#include <Utils/Memory.h>

#include <Utils/Log.h>

PRIVATE char *I2A(long num, char *str, U8 radix, int small)
{
    char *index;
    if (small)
    {
        index = "0123456789abcdefghijklmnopqrstuvwxyz";
    }
    else
    {
        index = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    unsigned long unum;
    int i = 0, j, k;
    char temp;

    /* signe */
    if (radix == 10 && num < 0)
    {
        unum = (unsigned long) -num;
        str[i++] = '-';
    }
    else
    {
        unum = (unsigned long) num;
    }

    /* reverse translate */
    do
    {
        str[i++] = index[unum % (unsigned long)radix];
        unum /= radix;
    } while (unum);
    
    /* pad '\0' to end of str */
    str[i] = '\0';
    
    /* has signe ? */
    if (str[0] == '-')
    {
        k = 1;
    }
    else
    {
        k = 0;
    }

    /* reverse string */
    for (j = k; j <= (i - 1) / 2; j++)
    {
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }
    return str;
}


PUBLIC void ConsoleOutChar(char ch)
{
    HAL_ConsoleOutChar(ch);
}

PUBLIC void ConsoleOutStr(const char *str)
{
    char *p = (char *)str;
    while (*p)
    {
        HAL_ConsoleOutChar(*p++);
    }
}

PRIVATE U32 strBufPtr = 0;
PRIVATE char tmpStrBuf[MAX_BUF_NR][MAX_INT_BUF_SZ];

PUBLIC char *NumberToString(long n, int radix, int small)
{
    if (radix < 2 || radix > 16)
    {
        return "(NULL)";
    }

    /* TODO: fetch and add atomic */
    char *buf = tmpStrBuf[strBufPtr];
    strBufPtr = (strBufPtr + 1) % MAX_BUF_NR;
    Zero(buf, MAX_INT_BUF_SZ);
    return I2A(n, buf, radix, small);
}

void AssertionFailure(char *exp, char *file, char *baseFile, int line)
{
	LOG_E("\nAssert(" $(exp) ") failed:\nfile: " $(file) "\nbase_file: " $(baseFile) "\nln: " $d(line) Endln);
	while (1);
}
