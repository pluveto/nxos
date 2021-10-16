/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Link script for QEMU Riscv64 Platform
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_Start)

BASE_ADDRESS = 0x80200000;

SECTIONS
{
    /* Load the OS at this address: "." means the current address */
    . = BASE_ADDRESS;

    OS_Start = .;

    . = ALIGN(4K);

    textStart = .;
    .text : {
        *(.text.entry)
        *(.text .text.*)
        *(.init.text)
		*(.exit.text)
        . = ALIGN(0x1000);
        _trampoline = .;
        *(trampsec)
        . = ALIGN(0x1000);
        /* ASSERT(. - _trampoline == 0x1000, "error: trampoline larger than one page"); */
        PROVIDE(etext = .);
    }

    . = ALIGN(4K);
    rodataStart = .;
    .rodata : {
        srodata = .;
        *(.rodata .rodata.*)
        erodata = .;
    }

    . = ALIGN(4K);
    dataStart = .;
    .data : {
        sdata = .;
        *(.data .data.*)
        edata = .;
    }

    . = ALIGN(4K);
    bssStart = .;
    .bss : {
	    *(.bss.stack)
        sbss_clear = .;
        *(.sbss .bss .bss.*)
        ebss_clear = .;
    }

    . = ALIGN(4K);
    PROVIDE(OS_End = .);
}