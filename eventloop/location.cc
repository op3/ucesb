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

#include "location.hh"
#include "error.hh"

#include "gen/locations.hh"

void ucesb_loc_print_lineno(formatted_error &fe,int internal)
#define FID_PRINTF(...)  fe.printf(__VA_ARGS__)
#if 0
void ucesb_loc_print_lineno(FILE *fid,int internal)
#define FID_PRINTF(...)  fprintf(fid,__VA_ARGS__)
#endif
{
  location *start = spec_locations;
  location *map   = spec_locations + countof(spec_locations) - 1;

  if (countof(spec_locations) == 0)
    FID_PRINTF("BEFORE_START:0:");

  while (map >= start)
    {
      // fprintf(fid,"%s:%d %d,%d\n",map->_file,map->_line,map->_internal,internal);

      if (internal >= map->_internal)
	{
	  FID_PRINTF("%s:%d:",map->_file,map->_line + internal - map->_internal);
	  return;
	}
      map--;
    }
  FID_PRINTF("LINE_NO_TOO_EARLY(%d):0:",internal);
}
