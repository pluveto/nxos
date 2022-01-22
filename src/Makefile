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
# include xbuild scripts
#
sinclude ../scripts/xbuild/env.mk

#
# Load default variables.
#
ifeq ($(G),y)
ASFLAGS			:= -g -ggdb -O0 -Wall
CFLAGS			:= -g -ggdb -O0 -Wall
else
ASFLAGS			:= -Wall -O3
CFLAGS			:= -Wall -O3
endif
LDFLAGS			:= -T $(srctree)/platforms/$(ARCH)/$(MACH)/nxos.ld -nostdlib -static
MCFLAGS			:=

LIBDIRS			:=
LIBS 			:=
INCDIRS			:=
SRCDIRS			:=

CP				:=cp

#
# Override default variables.
#
sinclude $(srctree)/platforms/$(ARCH)/$(MACH)/nxos.mk

#
# Modify default variables.
#
ifeq ($(strip $(ARCH)), arm32)
DEFINES		+=	-D__ARM__
endif
ifeq ($(strip $(ARCH)), arm64)
DEFINES		+=	-D__ARM64__
endif
ifeq ($(strip $(ARCH)), riscv64)
DEFINES		+=	-D__RISCV64__
endif
ifeq ($(strip $(ARCH)), riscv32)
DEFINES		+=	-D__RISCV32__
endif
ifeq ($(strip $(ARCH)), amd64)
DEFINES		+=	-D__AMD64__
endif
ifeq ($(strip $(ARCH)), i386)
DEFINES		+=	-D__I386__
endif

ASFLAGS		+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu99 $(DEFINES)
CFLAGS		+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu99 $(DEFINES)

#
# Add necessary directory for INCDIRS and SRCDIRS.
#
INCDIRS		+=	include
INCDIRS		+=	./
INCDIRS		+=	platforms/$(ARCH)/src/include
INCDIRS		+=	platforms/$(ARCH)/$(MACH)/src/include

# Srouce codes
SRCDIRS		+=	platforms/$(ARCH)/src/
SRCDIRS		+=	platforms/$(ARCH)/$(MACH)/src/
SRCDIRS		+=	init/
SRCDIRS		+=	mods/
SRCDIRS		+=	utils/
SRCDIRS		+=	mm/
SRCDIRS		+=	tests/
SRCDIRS		+=	sched/
SRCDIRS		+=	io/

#
# XBuild variables
#
X_ASFLAGS	:=	$(MCFLAGS) $(ASFLAGS)
X_CFLAGS	:=	$(MCFLAGS) $(CFLAGS)
X_LDFLAGS	:=	$(LDFLAGS)
X_ODFLAGS	:=	$(ODFLAGS)
X_INCDIRS	:=	$(INCDIRS)
SRC			:=	$(SRCDIRS)
NAME		:= platforms/$(ARCH)/$(MACH)/$(NXOS_NAME).elf

#
# Custom cmds for xbuild
# Link OS and copy into platform dir
#

ifeq ($(USE_BUILD_DIR), y)

define CUSTOM_TARGET_CMD
echo [KERNEL] $@; \
$(LD) $(X_LDFLAGS) -o $@ $(X_OBJS); \
$(CP) $@ $(srctree)/platforms/$(ARCH)/$(MACH); \
$(CP) $@ ./
endef # CUSTOM_TARGET_CMD

else

define CUSTOM_TARGET_CMD
echo [KERNEL] $@; \
$(LD) $(X_LDFLAGS) -o $@ $(X_OBJS); \
$(CP) $@ ./
endef # CUSTOM_TARGET_CMD

endif # USE_BUILD_DIR
