
/* mman.h
   Copyright (C) 2005, K. Sadakane, all rights reserved.

   This file contains an implementation of memory manager

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef _MYMMAP_H_
#define _MYMMAP_H_

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

#ifdef WIN32
#define PAGE_READONLY          0x02
#define SECTION_MAP_READ    0x0004
#define FILE_MAP_READ       SECTION_MAP_READ
#endif

namespace cds_static
{

	typedef struct
	{
		void *addr;
		int len;
		#ifdef WIN32
		HANDLE h1,h2;
		#else
		int fd;
		#endif
	} MMAP;

	MMAP *mymmap (char *fname);
	int mymunmap (MMAP *m);

};
#endif
