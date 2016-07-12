#!/usr/bin/perl -W

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

# This scripts copies the part of the input which is between lines containing
# BEGIN_${NAME} and END_${NAME} to the output

my $inside = 0;

while ( my $line = <STDIN> )
{
    # Begin copy?
    if ($line =~ /BEGIN_${ARGV[0]}/) { $inside = 1; }

    # Do copy
    if ($inside) { print $line; }

    # End copy?
    if ($line =~ /END_${ARGV[0]}/) { $inside = 0; }
}


###
#
# The remainder of this script could work as extract.sh, and use awk
# instead of needing perl.  Look in makefile_unpacker.mk at EXTRACT=

##!/bin/sh
#
## replacement for extract.pl, that only need awk
#
#awk "BEGIN{skip=1} ; \
# /BEGIN_$1/ { skip = 0; } ; \
# skip == 1 { next } ; \
# { print \$0 } ; \
# /END_$1/ { skip = 1 }"
