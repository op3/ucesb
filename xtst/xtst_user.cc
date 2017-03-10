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

#include "structures.hh"

#include "user.hh"
#include "lmd_event.hh"
#include <stdint.h>

#define NUM_CRATES  16

static uint32_t _sticky_active[NUM_CRATES];
static uint32_t _sticky_mark[NUM_CRATES];

static uint32_t _sticky_last_ev[NUM_CRATES][32];

uint64_t _spurious_revokes = 0;

void init_function()
{
  memset(_sticky_active, 0, sizeof (_sticky_active));
  memset(_sticky_mark, 0, sizeof (_sticky_mark));
  memset(_sticky_last_ev, 0, sizeof (_sticky_last_ev));
}

void sticky_subevent_user_function(unpack_event *event,
				   const void *header,
				   const char *start, const char *end,
				   bool swapping)
{
  lmd_subevent_10_1_host *sev_header = (lmd_subevent_10_1_host *) header;
  /*
  printf ("%5d %5d %2d (%d)\n",
	  sev_header->_header.i_type,
	  sev_header->_header.i_subtype,
	  sev_header->h_control,
	  sev_header->_header.l_dlen);
  */
  if (sev_header->_header.i_type == 10 &&
      sev_header->_header.i_subtype == 1)
    return; // Timestamp subevent (not real sticky)

  int crate = sev_header->h_subcrate;
  int isev = sev_header->h_control;
  
  if (sev_header->_header.l_dlen == LMD_SUBEVENT_STICKY_DLEN_REVOKE)
    {
      if (!(_sticky_active[crate] & (1 << isev)))
	{
	  /* Revoking markers may be spurious, so just count them, do
	   * not inform.
	   */
	  _spurious_revokes++;
	  /*
	  WARNING("Event %d: "
		  "Non-active sticky subevent (%d) revoked.",
		  event->event_no,
		  isev);
	  */
	}
      _sticky_active[crate] &= ~(1 << isev);
    }
  else
    {
      _sticky_active[crate] |= 1 << isev;

      if (end > start)
	{
	  const uint32_t *p = (const uint32_t *) start;

	  uint32_t payload = *p;

	  _sticky_mark[crate] = (_sticky_mark[crate] & ~(1 << isev)) |
	    ((payload & 1) << isev);
	}
    }

  if (isev < 32)
    {
      if (_sticky_last_ev[crate][isev] == event->event_no)
	{
	  WARNING("Event %d: crate %d: "
		  "Sticky subevent (%d) seen for same event no.",
		  event->event_no, crate,
		  isev);
	}      
      _sticky_last_ev[crate][isev] = event->event_no;
    }
}

int user_function(unpack_event *event)
{
  for (int crate = 0; crate < NUM_CRATES; crate++)
    {
      if (_sticky_active[crate] != event->regress[crate].sticky_active.active)
	{
	  for (int isev = 0; isev < 8; isev++)
	    WARNING("Event %d: crate %d: "
		    "Sticky subevent (%d) from event: %d.  (%s)",
		    event->event_no, crate,
		    isev,
		    _sticky_last_ev[crate][isev],
		    _sticky_active[crate] & (1 << isev) ? "active" : "inactive");

	  ERROR("Event %d: crate %d: "
		"Wrong sticky events active (active: %08x, event says: %08x).",
		event->event_no, crate,
		_sticky_active[crate],
		event->regress[crate].sticky_active.active);
	}

      if ((_sticky_mark[crate] & _sticky_active[crate]) !=
	  (event->regress[crate].sticky_active.mark & _sticky_active[crate]))
	{
	  ERROR("Event %d: crate %d: "
		"Wrong sticky events payload (mark: %08x, event says: %08x).",
		event->event_no, crate,
		_sticky_mark[crate] & _sticky_active[crate],
		event->regress[crate].sticky_active.active &
		_sticky_active[crate]);
	}
      // printf ("%02x %02x\n", _sticky_active[crate], _sticky_mark[crate]);
    }

  return 1;
}
