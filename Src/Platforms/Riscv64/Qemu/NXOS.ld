/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Link script for QEMU Riscv64 Platform
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 * 2021-12-1      JasonHu           Update symbol
 */

OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_Start)

BASE_ADDRESS = 0x80200000;

SECTIONS
{
    /* Load the OS at this address: "." means the current address */
    . = BASE_ADDRESS;

    /* code segment */
    . = ALIGN(8);
    __OS_TextStart = .;
    .text : 
    {
        *(.text.start)
        *(.text)
        *(.init.text)
		*(.exit.text)
        
        . = ALIGN(8);
        PROVIDE(__InitCallStart = .);
        KEEP(*(.initCall0.text))
        KEEP(*(.initCall1.text))
        KEEP(*(.initCall2.text))
        KEEP(*(.initCall9.text))
        PROVIDE(__InitCallEnd = .);

        . = ALIGN(8);
        PROVIDE(__ExitCallStart = .);
        KEEP(*(.exitcall0.text))
        KEEP(*(.exitcall1.text))
        KEEP(*(.exitcall2.text))
        PROVIDE(__ExitCallEnd = .);
    
        /* section information for utest */
        . = ALIGN(8);
        PROVIDE(__UTestCaseTableStart = .);
        KEEP(*(UTestCaseTable))
        PROVIDE(__UTestCaseTableEnd = .);
        
        /* section information for integration */
        . = ALIGN(8);
        PROVIDE(__IntegrationTableStart = .);
        KEEP(*(IntegrationTable))
        PROVIDE(__IntegrationTableEnd = .);
    }
    __OS_TextEnd = .;


    /* data segment */
    . = ALIGN(8);
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
    
    . = ALIGN(8);
    __OS_BssStart = .;
    .bss       :
    {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    }
    . = ALIGN(8);
    __OS_BssEnd = .;

}