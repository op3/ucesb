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

#ifndef __SIGNAL_ID_RANGE_HH__
#define __SIGNAL_ID_RANGE_HH__

#include "../common/signal_id.hh"

struct sig_part_range
{
public:
  sig_part_range(const char *name)
  {
    _id._name = name;
    _type = SIG_PART_NAME;
  }

  sig_part_range(int index)
  {
    _id._index._min = index;
    _id._index._max = index;
    _type = SIG_PART_INDEX;
  }

  sig_part_range(int min,int max)
  {
    _id._index._min = min;
    _id._index._max = max;
    _type = SIG_PART_INDEX;
  }

public:
  int _type;

  union
  {
    const char *_name;
    struct
    {
      int         _min;
      int         _max;
    } _index;
  } _id;
};

typedef std::vector<sig_part_range> sig_part_range_vector;

class signal_id_range
{
public:
  signal_id_range()
  {
  }

public:
  sig_part_range_vector _parts;

public:
  void push_back(const char *name)
  {
    _parts.push_back(sig_part_range(name));
  }

  void push_back(int min,int max)
  {
    _parts.push_back(sig_part_range(min,max));
  }

  void push_back_wildcard()
  {
    sig_part_range rng((const char *) NULL); // dummy argument
    rng._type = SIG_PART_WILDCARD; // overwrite the type
    _parts.push_back(rng);
  }

  void push_back_wildcards()
  {
    sig_part_range rng((const char *) NULL); // dummy argument
    rng._type = SIG_PART_WILDCARDS; // overwrite the type
    _parts.push_back(rng);
  }

public:
  bool encloses(const signal_id &id,bool sub_channel);

};

void dissect_name_range(const char *name,
			signal_id_range& id);

#endif//__SIGNAL_ID_RANGE_HH__
