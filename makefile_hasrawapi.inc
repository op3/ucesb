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

ifndef NO_RAWAPI
#RAWAPI_DIR=/u/johansso/staging_adsm/v60c_patched
# v80 has compile issues for source
# v70 has permission issues for the include files
RAWAPI_DIR=/GSI/staging/adsm/v80
HAS_RAWAPI = $(shell test -e $(RAWAPI_DIR)/inc/rawapin.h && echo exist)
endif
