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

#ifndef PSV_H
#define PSV_H

#include <LCP.h>
#include <BitSequence.h>

namespace cds_static
{
	class PSV
	{

		private:
			PSV();
			virtual void create_first_level(LCP *lcp, TextIndex *csa);
			virtual void create_level_r(LCP *lcp, size_t level, TextIndex *csa);
			virtual void create_last_level(LCP *lcp, TextIndex *csa);
			virtual size_t find_PSV_r(size_t v, size_t level, TextIndex *csa, LCP *lcp) const;

			size_t r;			 /*levels*/
			size_t b;			 /*block size*/
			size_t n;			 /*number of element of lcp*/
			BitSequence **P;	 /*bitarray with the pioneer for each level*/
			BitSequence **R;	 /*bitarray with the pioneer and their answers*/
			uint *A;			 /*stored values for the last level*/
			size_t b_A;			 /*bits needed per value in A*/

		public:
			/*PSV Data structure base on Fisher, Makinnen, and Navarro "Faster Entropy-Bounded Compressed Suffix Trees"
			 * @param lcp Any LCP array data structure
			 * @param levels Number of levels that the recursion will use (in general is better to use only 2 or 3 levels).
			 * @param block Block size that will be use
			 * @param csa The compresses suffix array*/
			PSV(LCP *lcp, size_t levels, size_t block, TextIndex *csa);

			//Return the position (plus 1) where is the PSV of v
			virtual size_t find_PSV(size_t v, TextIndex *SA, LCP *lcp) const;

			//Memory use for PSV in bytes
			virtual size_t getSize() const;

			virtual void save(ofstream & fp) const;

			static PSV* load(ifstream & fp);

			virtual ~PSV();
	};

};
#endif
