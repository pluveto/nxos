/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: RustSBI call 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#ifndef __PLATFORM_RISCV_SBI__
#define __PLATFORM_RISCV_SBI__

#include <XBook.h>

#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 1
#define SBI_CONSOLE_GETCHAR 2
#define SBI_CLEAR_IPI 3
#define SBI_SEND_IPI 4
#define SBI_REMOTE_FENCE_I 5
#define SBI_REMOTE_SFENCE_VMA 6
#define SBI_REMOTE_SFENCE_VMA_ASID 7
#define SBI_SHUTDOWN 8

#define SBI_CALL(which, arg0, arg1, arg2, arg3) ({		\
	register ubase a0 asm ("a0") = (ubase)(arg0);	\
	register ubase a1 asm ("a1") = (ubase)(arg1);	\
	register ubase a2 asm ("a2") = (ubase)(arg2);	\
	register ubase a3 asm ("a3") = (ubase)(arg3);	\
	register ubase a7 asm ("a7") = (ubase)(which);	\
	asm volatile ("ecall"					\
		      : "+r" (a0)				\
		      : "r" (a1), "r" (a2), "r" (a3), "r" (a7)	\
		      : "memory");				\
	a0;							\
})

/* Lazy implementations until SBI is finalized */
#define SBI_CALL_0(which) SBI_CALL(which, 0, 0, 0, 0)
#define SBI_CALL_1(which, arg0) SBI_CALL(which, arg0, 0, 0, 0)
#define SBI_CALL_2(which, arg0, arg1) SBI_CALL(which, arg0, arg1, 0, 0)
#define SBI_CALL_3(which, arg0, arg1, arg2) \
		SBI_CALL(which, arg0, arg1, arg2, 0)
#define SBI_CALL_4(which, arg0, arg1, arg2, arg3) \
		SBI_CALL(which, arg0, arg1, arg2, arg3)

INLINE void SBI_ConsolePutChar(int ch)
{
	SBI_CALL_1(SBI_CONSOLE_PUTCHAR, ch);
}

INLINE int SBI_ConsoleGetChar(void)
{
	return SBI_CALL_0(SBI_CONSOLE_GETCHAR);
}

INLINE void SBI_SetTimer(uint64 stime_value)
{
	SBI_CALL_1(SBI_SET_TIMER, stime_value);
}

INLINE void SBI_Shutdown(void)
{
	SBI_CALL_0(SBI_SHUTDOWN);
}

INLINE void SBI_ClearIpi(void)
{
	SBI_CALL_0(SBI_CLEAR_IPI);
}

INLINE void SBI_SendIpi(const ubase *hart_mask)
{
	SBI_CALL_1(SBI_SEND_IPI, hart_mask);
}

INLINE void SBI_RemoteFenceI(const ubase *hart_mask)
{
	SBI_CALL_1(SBI_REMOTE_FENCE_I, hart_mask);
}

INLINE void SBI_RemoteSfenceVma(const ubase *hart_mask, ubase start, ubase size)
{
	SBI_CALL_3(SBI_REMOTE_SFENCE_VMA, hart_mask, start, size);
}

INLINE void SBI_RemoteSfenceVmaAsid(const ubase *hart_mask,
    ubase start,
    ubase size,
    ubase asid)
{
	SBI_CALL_4(SBI_REMOTE_SFENCE_VMA_ASID, hart_mask, start, size, asid);
}

INLINE void SBI_SetExternInterrupt(ubase func_pointer)
{
	asm volatile("mv a6, %0" : : "r" (0x210));
	SBI_CALL_1(0x0A000004, func_pointer);
}

INLINE void SBI_SetMIE(void)
{
	SBI_CALL_0(0x0A000005);
}

#endif  /* __PLATFORM_RISCV_SBI__ */
