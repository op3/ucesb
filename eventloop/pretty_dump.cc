/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2016  Haakan T. Johansson  <f96hajo@chalmers.se>
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

#include "pretty_dump.hh"

#include <stdio.h>

void pretty_dump(const signal_id &id,const char *buf,
		 pretty_dump_info &pdi)
{
  // There are two ways.  Either we format the new @id completely, and
  // compare to the previous id we had.  Then we can blank out any
  // parts which are the same.  Or we compare the parts of the signals
  // themselves, and create only the new items.

  // Comparing the strings, we'll be forced to compare more than
  // char-by-char, as we'd like a difference to start at an entire
  // item, and not in the middle of a name, due to that character
  // changing...

  // It would also be nice to every 25 lines or so, rewrite the entire
  // string, such that one need not scroll around like mad...

  char name[245];

  id.format(name,sizeof(name));

  printf ("%s: %s\n",name,buf);

}

