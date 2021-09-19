##
# Copyright (c) 2018-2021, BookOS Development Team
# SPDX-License-Identifier: Apache-2.0
# 
# Contains: Makefile for run PC32 Platform
# 
# Change Logs:
# Date           Author            Notes
# 2021-9-20      JasonHu           Init
##

###############################################################################
# Configure options                                                           #
###############################################################################

# config graphic window ? (y/n)
QEMU_WINDOW ?= n

###############################################################################
# Directory                                                                   #
###############################################################################

# tools for PC32
TOOL_DIR 	:= Tools/
GRUB_DIR	:= $(TOOL_DIR)grub-2.04
BIOS_FW_DIR	:= $(TOOL_DIR)bios_fw

###############################################################################
# Variables                                                                   #
###############################################################################

# tools name
MAKE		:= make
QEMU 		:= qemu-system-i386

# target file name
NXOS_NAME ?=NXOS

# qemu args configure
QEMU_ARGS 	:=  -m 128m \
				-name "NXOS - Plartform PC32 - Arch x86" \
				-rtc base=localtime \
				-boot d \
				-cdrom $(NXOS_NAME).iso \

ifeq ($(QEMU_WINDOW),y)
	QEMU_ARGS += -serial stdio
else
	QEMU_ARGS += -nographic
endif

###############################################################################
# Commands                                                                    #
###############################################################################

# args for make
.PHONY: all clean qemu run

all:
	@$(MAKE) -s -C $(GRUB_DIR) KERNEL=$(NXOS_NAME).elf OS_NAME=$(NXOS_NAME)

clean:
	@$(MAKE) -s -C $(GRUB_DIR) clean

emu:
	$(QEMU) $(QEMU_ARGS)

run: all emu
