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

external EXT_CROS3(ccb_id);

SUBEVENT(CROS3_SUBEVENT)
{
  select several
    {
      norevisit external ccb[0] = EXT_CROS3(ccb_id=1);
      norevisit external ccb[1] = EXT_CROS3(ccb_id=2);
    }
}

#include "cros3_v27rewrite.spec"

SUBEVENT(CROS3_REWRITE_SUBEVENT)
{
  select several
    {
      norevisit ccb[0] = CROS3_REWRITE(ccb_id=1);
      norevisit ccb[1] = CROS3_REWRITE(ccb_id=2);
    }
}

