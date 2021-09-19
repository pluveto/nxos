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

MAKE = make

USE_BUILD_DIR	:=n

BUILD_DIR :=Build

.PHONY: all clean

all:
ifeq ($(USE_BUILD_DIR), y)
	@$(MAKE) -s -C Src O=$(BUILD_DIR)
else
	@$(MAKE) -s -C Src
endif

clean:
ifeq ($(USE_BUILD_DIR), y)
	@$(MAKE) -s -C Src clean O=$(BUILD_DIR)
else
	@$(MAKE) -s -C Src clean
endif
