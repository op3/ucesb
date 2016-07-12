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

// This file comes from CVS: isodaq/checker/scaler_timer.hh

#ifndef __SCALER_TIMER_HH__
#define __SCALER_TIMER_HH__

typedef unsigned int   uint32;
typedef unsigned short uint16;

#define ST_HISTORY_SIZE 16 /* power of 2 */

class scaler_timer
{
public:
  scaler_timer();

public:
  uint32  bits_mask;   /* What bits are in use.  (usually 32 or 26) */
  uint32  current0;    /* Time at 0 for last hit. */

  bool  current0_valid;

public:
  uint32  history[ST_HISTORY_SIZE];
  uint32  hist_i;

public:
  void set0(uint32 new0);

  bool set0_estimate(uint32 prev,uint32 now);

  void set_invalid() { current0_valid = false; }

public:
  uint32 diff(uint32 now);


};

#endif/*__SCALER_TIMER_HH__*/
