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

#ifndef LCP_PHISPARE_H
#define LCP_PHISPARE_H

#include <LCP.h>

namespace cds_static
{

	class LCP_PhiSpare: public LCP
	{
		private:
			int q;
			size_t n;			 //length of LCP
			int *plcp;			 //psi lcp

			LCP_PhiSpare();

		public:
			/*Karkkainen, Manzini, and Puglisi encoding of the LCP,
			 * @param csa The compressed suffix array of the text
			 * @param text The text
			 * @param n  The length of the text
			 * @param _q The sample period to use (if q = -1 its mean that we will not use any sample)
			 * */
			LCP_PhiSpare(TextIndex *csa, char *text, size_t _n, int _q = 32);
			virtual size_t get_LCP(size_t i, TextIndex *csa) const;
			virtual size_t get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const;
			virtual size_t getSize() const;
			virtual void save(ofstream & fp) const;
			static LCP_PhiSpare * load(ifstream & fp);
			virtual ~LCP_PhiSpare();
	};
};
#endif
