/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Link script for PC32 Platfrom
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-12      JasonHu           Init
 */

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(Start)

SECTIONS {
    /* Load the kernel at this address: "." means the current address */
    
    . = 0x00100000; /* GRUB2 need kernel here */

    /* code segment */
    . = ALIGN(4);
    __textStart = .;
    .text : {
        *(.text.start)
        *(.text)
        *(.init.text)
		*(.exit.text)
        
        . = ALIGN(4);
        PROVIDE(__initCallStart = .);
        KEEP(*(.initCall0.text))
        KEEP(*(.initCall1.text))
        KEEP(*(.initCall2.text))
        PROVIDE(__initCallEnd = .);

        . = ALIGN(4);
        PROVIDE(__exitCallStart = .);
        KEEP(*(.exitcall0.text))
        KEEP(*(.exitcall1.text))
        KEEP(*(.exitcall2.text))
        PROVIDE(__exitCallEnd = .);
    
        /* section information for utest */
        . = ALIGN(4);
        PROVIDE(__UTestCaseTableStart = .);
        KEEP(*(UTestCaseTable))
        PROVIDE(__UTestCaseTableEnd = .);
    }
    __textEnd = .;

	. = ALIGN(4);
    __rodataStart = .;
    .rodata : { *(.rodata) *(.rodata.*) }
    __rodataEnd = .;


    /* data segment */
    . = ALIGN(4);
    __dataStart = .;
    .data :
    {
        *(.data)
        *(.data.*)
    }
    __dataEnd = .;
    
    /* bss segement */
    
    . = ALIGN(4);
    __bssStart = .;
    .bss       :
    {
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    }
    . = ALIGN(4);
    __bssEnd = .;

}
