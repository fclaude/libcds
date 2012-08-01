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

#ifndef NPR_FMN_H
#define NPR_FMN_H

#include <NPR.h>
#include <NSV.h>
#include <PSV.h>
#include <RMQ_succinct_lcp.h>

namespace cds_static
{

	class NPR_FMN: public NPR
	{
		private:
			NSV *nsv;
			PSV *psv;
			RMQ_succinct_lcp *rmq;

			NPR_FMN();

		public:

			/*NPR Data structure base on Fisher, Makinnen, and Navarro "Faster Entropy-Bounded Compressed Suffix Trees"
			 * @param lcp Any LCP array data structure
			 * @param block_Size Block size that will be used
			 * @param csa The compresses suffix array
			 * @param levels Number of levels that the recursion will use. (in general is better to use only 2 or 3 levels)
			 * */
			NPR_FMN(LCP *lcp, size_t block_Size, TextIndex *csa, size_t levels = 2);

			virtual ~NPR_FMN();

			virtual size_t find_NSV(size_t i,  TextIndex *csa, LCP *lcp) const;

			virtual size_t find_PSV(size_t i,  TextIndex *csa, LCP *lcp) const;

			virtual size_t bwd_PSV(size_t i, TextIndex *csa, LCP *lcp, size_t d) const;

			virtual size_t fwd_NSV(size_t i, TextIndex *csa, LCP *lcp, size_t d) const;

			virtual size_t find_RMQ(size_t x, size_t y, TextIndex *csa, LCP *lcp) const;

			virtual  size_t getSize() const;

			virtual void save(ofstream & fp) const;

			static NPR_FMN * load(ifstream & fp);
	};

};
#endif
