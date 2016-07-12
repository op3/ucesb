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

#ifndef __STR_SET_H__
#define __STR_SET_H__

#include <string.h>
#include <unistd.h>

/* Normal C strings and not std::string is used because of the
 * overhead of using std::string.  And it also seems to have a few
 * undesirable features.  See string_discussion.html in the SGI stl manual.
 *
 * Instead, I operate on new/delete C strings.
 */

struct compare_str_less
{
  bool operator()(const char *s1,const char *s2) const
  {
    return strcmp(s1,s2) < 0;
  }
};

const char *find_str_identifiers(const char *str);

const char *find_str_strings(const char *str,size_t len = (size_t) -1);

const char *find_concat_str_strings(const char *s1,const char *s2,const char *s3);



#endif// __STR_SET_H__

