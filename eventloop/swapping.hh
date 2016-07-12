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

#ifndef __SWAPPING_HH__
#define __SWAPPING_HH__

#include "byteswap_include.h"
#include "byteorder_include.h"

#include <assert.h>
#include <stdlib.h>

#include <sys/types.h>

#if BYTE_ORDER == BIG_ENDIAN
# define HOST_ENDIAN_TYPE(x)  x##_big_endian
#endif
#if BYTE_ORDER == LITTLE_ENDIAN
# define HOST_ENDIAN_TYPE(x)  x##_little_endian
#endif

inline uint32 bswap(uint32 src) { return bswap_32(src); }
inline uint16 bswap(uint16 src) { return bswap_16(src); }
inline uint8  bswap(uint8 src)  { return src; } // just to make template happy...

template<typename T>
inline void byteswap(T* ptr,size_t len)
{
  assert (len % sizeof(T) == 0);

  uint* p = (uint *) ptr;

  for (size_t i = len / sizeof(T); i; i--)
    {
      *p = bswap(*p);
      p++;
    }
}

#endif//__SWAPPING_HH__
