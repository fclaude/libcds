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

#ifndef LCP_SAD_H
#define LCP_SAD_H

#include <BitSequence.h>
#include <LCP.h>

namespace cds_static
{
	class LCP_Sad: public LCP
	{
		private:
			LCP_Sad();
			size_t U_type;

		public:
			BitSequence *U;
			size_t U_length;	 //length of bitmap U

			/*Create Sadakane encoding of the LCP, that use 2n+o(n) bits,
			 * @param csa The compressed suffix array of the text
			 * @param text The text
			 * @param n  The length of the text
			 * */
			LCP_Sad(TextIndex *csa, char *text, size_t n, size_t op_rs = BRW32_HDR);

			/*Create the LCP representation starting from a precomputed LCP array*/
			LCP_Sad(LCP *lcp, TextIndex *csa, size_t n, size_t op_rs = BRW32_HDR);

			/**Return LCP[i]*/
			virtual size_t get_LCP(size_t i, TextIndex *csa) const;

			/**Return LCP[i], being faster if sequential acceses to the LCP had been done*/
			virtual size_t get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const;

			/** Returns the size of the structure in bytes */
			virtual  size_t getSize() const;

			/** Stores the structure given a file pointer*/
			virtual void save(ofstream & fp) const;

			/** Reads a LCP determining the type */
			static LCP_Sad * load(ifstream & fp);

			virtual ~LCP_Sad();
	};
};
#endif
