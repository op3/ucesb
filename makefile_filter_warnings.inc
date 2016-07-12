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

# Makefile include for generating listings of dependencies

# $(1) file pattern when to avoid the warning, $(2) warnings to filter out
# $(3) the file, $(4) warnings input
FILTER_WARNING = $(if $(filter $(1),$(3)),$(filter-out $(2),$(4)),$(4))
# $(1) file pattern when to add the warning, $(2) warnings to add
# $(3) the file
ADD_WARNING = $(if $(filter $(1),$(3)),$(2))

CONV_BISON_WARN_OBJS = generated/parser.o generated/lexer.o \
	generated/st_parser.o generated/st_lexer.o

FILTER_CONV_BISON = $(call FILTER_WARNING,$(CONV_BISON_WARN_OBJS),-Wconversion,$(1),$(2))

RAWAPI_WARN_OBJS = rawapi_rawCliProcn.o rawapi_rawProcUn.o rawapi_rawProcn.o \
        rawapi_rawapin.o rawapi_rconnect.o rfio_pipe_buffer.o

FILTER_RAWAPI = $(call FILTER_WARNING,$(RAWAPI_WARN_OBJS),-Wconversion,$(1),$(2)) $(call ADD_WARNING,$(RAWAPI_WARN_OBJS),-Wno-unused-variable -Wno-format -Wno-sign-compare -Wno-uninitialized,$(1))

# A ton of warnings about conversions from the fortran-hacks used by hbook
# since gcc 4.6

HBOOK_WARN_OBJS = hbook.o

FILTER_HBOOK = $(call FILTER_WARNING,$(HBOOK_WARN_OBJS),-Wconversion,$(1),$(2))

# GCC 4.3 warns badly (falsely) about int -> short conversion in htons
# and friend.  It cannot be circumvented by any casting!
ifneq (,$(GCC_IS_4_3)$(GCC_IS_4_4))
FILTER_GCC_4_3 = $(filter-out -Wconversion,$2)
else
FILTER_GCC_4_3 = $2
endif

# $(1) the file, $(2) warnings input
FILTER_WARNINGS = $(call FILTER_CONV_BISON,$1, \
	$(call FILTER_RAWAPI,$1, \
	$(call FILTER_HBOOK,$1, \
	$(call FILTER_GCC_4_3,$1,$2))))

# call like this: $(call FILTER_WARNINGS,$@,$(CXXFLAGS))

