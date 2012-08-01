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

#ifndef LCP_FMN_H
#define LCP_FMN_H

#include <BitSequence.h>
#include <LCP.h>

using namespace cds_static;

namespace cds_static
{

	class LCP_FMN: public LCP
	{
		private:
			LCP_FMN();
			virtual void generate_OZ(BitSequence *U, uint **O, uint **Z, uint length);

			BitSequence *O, *Z;
			size_t b_length;	 //length of the bitmaps
			size_t OZ_type;

		public:

			/*Johannes, Veli & Gonzalo LCP encode
			 * @param csa The compressed suffix array of the text
			 * @param text The text
			 * @param n  The length of the text
			 * @param op_rs Choose rank/select implementation that will be used (must be RRR02_HDR or SDARRAY_HDR)
			 * */
			LCP_FMN(TextIndex *csa, char *text, size_t n, size_t op_rs=RRR02_HDR);

			LCP_FMN(LCP *lcp, TextIndex *csa, size_t n, size_t op_rs=RRR02_HDR);

			/**Return LCP[i]*/
			virtual size_t get_LCP(size_t i, TextIndex *csa) const;

			/**Return LCP[i], being faster if sequential acceses to the LCP had been done*/
			virtual size_t get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const;

			/** Returns the size of the structure in bytes */
			virtual size_t getSize() const;

			/** Stores the structure given a file pointer, return 0 in case of success */
			virtual void save(ofstream & fp) const;

			/** Reads a LCP determining the type */
			static LCP_FMN * load(ifstream & fp);

			virtual ~LCP_FMN();
	};
};
#endif
