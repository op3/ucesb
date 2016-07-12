#!/usr/bin/perl -W

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

use strict;
use warnings;

my $type = "";
my $def  = "";

foreach my $arg (@ARGV)
{
    if ($arg =~ /default_(fcncall|mirror)_(define|undef)\.hh/)
    {
	$type = $1;
	$def = $2;
    }
}

my %decl = ();

if ($type eq "fcncall")
{
    $decl{"FCNCALL_TEMPLATE"} = "";
    $decl{"FCNCALL_RET_TYPE"} = "void";
    $decl{"FCNCALL_INIT"}     = "((void)0)";
    $decl{"FCNCALL_RET"}      = "((void)0)";
    $decl{"FCNCALL_SUBNAME(name)"} = "((void)0)";
    $decl{"FCNCALL_SUBNAME_END"} = "((void)0)";
    $decl{"FCNCALL_SUBINDEX(index)"} = "((void)0)";
    $decl{"FCNCALL_SUBINDEX_END(index)"} = "((void)0)";
    $decl{"FCNCALL_UNIT(unit)"} = "((void)0)";
    $decl{"FCNCALL_CALL_CTRL_WRAP(ctrl,call)"} = "call";
    $decl{"FCNCALL_CALL_CTRL_WRAP_ARRAY(ctrl_name,ctrl_non_last_index,ctrl_last_index,call)"} = "call";
}
elsif ($type eq "mirror")
{
    $decl{"STRUCT_MIRROR_TEMPLATE"} = "";
    $decl{"STRUCT_MIRROR_TYPE_TEMPLATE"} = "";
    $decl{"STRUCT_MIRROR_TYPE_TEMPLATE_FULL"} = "";
    $decl{"STRUCT_MIRROR_ITEM_CTRL_BASE(name)"} = "";
    $decl{"STRUCT_MIRROR_ITEM_CTRL(name)"} = "";
    $decl{"STRUCT_MIRROR_ITEM_CTRL_ARRAY(name,name2,last_index)"} = "";
}
else { die "Unknown type: $type"; }

print <<"EndOfText";
/***********************************************************************
 *
 * Default $def for $type,
 *
 * Do not edit - automatically generated.
 */

EndOfText

if ($def eq "define")
{
    foreach $def (sort keys %decl)
    {
	my $defname = $def;
	$defname =~ s/\(.*//;

print <<"EndOfText";
#ifndef               $defname
#define __DEFAULT_DEF_$defname
#define               $def $decl{$def}
#endif

EndOfText
    }
}
elsif ($def eq "undef")
{
    foreach $def (sort keys %decl)
    {
	my $defname = $def;
	$defname =~ s/\(.*//;

print <<"EndOfText";
#ifdef __DEFAULT_DEF_$defname
#undef __DEFAULT_DEF_$defname
#undef               $defname
#endif

EndOfText
    }
}
else { die "Unknown def: $def"; }
