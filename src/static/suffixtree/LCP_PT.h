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

#ifndef LCP_PT_H
#define LCP_PT_H

#include <RMQ_succinct.h>
#include <LCP.h>

namespace cds_static
{

	class LCP_PT: public LCP
	{
		private:
			LCP_PT();

			virtual int calc_delta(unsigned int i, unsigned int j) const;

			int logv;
			size_t length;
			int mask;
			int v;
			int m;				 /*size of rev_ess*/
			int *rev_cover;
			int *delta;
			int cover_size;
			int *rev_ess;
			RMQ_succinct *rmq;

		public:
			/*Puglisi and Turpin encoding of the LCP,
			 * @param csa The compressed suffix array of the text
			 * @param text The text
			 * @param n  The length of the text
			 * @param _logv logarithm (base 2) of the period of the difference cover you would like to use. Valid values for "logv" are 2,3,4,5,6,7 and 8
			 * */
			LCP_PT(TextIndex *csa, char *text, size_t n, int _logv=8);
			virtual size_t get_LCP(size_t i, TextIndex *csa) const;
			virtual size_t get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const;
			virtual size_t getSize() const;
			virtual void save(ofstream & fp) const;
			static LCP_PT * load(ifstream & fp);
			virtual ~LCP_PT();
	};
};
#endif
