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

#include "structures.hh"

#include "user.hh"

void user_function(unpack_event *event,
		   raw_event    *raw_event,
		   cal_event    *cal_event)
{
  printf ("%7d: %5d %5d %5d %5d\n",
	  event->event_no,
	  raw_event->SCI[0][0].T.value,
	  raw_event->SCI[0][1].T.value,
	  raw_event->SCI[0][0].E.value,
	  raw_event->SCI[0][1].E.value);
  /*
  printf ("tdc0: "); event->vme.tdc0.data.dump(); printf ("\n");
  printf ("qdc0: "); event->vme.qdc0.data.dump(); printf ("\n");
  printf ("adc0: "); event->vme.adc0.data.dump(); printf ("\n");
  printf ("scaler: "); event->vme.scaler0.data.dump(); printf ("\n");
  */
}
