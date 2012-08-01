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

#ifndef NPR_CN_H
#define NPR_CN_H

#include <NPR.h>

namespace cds_static
{
	/*****************************
	There are r=(log(n)/log(b))-1 levels
	****************************/

	class NPR_CN: public NPR
	{
		private:
			NPR_CN();
			void create_first_level(LCP *lcp, TextIndex *csa);
			void create_next_level(size_t r);
			size_t find_NSV_level_r(size_t value_v, size_t next_pos, size_t r) const;
			size_t find_PSV_level_r(size_t value_v, size_t next_pos, size_t r) const;
			size_t find_RMQ(size_t x, size_t y, size_t r, size_t *min_r) const;

			size_t n;			 //size of lcp
			size_t b;			 //block Size
			size_t bits_b;		 //bits for representated b
			size_t l;			 //number of levels
			uint **min_level;	 //array for each level that contain the min value of each block
			uint **min_pos;		 // array for each level that contain the local position of the min value of each block
			uint *level_size;	 //size of each level

		public:
			/*NPR Data structure base on Canovas and Navarro "Practical Compressed Suffix Trees"
			 * @param lcp Any LCP array data structure
			 * @param block_Size Block size that will be used
			 * @param csa The compresses suffix array
			 */
			NPR_CN(LCP *lcp, size_t block_Size, TextIndex *csa);

			virtual ~NPR_CN();

			// Return the position of NSV[i]
			virtual size_t find_NSV(size_t i,  TextIndex *csa, LCP *lcp) const;

			// Return the position of PSV[i]+1
			virtual size_t find_PSV(size_t i,  TextIndex *csa, LCP *lcp) const;

			virtual size_t bwd_PSV(size_t i, TextIndex *csa, LCP *lcp, size_t d) const;

			virtual size_t fwd_NSV(size_t i, TextIndex *csa, LCP *lcp, size_t d) const;

			//Return the left-most position  j, x<= j <= y  , where is the minimun in the range [x,y]
			virtual size_t find_RMQ(size_t x, size_t y, TextIndex *csa, LCP *lcp) const;

			virtual  size_t getSize() const;

			virtual void save(ofstream & fp) const;

			static NPR_CN * load(ifstream & fp);

	};

};
#endif
