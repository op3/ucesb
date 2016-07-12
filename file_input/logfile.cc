/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2016  Michael Munch
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

#include "error.hh"
#include "logfile.hh"
#include "forked_child.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

logfile::logfile(const char* file)
{
  _fd = open(file, O_CREAT | O_WRONLY | O_APPEND,
	     S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if (_fd == -1)
    ERROR("Failed to open logfile '%s'", file);
  INFO(0,"Opened logfile '%s'.", file);
}

logfile::~logfile()
{
  if (_fd != -1)
    {
      if (close(_fd) != 0)
	perror("close");
    }
}

size_t logfile::append(const char* msg)
{
  if (_fd == -1)
    return (size_t) -1;
  return full_write(_fd, msg, strlen(msg));
}

