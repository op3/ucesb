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

#ifndef __ERROR_HH__
#define __ERROR_HH__

#include <stdlib.h>



#if defined __GNUC__ && __GNUC__ < 3 // 2.95 do not do iso99 variadic macros
#define ERROR(__VA_ARGS__...) do { \
  fprintf(stderr,"%s:%d: ",__FILE__,__LINE__);  \
  fprintf(stderr,__VA_ARGS__);  \
  fputc('\n',stderr);           \
  exit(1);                      \
} while (0)
#else
#define ERROR(...) do {         \
  fprintf(stderr,"%s:%d: ",__FILE__,__LINE__);  \
  fprintf(stderr,__VA_ARGS__);  \
  fputc('\n',stderr);           \
  exit(1);                      \
} while (0)
#endif

#if defined __GNUC__ && __GNUC__ < 3 // 2.95 do not do iso99 variadic macros
#define WARNING(__VA_ARGS__...) do { \
  fprintf(stderr,"%s:%d: ",__FILE__,__LINE__);  \
  fprintf(stderr,__VA_ARGS__);  \
  fputc('\n',stderr);           \
} while (0)
#else
#define WARNING(...) do {       \
  fprintf(stderr,"%s:%d: ",__FILE__,__LINE__);  \
  fprintf(stderr,__VA_ARGS__);  \
  fputc('\n',stderr);           \
} while (0)
#endif

#if defined __GNUC__ && __GNUC__ < 3 // 2.95 do not do iso99 variadic macros
#define ERROR_LOC(loc,__VA_ARGS__...) do { \
  (loc).print_lineno(stderr);    \
  fputc(' ',stderr);             \
  fprintf(stderr,__VA_ARGS__);   \
  fputc('\n',stderr);            \
  exit(1);                       \
} while (0)
#else
#define ERROR_LOC(loc,...) do { \
  (loc).print_lineno(stderr);    \
  fputc(' ',stderr);             \
  fprintf(stderr,__VA_ARGS__);   \
  fputc('\n',stderr);            \
  exit(1);                       \
} while (0)
#endif

#if defined __GNUC__ && __GNUC__ < 3 // 2.95 do not do iso99 variadic macros
#define ERROR_LOC_PREV(loc,loc2,__VA_ARGS__...) do { \
  (loc).print_lineno(stderr);    \
  fputc(' ',stderr);             \
  fprintf(stderr,__VA_ARGS__);   \
  fputc('\n',stderr);            \
  (loc2).print_lineno(stderr);   \
  fprintf(stderr," Previous here.\n"); \
  exit(1);                       \
} while (0)
#else
#define ERROR_LOC_PREV(loc,loc2,...) do { \
  (loc).print_lineno(stderr);    \
  fputc(' ',stderr);             \
  fprintf(stderr,__VA_ARGS__);   \
  fputc('\n',stderr);            \
  (loc2).print_lineno(stderr);   \
  fprintf(stderr," Previous here.\n"); \
  exit(1);                       \
} while (0)
#endif

#if defined __GNUC__ && __GNUC__ < 3 // 2.95 do not do iso99 variadic macros
#define WARNING_LOC(loc,__VA_ARGS__...) do { \
  (loc).print_lineno(stderr);    \
  fputc(' ',stderr);             \
  fprintf(stderr,__VA_ARGS__);   \
  fputc('\n',stderr);            \
} while (0)
#else
#define WARNING_LOC(loc,...) do { \
  (loc).print_lineno(stderr);    \
  fputc(' ',stderr);             \
  fprintf(stderr,__VA_ARGS__);   \
  fputc('\n',stderr);            \
} while (0)
#endif




#endif//__ERROR_HH__


