##
# Copyright (c) 2018-2021, BookOS Development Team
# SPDX-License-Identifier: Apache-2.0
# 
# Contains: Makefile for NXOS
# 
# Change Logs:
# Date           Author            Notes
# 2021-9-20      JasonHu           Init
##

#
# Tools name
#
MAKE = make

#
# Use build dir for target
#
USE_BUILD_DIR	:=y
BUILD_DIR :=Build

#
# Set compile and machine
#
CROSS_COMPILE	?=
PLATFORM		?=

#
# Target file name
#
NXOS_NAME ?= NXOS
export NXOS_NAME

#
# Get platform information about ARCH and MACH from PLATFORM variable.
#
ifeq ($(words $(subst -, , $(PLATFORM))), 2)
ARCH			:= $(word 1, $(subst -, , $(PLATFORM)))
MACH			:= Platform-$(word 2, $(subst -, , $(PLATFORM)))
else
ARCH			:= I386
MACH			:= Platform-PC32
#ARCH			:= Riscv64
#MACH			:= Platform-Qemu
endif

#
# System environment variable.
#
ifeq ($(OS), Windows_NT)
	HOSTOS		:= windows
else
	ifneq (,$(findstring Linux, $(shell uname -a)))
		HOSTOS	:= linux
	endif
endif

#
# Override default variables.
#
sinclude Src/Platforms/$(ARCH)/$(MACH)/COMPILE.mk

#
# Export global values
#
export CROSS_COMPILE
export MACH
export ARCH
export HOSTOS
export USE_BUILD_DIR

#
# Cmds
#
.PHONY: all clean run

#
# Compile only
#
all:
ifeq ($(USE_BUILD_DIR), y)
	@$(MAKE) -s -C Src O=$(BUILD_DIR)
else
	@$(MAKE) -s -C Src
endif

#
# Clean all targets
#
clean:
ifeq ($(USE_BUILD_DIR), y)
	@$(MAKE) -s -C Src clean O=$(BUILD_DIR)
else
	@$(MAKE) -s -C Src clean
endif
	@$(MAKE) -s -C Src/Platforms/$(ARCH)/$(MACH) clean

#
# Run OS
#
run: all
	@$(MAKE) -s -C Src/Platforms/$(ARCH)/$(MACH) run

#
# Prepare platform tools
#
prepare: 
	@$(MAKE) -s -C Src/Platforms/$(ARCH)/$(MACH) prepare
