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

#ifndef LCP_DAC_H
#define LCP_DAC_H

#include <factorization.h>
#include <LCP.h>

namespace cds_static
{

	class LCP_DAC: public LCP
	{

		private:
			size_t *next_p;		 //usefull for get_seq_LCP
			factorization *rep;
			LCP_DAC();

		public:
			size_t n;

			/* LCP encoding using Susana's static factorization,
			 * @param csa The compressed suffix array of the text
			 * @param text The text
			 * @param n  The length of the text
			 * */
			LCP_DAC(TextIndex *csa, char *text, size_t n);
			virtual size_t get_LCP(size_t i, TextIndex *csa) const;
			virtual size_t get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const;
			virtual size_t getSize() const;
			virtual void save(ofstream & fp) const;
			static LCP_DAC * load(ifstream & fp);
			virtual ~LCP_DAC();
	};
};
#endif
