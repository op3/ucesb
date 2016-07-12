/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2016  GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_error.hh"

#include "c_struct.hh"

#include "corr_struct.hh"

extern int lexer_read_fd;

bool parse_definitions();

void usage()
{
  printf ("psdc\n"
	  "usage psdc\n");
  printf ("    ");
}

extern bool _ignore_missing_corr_struct;

int main(int argc,char *argv[])
{
  lexer_read_fd = 0; // read from stdin

  for (int i = 1; i < argc; i++)
    if (strcmp(argv[i],"--ignore-missing-corr-struct") == 0)
      _ignore_missing_corr_struct = true;

  if (!parse_definitions())
    ERROR("%s: Aborting!\n",argv[0]);

  map_definitions();

  dump_definitions();

  mirror_struct();
  mirror_struct_decl();
  fcn_call_per_member();

  corr_struct();

  return 0;
}

