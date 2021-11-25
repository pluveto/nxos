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
    __OS_TextStart = .;
    .text : 
    {
        *(.text.start)
        *(.text)
        *(.init.text)
		*(.exit.text)
        
        . = ALIGN(4);
        PROVIDE(__InitCallStart = .);
        KEEP(*(.initCall0.text))
        KEEP(*(.initCall1.text))
        KEEP(*(.initCall2.text))
        KEEP(*(.initCall9.text))
        PROVIDE(__InitCallEnd = .);

        . = ALIGN(4);
        PROVIDE(__ExitCallStart = .);
        KEEP(*(.exitcall0.text))
        KEEP(*(.exitcall1.text))
        KEEP(*(.exitcall2.text))
        PROVIDE(__ExitCallEnd = .);
    
        /* section information for utest */
        . = ALIGN(4);
        PROVIDE(__UTestCaseTableStart = .);
        KEEP(*(UTestCaseTable))
        PROVIDE(__UTestCaseTableEnd = .);
        
        /* section information for integration */
        . = ALIGN(4);
        PROVIDE(__IntegrationTableStart = .);
        KEEP(*(IntegrationTable))
        PROVIDE(__IntegrationTableEnd = .);
    }
    __OS_TextEnd = .;


    /* data segment */
    . = ALIGN(4);
    __OS_DataStart = .;
    .data :
    {
        *(.data)
        *(.data.*)
        *(.rodata)
        *(.rodata.*)
    }
    __OS_DataEnd = .;
    
    /* bss segement */
    
    . = ALIGN(4);
    __OS_BssStart = .;
    .bss       :
    {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    }
    . = ALIGN(4);
    __OS_BssEnd = .;

}
