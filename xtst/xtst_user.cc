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

static uint32_t _sticky_active = 0;
static uint32_t _sticky_mark = 0;

static uint32_t _sticky_last_ev[32] = {
  0, 0, 0, 0,  0, 0, 0, 0,
  0, 0, 0, 0,  0, 0, 0, 0,
  0, 0, 0, 0,  0, 0, 0, 0,
  0, 0, 0, 0,  0, 0, 0, 0,
};

uint64_t _spurious_revokes = 0;

void sticky_event_user_function(unpack_event *event,
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

  int isev = sev_header->h_control;
  
  if (sev_header->_header.l_dlen == LMD_SUBEVENT_STICKY_DLEN_REVOKE)
    {
      if (!(_sticky_active & (1 << isev)))
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
      _sticky_active &= ~(1 << isev);
    }
  else
    {
      _sticky_active |= 1 << isev;

      if (end > start)
	{
	  const uint32_t *p = (const uint32_t *) start;

	  uint32_t payload = *p;

	  _sticky_mark = (_sticky_mark & ~(1 << isev)) |
	    ((payload & 1) << isev);
	}
    }

  if (isev < 32)
    {
      if (_sticky_last_ev[isev] == event->event_no)
	{
	  WARNING("Event %d: "
		  "Sticky subevent (%d) seen for same event no.",
		  event->event_no,
		  isev);
	}      
      _sticky_last_ev[isev] = event->event_no;
    }
}

int user_function(unpack_event *event)
{
  (void) _sticky_mark;

  if (_sticky_active != event->regress.sticky_active.active)
    {
      for (int isev = 0; isev < 8; isev++)
	WARNING("Event %d: Sticky subevent (%d) from event: %d.",
		event->event_no,
		isev,
		_sticky_last_ev[isev]);

      ERROR("Event %d: "
	    "Wrong sticky events active (active: %08x, event says: %08x).",
	    event->event_no,
	    _sticky_active, event->regress.sticky_active.active);
    }

  if ((_sticky_mark & _sticky_active) !=
      (event->regress.sticky_active.mark & _sticky_active))
    {
      ERROR("Event %d: "
	    "Wrong sticky events payload (mark: %08x, event says: %08x).",
	    event->event_no,
	    _sticky_mark & _sticky_active,
	    event->regress.sticky_active.active & _sticky_active);
    }

  // printf ("%02x %02x\n", _sticky_active, _sticky_mark);

  return 1;
}