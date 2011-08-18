/* basics.h
 * Copyright (C) 2010, Rodrigo Canovas, all rights reserved.
 *
 * Some preliminary stuff
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * */

#ifndef _BASICS_SDARRAY_H
#define _BASICS_SDARRAY_H
#include <libcdsBasics.h>

namespace cds_utils{
	typedef unsigned long long qword;
	static const unsigned int logD = 5;
	static const unsigned int PBS = (sizeof(uint)*8);
  static const unsigned int D = (1<<logD);
  static const unsigned int logM = 5;
	static const unsigned int M = (1<<logM);
	static const unsigned int logP = 8;
	static const unsigned int P = (1<<logP);
	static const unsigned int logLL = 16;             // size of word
	static const unsigned int LL = (1<<logLL);
	static const unsigned int logLLL = 5;
	static const unsigned int LLL = (1<<logLLL);
	static const unsigned int logL = (logLL-1-5);
	static const unsigned int L = (1<<logL);
	static const unsigned int logRR = 8; 
	static const unsigned int RR  = (1<<logRR); 
	static const unsigned int logRRR = 6; 
	static const unsigned int RRR = (1<<logRRR);


	inline int __blog(const int x1) {
		int x=x1;
		int l;
		l = 0;
		while (x>0) {
			x>>=1;
			l++;
		}
		return l;
	}

	inline int __setbit(uint *B, const int i, const int x) {
		int j,l;
		j = i / D;
		l = i % D;
		if (x==0) B[j] &= (~(1<<(D-1-l)));
		else if (x==1) B[j] |= (1<<(D-1-l));
		else {
			printf("error __setbit x=%d\n",x);
			exit(1);
		}
		return x;
	}
	
	inline int __setbit2(unsigned char *B, const int i, const int x) {
		int j,l;
		j = i / 8;
		l = i % 8;
		if (x==0) B[j] &= (~(1<<(8-1-l)));
		else if (x==1) B[j] |= (1<<(8-1-l));
		else {
			printf("error __setbit2 x=%d\n",x);
			exit(1);
		}
		return x;
	}

	inline int __setbits(uint *B, const int i, const int d, const int x) {
		int j;
		for (j=0; j<d; j++) {
			__setbit(B,i+j,(x>>(d-j-1))&1);
		}
		return x;
	}

	inline int __getbit(const uint *B, const int i) {
		int j,l;
		j = i >> logD;
		l = i & (D-1);
		return (B[j] >> (D-1-l)) & 1;
	}

	inline int __getbit2(const unsigned char *B, const int i) {
		int j,l;
		 j = i >> 3;
		 l = i & (8-1);
		 return (B[j] >> (8-1-l)) & 1;
	}

#if 1
	inline uint __getbits(const uint *B, const int i1, const int d) {
		qword x,z;
		int i = i1;
		B += (i >> logD);
		i &= (D-1);
		if (i+d <= 2*(int)D) {
			x = (((qword)B[0]) << D) + B[1];
			x <<= i;
			x >>= (D*2-1-d);
			x >>= 1;
		}
		else {
			x = (((qword)B[0])<<D)+B[1];
			z = (x<<D)+B[2];
			x <<= i;
			x &= (((qword)1L<<D)-1)<<D;
			z <<= i;
			z >>= D;
			x += z;
			x >>= (2*D-d);
		}
		return x;
	}
#endif
#if 0
	inline uint __getbits(uint *B, int i, int d) {
		uint j,x;
		x = 0;
		for (j=0; j<d; j++) {
			x <<= 1;
			x += __getbit(B,i+j);
		}
		return x;
	}
#endif


	static const unsigned int _popCount[] = {
		0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
		1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
		1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
		1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
		3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
		1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
		3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
		3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
		3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
		4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
	};

	inline unsigned int __popCount(const uint x) {
		uint r;
		r = x;
		r = ((r & 0xaaaaaaaa)>>1) + (r & 0x55555555);
		r = ((r & 0xcccccccc)>>2) + (r & 0x33333333);
		r = ((r>>4) + r) & 0x0f0f0f0f;
		r = (r>>8) + r;
		r = ((r>>16) + r) & 63;
		return r;
	}

	inline unsigned int __popCount8(const uint x) {
		uint r;
#if 1
		r = x;
		r = ((r & 0xaa)>>1) + (r & 0x55);
		r = ((r & 0xcc)>>2) + (r & 0x33);
		r = ((r>>4) + r) & 0x0f;
#else
		r = _popCount[x & 0xff];
#endif
		return r;
	}

};

#endif

