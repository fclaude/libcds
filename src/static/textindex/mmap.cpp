/* mmap.c
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

#include <mmap.h>
#include <unistd.h>

namespace cds_static
{

	#ifdef WIN32
	MMAP *  mymmap (char *fname) {
		void *base;
		HANDLE fd,h;
		int len;
		MMAP *m;
		m = malloc(sizeof(*m));
		if (m==NULL) {perror("mymmap malloc");  exit(1);}
		fd = CreateFile(fname,GENERIC_READ,0,NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,NULL);
		if (fd==INVALID_HANDLE_VALUE) {
			printf("createfile\n");
			exit(1);
		}
		m->h1 = fd;
		len = GetFileSize(fd,0);
		m->len = len;
		h = CreateFileMapping (fd, NULL, PAGE_READONLY, 0, len, NULL);
		if (h==NULL) {
			printf("createfilemapping\n");
			exit(1);
		}
		m->h2 = h;
		base = MapViewOfFile (h, FILE_MAP_READ, 0, 0, len);
		if (base==NULL) {
			printf("mapviewoffile\n");
			return NULL;
		}
		m->addr = base;
		return m;
	}

	int mymunmap (MMAP *m) {
		UnmapViewOfFile (m->addr);
		CloseHandle(m->h2);
		CloseHandle(m->h1);
		return 0;
	}

	#else

	MMAP *mymmap (char *fname) {
		int fd;
		int len;
		MMAP *m;
		struct stat statbuf;
		caddr_t base;
		m =(MMAP *) malloc(sizeof(*m));
		if (m==NULL) {perror("mymmap malloc");  exit(1);}

		stat(fname,&statbuf);
		len = statbuf.st_size;
		fd = open(fname,O_RDONLY);
		base = (caddr_t)mmap(0,len,PROT_READ,MAP_SHARED,fd,0);
		if (base==(caddr_t)-1) {
			perror("mmap1\n");
			exit(1);
		}
		m->addr = (void *)base;
		m->fd = fd;
		m->len = len;
		return m;
	}

	int mymunmap (MMAP *m) {
		if (munmap(m->addr,m->len)==-1) {
			perror("munmap 1:");
		}
		close(m->fd);
		return 0;
	}

};
#endif
