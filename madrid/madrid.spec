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

#include "spec/midas32.spec"

SUBEVENT(EV_EVENT)
{
  select several
    {
      adc[0] = MIDAS_CAEN_V785(group=0x20/*32*/);
      adc[1] = MIDAS_CAEN_V785(group=0x21/*33*/);
      adc[2] = MIDAS_CAEN_V785(group=0x22/*34*/);
      adc[3] = MIDAS_CAEN_V785(group=0x23/*35*/);
      adc[4] = MIDAS_CAEN_V785(group=0x24/*36*/);
      tdc[0] = MIDAS_CAEN_V1190(group=0x18/*24*/);
      scaler = MIDAS_CAEN_V830(group=2);
    }
}


EVENT
{
  ev = EV_EVENT();
}

SIGNAL(ZERO_SUPPRESS: DSSSD1_F_1);
SIGNAL(ZERO_SUPPRESS: DSSSD1_B_1);

SIGNAL(DSSSD1_F_1_E, ev.adc[0].data[0],
       DSSSD1_F_16_E,ev.adc[0].data[15],(DATA12, float));
SIGNAL(DSSSD1_B_1_E, ev.adc[0].data[16],
       DSSSD1_B_16_E,ev.adc[0].data[31],(DATA12, float));
SIGNAL(DSSSD2_F_1_E, ev.adc[1].data[0],
       DSSSD2_F_16_E,ev.adc[1].data[15],(DATA12, float));
SIGNAL(DSSSD2_B_1_E, ev.adc[1].data[16],
       DSSSD2_B_16_E,ev.adc[1].data[31],(DATA12, float));

SIGNAL(DSSSD1_F_1_T, ev.tdc[0].data[0],
       DSSSD1_F_16_T,ev.tdc[0].data[15],(DATA16, float));

SIGNAL(COUNTER_1,    ev.scaler.data[0],DATA32);
SIGNAL(COUNTER_2,    ev.scaler.data[1],DATA32);
