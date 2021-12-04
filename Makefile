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
# Get platform config
#
sinclude Platform.mk

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
# Use kconfiglib
#
USE_KCONFIGLIB	:=y
KCONFIGLIB_DIR	:= Scripts/Kconfiglib

#
# Enable GDB debug
#
G	?=n

#
# Get platform information about ARCH and MACH from PLATFORM variable.
#
ifeq ($(words $(subst -, , $(PLATFORM))), 2)
ARCH			:= $(word 1, $(subst -, , $(PLATFORM)))
MACH			:= $(word 2, $(subst -, , $(PLATFORM)))
else
# you can set default platfrom here
ARCH			:= I386
MACH			:= PC32
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
export G

#
# Kconfig path
#
CONFIG_OUT_FILE = ./Src/Inc/Configure.h
CONFIG_OUT_FILE_PLATFORM = ./Src/Platforms/$(ARCH)/$(MACH)/Src/Inc/Configure.h
CONFIG_IN_FILE = .config

#
# Cmds
#
.PHONY: all clean run prepare menuconfig defconfig saveconfig gdb cleanAll

#
# Compile only
#
all:
ifeq ($(USE_BUILD_DIR), y)
	@$(MAKE) -s -C Src O=$(BUILD_DIR)
else
	@$(MAKE) -s -C Src
endif

cleanAll: clean
	@-rm -f .config
	@-rm -f .config.old
	@-rm -f ./Src/Platforms/Kconfig
	@-rm -f ./Src/Inc/Configure.h

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

#
# GDB command
#
gdb:
	@$(MAKE) -s -C Src/Platforms/$(ARCH)/$(MACH) gdb

#
# dump kernel
#
dump:
	@$(MAKE) -s -C Src/Platforms/$(ARCH)/$(MACH) dump

#
# menuconfig
#
menuconfig:
ifeq ($(USE_KCONFIGLIB), y)
	@python $(KCONFIGLIB_DIR)/menuconfig.py Kconfig
	@python $(KCONFIGLIB_DIR)/genconfig.py --header-path=$(CONFIG_OUT_FILE).tmp
	@echo "#ifndef __OS_CONFIG__" > $(CONFIG_OUT_FILE)
	@echo "#define __OS_CONFIG__" >> $(CONFIG_OUT_FILE)
	@cat $(CONFIG_OUT_FILE).tmp >> $(CONFIG_OUT_FILE)
	@echo "#endif" >> $(CONFIG_OUT_FILE)
	@rm -f $(CONFIG_OUT_FILE).tmp
	@echo genconfig to $(CONFIG_OUT_FILE) .
endif

defconfig:
	@-rm -f .config
	@-rm -f .config.old
	@-cp Src/Platforms/$(ARCH)/$(MACH)/defconfig ./.config
	@-cp Src/Platforms/$(ARCH)/$(MACH)/Kconfig ./Src/Platforms/Kconfig
	@echo update Kconfig from platform $(ARCH)-$(MACH) .

saveconfig:
	@-cp ./.config Src/Platforms/$(ARCH)/$(MACH)/defconfig
	@-cp ./Src/Platforms/Kconfig ./Src/Platforms/$(ARCH)/$(MACH)/Kconfig 
	@-cp $(CONFIG_OUT_FILE) $(CONFIG_OUT_FILE_PLATFORM)
	@echo save Kconfig to platform $(ARCH)-$(MACH) .
