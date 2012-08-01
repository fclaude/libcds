/* comparray4.cpp
   Copyright (C) 2005, K. Sadakane, all rights reserved.

   This file contains an implementation of CSA.
   For more information, see

   K. Sadakane. Compressed text databases with efficient query
	 algorithms based on the compressed suffix array.
	 In Proceedings 11th Annual International Symposium on Algorithms
	 and Computation (ISAAC)}, LNCS v. 1969, pages 410--421, 2000.

   K. Sadakane. Succinct representations of lcp information and
	 improvements in the compressed suffix arrays.
	 In Proceedings 13th Annual ACM-SIAM Symposium on Discrete
	 Algorithms (SODA), 2002.

   K. Sadakane. New text indexing functionalities of the compressed
	 suffix arrays. Journal of Algorithms, 48(2):294--313, 2003.

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

#ifndef _COMPARRAY_H_
#define _COMPARRAY_H_

#include <libcdsBasics.h>
using namespace cds_utils;
#define USE_MMAP

#ifdef USE_MMAP
#include <mmap.h>
#endif

#define SIGMA 256

namespace cds_static
{

	using namespace std;

	typedef struct csa
	{
		int n,m;
		int l,two,two2;
		int K[SIGMA+2],C[SIGMA+1],C2[SIGMA+1];
		int *SA,*ISA,*R;
		unsigned short *B;		 /* bit */
		#ifdef USE_MMAP
		MMAP *mapp,*mapi;
		int p_size, i_size, r_size, sa_size, isa_size;
		#endif
	} CSA;

	int csa_psi(CSA *SA,int i);
	int csa_T(CSA *SA,int i);
	int csa_inverse(CSA *SA, int suf);
	void csa_decode(unsigned char *p,CSA *SA,int suf,int len);
	void csa_decode2(unsigned char *p,CSA *SA,int pos,int len);
	int csa_write(CSA *SA,char *fname1,char *fname2);
	void csa_new(int n, int *p, unsigned char *s, char *fname1, char *fname2, int rankb_w,int rankb_w2);
	int csa_read(CSA *SA,char *fname1,char *fname2);
	int initranktables(void);
	void mkdecodetable(void);
	int csa_lookup(CSA *SA, int i);
	int *csa_batchlookup(CSA *SA,int l, int r);
	unsigned long *csa_batchlookup2(CSA *SA,int l, int r);
	int csa_bsearch(unsigned char *key,int keylen,CSA *SA,int *li,int *ri);
	void csa_decode1line(unsigned char *p,CSA *SA,int suf,int maxlen);
	void csa_decodeall(unsigned char *p,CSA *SA);
	void csa_save(CSA *csa, ofstream & fp);
	CSA *csa_load(ifstream & fp);
	void csa_free(CSA *csa);
	void csa_init(CSA *csa);
};
#endif
