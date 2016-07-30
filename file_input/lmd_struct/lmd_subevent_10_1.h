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

#ifndef __LMD_SUBEVENT_10_1_H__
#define __LMD_SUBEVENT_10_1_H__

#include "lmd_event_10_1.h"

/*  ================= GSI VME Subevent header =======================  */

typedef struct
{
  lmd_event_header_little_endian _header;

  sint16  i_procid;     /*  Processor ID [as loaded from VAX] */
  sint8   h_subcrate;   /*  Subcrate number */
  sint8   h_control;    /*  Processor type code */
} lmd_subevent_10_1_little_endian;

typedef struct
{
  lmd_event_header_big_endian _header;

  sint8   h_control;    /*  Processor type code */
  sint8   h_subcrate;   /*  Subcrate number */
  sint16  i_procid;     /*  Processor ID [as loaded from VAX] */
} lmd_subevent_10_1_big_endian;

typedef lmd_subevent_10_1_big_endian        lmd_subevent_10_1_network_order;
#define lmd_subevent_10_1_host              HOST_ENDIAN_TYPE(lmd_subevent_10_1)

/* ------------------------------------------------------------------ */

typedef struct
{
  lmd_event_header_network_order _header;

  // these are not used for anything,
  // so avoid them in repacked data

  // sint8   h_control;   /*  Processor type code */
  // sint8   h_subcrate;   /*  Subcrate number */
  // sint16 i_procid;   /*  Processor ID [as loaded from VAX] */
} lmd_subevent_10_1_x_network_order;

/* ------------------------------------------------------------------ */

#endif// __LMD_SUBEVENT_10_1_H__
