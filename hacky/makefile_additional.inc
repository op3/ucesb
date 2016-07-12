# -*- Makefile -*-

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

USE_LMD_INPUT=1

# If you use an external unpacker (and then copy e.g. the
# external_data.{hh,cc} template files to this directory,
# you need to also get them into the list of sources =>
# enable external_hacky.o below.

OBJS += hacky_user.o # external_hacky_format.o

# The file containing user structure elements that are ntuple-dumpable,
# bia psdc-parsing

USER_STRUCT_FILE = hacky_user_struct.hh

CXXFLAGS += -DCONTROL_INCLUDE
