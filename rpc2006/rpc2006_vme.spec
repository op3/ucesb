// -*- C++ -*-

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

#include "spec/land_std_vme.spec"

SUBEVENT(RPC2006_VME)
{
  header = LAND_STD_VME();

  select several
    {
      norevisit tdc0 = VME_CAEN_V775(geom=16,crate=129);
      norevisit qdc0 = VME_CAEN_V792(geom=0,crate=1);
      norevisit adc0 = VME_CAEN_V785(geom=1,crate=2);
      norevisit scaler0 = VME_CAEN_V830(geom=30);
    }
}
