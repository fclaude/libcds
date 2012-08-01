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

#ifndef NPR_H
#define NPR_H

#include <LCP.h>

namespace cds_static
{

	static const size_t FMN_NPR = 2;
	static const size_t CN_NPR  = 3;

	class NPR
	{
		public:
			size_t npr_type;

			virtual ~NPR() {};

			// Return the position of the next smaller value of i (NSV[i])
			virtual size_t find_NSV(size_t i,  TextIndex *csa, LCP *lcp) const= 0;

			// Return the position of the previous smaller value of i plus one (PSV[i]+1)
			virtual size_t find_PSV(size_t i,  TextIndex *csa, LCP *lcp) const= 0;

			//Return the first previous position, from i, that contains a value < d
			virtual size_t bwd_PSV(size_t i, TextIndex *csa, LCP *lcp, size_t d) const= 0;

			//Return the first next position, from i, that contains a value < d
			virtual size_t fwd_NSV(size_t i, TextIndex *csa, LCP *lcp, size_t d) const= 0;

			//Return the range minimum query over the LCP array, between the positions x and y
			virtual size_t find_RMQ(size_t x, size_t y, TextIndex *csa, LCP *lcp) const= 0;

			//Return the size, in bytes, of the NPR structure
			virtual  size_t getSize() const = 0;

			virtual void save(ofstream & fp) const=0;

			static NPR * load(ifstream & fp);
	};

};
#include <NPR_FMN.h>
#include <NPR_CN.h>
#endif
