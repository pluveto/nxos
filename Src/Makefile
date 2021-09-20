##
# Copyright (c) 2018-2021, BookOS Development Team
# SPDX-License-Identifier: Apache-2.0
# 
# Contains: Makefile for Source code
# 
# Change Logs:
# Date           Author            Notes
# 2021-9-20      JasonHu           Init
##

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
# Load default variables.
#
ASFLAGS			:= -g -ggdb -Wall -O0
CFLAGS			:= -g -ggdb -Wall -O0
LDFLAGS			:= -T Platforms/$(ARCH)/$(PLATFORM)/NXOS.ld -nostdlib -static
MCFLAGS			:=

LIBDIRS			:=
LIBS 			:=
INCDIRS			:=
SRCDIRS			:=

#
# include xbuild scripts
#
include ../Scripts/XBuild/env.mk

#
# Override default variables.
#

include Platforms/$(ARCH)/$(PLATFORM)/NXOS.mk

#
# Modify default variables.
#
ifeq ($(strip $(ARCH)), Arm32)
DEFINES		+=	-D__ARM__
endif
ifeq ($(strip $(ARCH)), Arm64)
DEFINES		+=	-D__ARM64__
endif
ifeq ($(strip $(ARCH)), Riscv64)
DEFINES		+=	-D__RISCV64__
endif
ifeq ($(strip $(ARCH)), Riscv32)
DEFINES		+=	-D__RISCV32__
endif
ifeq ($(strip $(ARCH)), Amd64)
DEFINES		+=	-D__AMD64__
endif
ifeq ($(strip $(ARCH)), I386)
DEFINES		+=	-D__I386__
endif

ASFLAGS		+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu99 $(DEFINES)
CFLAGS		+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu99 $(DEFINES)

#
# Add necessary directory for INCDIRS and SRCDIRS.
#
INCDIRS		+=	Inc \
				Platforms/$(ARCH)/Inc \
				Platforms/$(ARCH)/$(PLATFORM)/Inc

SRCDIRS		+=	Platforms/$(ARCH)/$(PLATFORM)/*.c \
				Platforms/$(ARCH)/$(PLATFORM)/*.S \
				Init/*.c

#
# XBuild variables
#
X_ASFLAGS	:=	$(MCFLAGS) $(ASFLAGS)
X_CFLAGS	:=	$(MCFLAGS) $(CFLAGS)
X_LDFLAGS	:=	$(LDFLAGS)
X_ODFLAGS	:=	$(ODFLAGS)
X_INCDIRS	:=	$(INCDIRS)
SRC			:=	$(SRCDIRS)
NAME		:= Platforms/$(ARCH)/$(PLATFORM)/$(NXOS_NAME).elf

#
# Custom cmds for xbuild
#
define CUSTOM_TARGET_CMD
echo [KERNEL] $@; \
$(LD) $(X_LDFLAGS) -o $@ $(X_OBJS)
endef
