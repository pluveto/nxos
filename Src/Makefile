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
sinclude ../Scripts/XBuild/env.mk

#
# Load default variables.
#
ASFLAGS			:= -g -ggdb -Wall -O0
CFLAGS			:= -g -ggdb -Wall -O0
LDFLAGS			:= -T $(srctree)/Platforms/$(ARCH)/$(MACH)/NXOS.ld -nostdlib -static
MCFLAGS			:=

LIBDIRS			:=
LIBS 			:=
INCDIRS			:=
SRCDIRS			:=

CP				:=cp

#
# Override default variables.
#
sinclude $(srctree)/Platforms/$(ARCH)/$(MACH)/NXOS.mk

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

ASFLAGS		+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu99 -Werror $(DEFINES)
CFLAGS		+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu99 -Werror $(DEFINES)

#
# Add necessary directory for INCDIRS and SRCDIRS.
#
INCDIRS		+=	Inc
INCDIRS		+=	./
INCDIRS		+=	Platforms/$(ARCH)/Src/Inc
INCDIRS		+=	Platforms/$(ARCH)/$(MACH)/Src/Inc

# Srouce codes
SRCDIRS		+=	Platforms/$(ARCH)/Src/
SRCDIRS		+=	Platforms/$(ARCH)/$(MACH)/Src/
SRCDIRS		+=	Init/
SRCDIRS		+=	Mods/
SRCDIRS		+=	Utils/

#
# XBuild variables
#
X_ASFLAGS	:=	$(MCFLAGS) $(ASFLAGS)
X_CFLAGS	:=	$(MCFLAGS) $(CFLAGS)
X_LDFLAGS	:=	$(LDFLAGS)
X_ODFLAGS	:=	$(ODFLAGS)
X_INCDIRS	:=	$(INCDIRS)
SRC			:=	$(SRCDIRS)
NAME		:= Platforms/$(ARCH)/$(MACH)/$(NXOS_NAME).elf

#
# Custom cmds for xbuild
# Link OS and copy into platform dir
#

ifeq ($(USE_BUILD_DIR), y)

define CUSTOM_TARGET_CMD
echo [KERNEL] $@; \
$(LD) $(X_LDFLAGS) -o $@ $(X_OBJS); \
$(CP) $@ $(srctree)/Platforms/$(ARCH)/$(MACH)
endef # CUSTOM_TARGET_CMD

else

define CUSTOM_TARGET_CMD
echo [KERNEL] $@; \
$(LD) $(X_LDFLAGS) -o $@ $(X_OBJS)
endef # CUSTOM_TARGET_CMD

endif # USE_BUILD_DIR