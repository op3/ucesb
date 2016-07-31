# -*- Makefile -*-

# This file is part of UCESB - a tool for data unpacking and processing.
#
# Copyright (C) 2016  Haakan T. Johansson  <f96hajo@chalmers.se>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301  USA

UNAME_S:=$(shell uname -s)

ifneq ($(UNAME_S),Darwin)
  LIB_LRT:=-lrt
else
  LIB_LRT:=
endif

EMPTY_FILE=$(UCESB_BASE_DIR)/file_input/empty_file

$(EMPTY_FILE): gen/acc_auto_def/byteorder_include.h \
	gen/acc_auto_def/byteswap_include.h \
	$(UCESB_BASE_DIR)/hbook/example/test_caen_v775_data.h \
	$(UCESB_BASE_DIR)/hbook/example/test_caen_v1290_data.h

$(EMPTY_FILE): $(EMPTY_FILE).cc
	@echo "  BUILD  $@"
	@$(CXX) -O3 -g \
	  -I$(UCESB_BASE_DIR)/file_input \
	  -I$(UCESB_BASE_DIR)/file_input/lmd_struct \
	  -I$(UCESB_BASE_DIR)/eventloop \
	  -I$(UCESB_BASE_DIR)/acc_def \
	  -I$(UCESB_BASE_DIR) \
	  -o $@ $< $(LIB_LRT)
