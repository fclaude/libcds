/* Copyright (C) 2010, Rodrigo Cnovas, all rights reserved.
 *
 *This library is free software; you can redistribute it and/or
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
 *
 */

#ifndef _RMQ_succinct_h
#define _RMQ_succinct_h

#define MEM_COUNT

#include <libcdsTrees.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
using namespace std;
using namespace cds_utils;

typedef unsigned char DTsucc;
typedef unsigned short DTsucc2;

namespace cds_static
{
	class RMQ_succinct
	{
		private:
			RMQ_succinct();
		public:
			/*array*/
			int *a;

			/* liefert RMQ[i,j]*/
			virtual unsigned int query(unsigned int, unsigned int);

			/*Johannes RMQ encode
			 * @param a The array
			 * @param n Number of elements in the array
			 * */
			RMQ_succinct(int* a, unsigned int n);

			uint getSize();
			void  save(ofstream & fp);
			RMQ_succinct * load(ifstream & fp);

			~RMQ_succinct();

		protected:
			/* size of array a*/
			unsigned int n;
			/* table M for the out-of-block queries (contains indices of block-minima)*/
			DTsucc** M;
			/* because M just stores offsets (rel. to start of block), this method re-calculates the true index:*/
			inline unsigned int m(unsigned int k, unsigned int block) { return M[k][block]+(block*sprime); }
			/* depth of table M:*/
			unsigned int M_depth;
			/* table M' for superblock-queries (contains indices of block-minima)*/
			unsigned int** Mprime;
			/* depth of table M':*/
			unsigned int Mprime_depth;
			/* type of blocks*/
			DTsucc2 *type;
			/* precomputed in-block queries*/
			DTsucc** Prec;
			/* microblock size*/
			unsigned int  s;
			/* block size*/
			unsigned int sprime;
			/* superblock size*/
			unsigned int sprimeprime;
			/* number of blocks (always n/sprime)*/
			unsigned int nb;
			/* number of superblocks (always n/sprimeprime)*/
			unsigned int nsb;
			/* number of microblocks (always n/s)*/
			unsigned int nmb;
			/* return microblock-number of entry i:*/
			inline unsigned int microblock(unsigned int i) { return i/s; }
			/* return block-number of entry i:*/
			inline unsigned int block(unsigned int i) { return i/sprime; }
			/* return superblock-number of entry i:*/
			inline unsigned int superblock(unsigned int i) { return i/sprimeprime; }
			/* precomputed Catalan triangle (17 is enough for 64bit computing):*/
			static const unsigned int Catalan[17][17];
			/* minus infinity (change for 64bit version)*/
			static const int minus_infinity;
			/* stuff for clearing the least significant x bits (change for 64-bit computing)*/
			static const DTsucc HighestBitsSet[8];
			virtual DTsucc clearbits(DTsucc, unsigned int);
			/* Least Significant Bits for 8-bit-numbers:*/
			static const char LSBTable256[256];
			/* return least signigicant bit in constant time (change for 64bit version)*/
			virtual unsigned int lsb(DTsucc);
			/* the following stuff is for fast base 2 logarithms: (currently only implemented for 32 bit numbers)*/
			static const char LogTable256[256];
			virtual unsigned int log2fast(unsigned int);
	};
};
#endif
