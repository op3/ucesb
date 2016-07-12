// -*- C++ -*-

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

// #include "spec/spec.spec"

// #include "is430_05_vme.spec"

external EXT_LADDER(id);

NORMAL_EVENT()
{
  select several
    {
      external ladder[0] = EXT_LADDER(id=0);
      external ladder[1] = EXT_LADDER(id=1);
      // external ladder[2] = EXT_LADDER(id=2);
      // external ladder[3] = EXT_LADDER(id=3);
      external ladder[4] = EXT_LADDER(id=4);
      external ladder[5] = EXT_LADDER(id=5);
      external ladder[8] = EXT_LADDER(id=8);
    }

  UINT16 end_marker
    {
      0_7   : 0xc0;
      8_9   : any;  // have seen 0x01c0 and 0x03c0
      10_15 : 0;
    };
};

SUBEVENT(SID_GENF_EVENT)
{
  normal = NORMAL_EVENT();
}

EVENT
{
  // dummy = DUMMY_SUBEVENT();
  // vme = IS430_05_VME(type=10,subtype=1);

  esn = SID_GENF_EVENT();

}

//SIGNAL(VDC2_2_240T8,,(DATA12,float));
//SIGNAL(ZERO_SUPPRESS: VDC2_2_240);
//SIGNAL(ZERO_SUPPRESS: VDC2_2_240T8);

// SIGNAL(MWPC4_2H,,EXT_MWPC_HITS);
// SIGNAL(VDC2_2H,,EXT_VDC_RAW);
