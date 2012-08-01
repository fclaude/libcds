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

#ifndef _RMQ_succinct_LCP_H
#define _RMQ_succinct_LCP_H

#define MEM_COUNT

#include <LCP.h>

typedef unsigned char DTsucc;
typedef unsigned short DTsucc2;

namespace cds_static
{

	class RMQ_succinct_lcp
	{
		private:
			RMQ_succinct_lcp();

		public:
			// liefert RMQ[i,j]
			virtual uint query(uint i, uint j,  TextIndex *csa, LCP *lcp);

			/*Johannes RMQ encode
			 * @param lcp The LCP array
			 * @param n Number of elements in the array
			 * @param csa The compressed suffix array
			 **/
			RMQ_succinct_lcp(LCP *lcp, TextIndex *csa);

			virtual uint getSize();

			virtual void save(ofstream & fp);

			static RMQ_succinct_lcp * load(ifstream & fp);

			virtual ~RMQ_succinct_lcp();

		protected:
			// array 	DT *a;
			uint n;				 //number of element of lcp
			DTsucc** M;			 // table M for the out-of-block queries (contains indices of block-minima)

			uint M_depth;		 //depth of table M
			uint **Mprime;		 // table M' for superblock-queries (contains indices of block-minima)
			uint Mprime_depth;	 // depth of table M':
			DTsucc2 *type;		 // type of blocks
			DTsucc** Prec;		 // precomputed in-block queries
			uint s;				 // microblock size
			uint sprime;		 // block size
			uint sprimeprime;	 // superblock size
			uint nb;			 // number of blocks (always n/sprime)
			uint nsb;			 // number of superblocks (always n/sprimeprime)
			uint nmb;			 // number of microblocks (always n/s)

			// because M just stores offsets (rel. to start of block), this method
			// re-calculates the true index:
			inline uint m(uint k, uint block) {
				return M[k][block]+(block*sprime);
			}

			// return microblock-number of entry i:
			inline uint microblock(uint i) { return i/s; }

			// return block-number of entry i:
			inline uint block(uint i) { return i/sprime; }

			// return superblock-number of entry i:
			inline uint superblock(uint i) { return i/sprimeprime; }

			// precomputed Catalan triangle (17 is enough for 64bit computing):
			static const uint Catalan[17][17];

			// minus infinity (change for 64bit version)
			static const int minus_infinity;

			// stuff for clearing the least significant x bits (change for 64-bit computing)
			static const DTsucc HighestBitsSet[8];

			DTsucc clearbits(DTsucc, uint);

			// Least Significant Bits for 8-bit-numbers:
			static const char LSBTable256[256];

			// return least signigicant bit in constant time (change for 64bit version)
			uint lsb(DTsucc);

			// the following stuff is for fast base 2 logarithms:
			// (currently only implemented for 32 bit numbers)
			static const char LogTable256[256];

			uint log2fast(uint);
	};

};
#endif
