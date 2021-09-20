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
USE_BUILD_DIR	:=n
BUILD_DIR :=Build

#
# Set compile and machine
#
CROSS_COMPILE	?=
MACHINE			?=I386-PC32

#
# Target file name
#
NXOS_NAME ?= NXOS
export NXOS_NAME

#
# Get platform information about ARCH and PLATFORM from MACHINE variable.
#
ifeq ($(words $(subst -, , $(MACHINE))), 2)
ARCH			:= $(word 1, $(subst -, , $(MACHINE)))
PLATFORM		:= Platform-$(word 2, $(subst -, , $(MACHINE)))
else
ARCH			:= I386
PLATFORM		:= Platform-PC32
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
# Export global values
#
export CROSS_COMPILE
export PLATFORM
export ARCH
export HOSTOS

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
	@$(MAKE) -s -C Src/Platforms/$(ARCH)/$(PLATFORM) clean

#
# Run OS
#
run: all
	@$(MAKE) -s -C Src/Platforms/$(ARCH)/$(PLATFORM) run
