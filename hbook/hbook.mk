# This file is part of UCESB - a tool for data unpacking and processing.
#
# Copyright (C) 2016  GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
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

MOD_NAME = hbook
MOD_DIR  = util/hbook
MOD_DEP  = 
MOD_SRCS = hbook.cc
MOD_CSRCS =

ifeq (,$(findstring -DNO_NEED_DATABASE,$(NEED_CXX_FLAGS)))
USING_EXT_WRITER = YES
endif

ifdef USING_EXT_WRITER
NTUPLE_STAGING = 1
endif

ifdef NTUPLE_STAGING
MOD_SRCS += staged_ntuple.cc staging_ntuple.cc writing_ntuple.cc 
ifdef USING_EXT_WRITER
MOD_SRCS += external_writer.cc
endif
endif

#########################################################

EXTWRITE = util/hbook/make_external_struct_sender.pl

EXTWRITE_GENDIR := $(BUILD_DIR)/auto_gen

$(EXTWRITE_GENDIR)/extwrite_%.hh: $(EXTWRITE) $(EXTWRITE_SOURCES)
	@echo "EXTWRSTR $@" # $(ASMGENFLAGS)
	@$(EXTWRITE) --struct=extwrite_$* < $(EXTWRITE_SOURCES) > $@ || rm $@

# This rule fixes up problems with auto-gen dependent paths
# in dependecy files.  Automatic dependencies are a pain.
auto_gen/extwrite_%.hh: $(EXTWRITE_GENDIR)/extwrite_%.hh
	@echo " SPECIAL $@"

#########################################################

include module.mk
