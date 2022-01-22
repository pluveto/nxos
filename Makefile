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
sinclude platform.mk

#
# Tools name
#

#
# Use build dir for target
#
USE_BUILD_DIR	:=y
BUILD_DIR :=build

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
KCONFIGLIB_DIR	:= scripts/kconfiglib

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
ARCH			:= i386
MACH			:= pc32
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
sinclude src/platforms/$(ARCH)/$(MACH)/compile.mk

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
CONFIG_OUT_FILE = ./src/include/nx_configure.h
CONFIG_OUT_FILE_PLATFORM = ./src/platforms/$(ARCH)/$(MACH)/src/include/nx_configure.h
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
	@$(MAKE) -s -C src O=$(BUILD_DIR)
else
	@$(MAKE) -s -C src
endif

cleanAll: clean
	@-rm -f .config
	@-rm -f .config.old
	@-rm -f ./src/platforms/Kconfig
	@-rm -f ./src/include/nx_configure.h

#
# Clean all targets
#
clean:
ifeq ($(USE_BUILD_DIR), y)
	@$(MAKE) -s -C src clean O=$(BUILD_DIR)
else
	@$(MAKE) -s -C src clean
endif
	@$(MAKE) -s -C src/platforms/$(ARCH)/$(MACH) clean

#
# Run OS
#
run: all
	@$(MAKE) -s -C src/platforms/$(ARCH)/$(MACH) run

#
# Prepare platform tools
#
prepare: 
	@$(MAKE) -s -C src/platforms/$(ARCH)/$(MACH) prepare

#
# GDB command
#
gdb:
	@$(MAKE) -s -C src/platforms/$(ARCH)/$(MACH) gdb

#
# dump kernel
#
dump:
	@$(MAKE) -s -C src/platforms/$(ARCH)/$(MACH) dump

#
# menuconfig
#
menuconfig:
ifeq ($(USE_KCONFIGLIB), y)
	@python $(KCONFIGLIB_DIR)/menuconfig.py Kconfig
	@python $(KCONFIGLIB_DIR)/genconfig.py --header-path=$(CONFIG_OUT_FILE).tmp
	@echo "#ifndef __NX_CONFIG__" > $(CONFIG_OUT_FILE)
	@echo "#define __NX_CONFIG__" >> $(CONFIG_OUT_FILE)
	@cat $(CONFIG_OUT_FILE).tmp >> $(CONFIG_OUT_FILE)
	@echo "#endif" >> $(CONFIG_OUT_FILE)
	@rm -f $(CONFIG_OUT_FILE).tmp
	@echo genconfig to $(CONFIG_OUT_FILE) .
endif

defconfig:
	@-rm -f .config
	@-rm -f .config.old
	@-cp src/platforms/$(ARCH)/$(MACH)/defconfig ./.config
	@-cp src/platforms/$(ARCH)/$(MACH)/Kconfig ./src/platforms/Kconfig
	@-cp $(CONFIG_OUT_FILE_PLATFORM) $(CONFIG_OUT_FILE)
	@echo update Kconfig from platform $(ARCH)-$(MACH) .

saveconfig:
	@-cp ./.config src/platforms/$(ARCH)/$(MACH)/defconfig
	@-cp ./src/platforms/Kconfig ./src/platforms/$(ARCH)/$(MACH)/Kconfig 
	@-cp $(CONFIG_OUT_FILE) $(CONFIG_OUT_FILE_PLATFORM)
	@echo save Kconfig to platform $(ARCH)-$(MACH) .
