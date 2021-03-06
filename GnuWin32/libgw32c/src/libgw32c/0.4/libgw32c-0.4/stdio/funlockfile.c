/* Unlock I/O stream.  Singlethreaded version.
   Copyright (C) 1996, 1997, 2000, 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <stdio.h>
#include <windows.h>

#undef _IO_funlockfile

void
__funlockfile (FILE *stream)
{
  HANDLE hf;
  int fd;
  LARGE_INTEGER li;

  fd = _fileno (stream);
  hf = (HANDLE) _get_osfhandle (fd);
  li.QuadPart = _filelengthi64 (fd);
  UnlockFile (hf, 0, 0, li.LowPart, li.HighPart);
}
weak_alias (__funlockfile, _IO_funlockfile)
weak_alias (__funlockfile, funlockfile);
