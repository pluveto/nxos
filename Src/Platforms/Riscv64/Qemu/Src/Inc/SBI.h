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
	register UBase a0 asm ("a0") = (UBase)(arg0);	\
	register UBase a1 asm ("a1") = (UBase)(arg1);	\
	register UBase a2 asm ("a2") = (UBase)(arg2);	\
	register UBase a3 asm ("a3") = (UBase)(arg3);	\
	register UBase a7 asm ("a7") = (UBase)(which);	\
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

INLINE void SBI_SetTimer(Uint64 stime_value)
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

INLINE void SBI_SendIpi(const UBase *hart_mask)
{
	SBI_CALL_1(SBI_SEND_IPI, hart_mask);
}

INLINE void SBI_RemoteFenceI(const UBase *hart_mask)
{
	SBI_CALL_1(SBI_REMOTE_FENCE_I, hart_mask);
}

INLINE void SBI_RemoteSfenceVma(const UBase *hart_mask, UBase start, UBase Size)
{
	SBI_CALL_3(SBI_REMOTE_SFENCE_VMA, hart_mask, start, Size);
}

INLINE void SBI_RemoteSfenceVmaAsid(const UBase *hart_mask,
    UBase start,
    UBase Size,
    UBase asid)
{
	SBI_CALL_4(SBI_REMOTE_SFENCE_VMA_ASID, hart_mask, start, Size, asid);
}

INLINE void SBI_SetExternInterrupt(UBase func_pointer)
{
	asm volatile("mv a6, %0" : : "r" (0x210));
	SBI_CALL_1(0x0A000004, func_pointer);
}

INLINE void SBI_SetMIE(void)
{
	SBI_CALL_0(0x0A000005);
}

#endif  /* __PLATFORM_RISCV_SBI__ */
