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

#ifndef LCP_NAIVE_H
#define LCP_NAIVE_H

#include <LCP.h>

namespace cds_static
{

	class LCP_naive: public LCP
	{
		private:
			LCP_naive();
			uint *lcp_array;
			size_t  b;
			size_t  length;

		public:

			LCP_naive(TextIndex *csa, char *text, size_t n);

			/**Return LCP[i]*/
			virtual size_t get_LCP(size_t i, TextIndex *csa) const;

			/**Return LCP[i], being faster if sequential acceses to the LCP had been done*/
			virtual size_t get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const;

			/** Returns the size of the structure in bytes */
			virtual  size_t getSize() const;

			/** Stores the structure given a file pointer, return 0 in case of success */
			virtual void save(ofstream & fp) const;

			/** Reads a LCP determining the type */
			static LCP_naive * load(ifstream & fp);

			virtual ~LCP_naive();
	};
};
#endif
