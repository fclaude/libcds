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

#include <LCP_naive.h>

namespace cds_static
{

	LCP_naive::LCP_naive() {
		lcp_array = NULL;
		length = b = 0;
	}

	/*Compute LCP by brute force using nlog(n) bits */

	LCP_naive::LCP_naive(TextIndex *csa, char *text, size_t n) {
		length =n;
		size_t prev, act, val;
		b = bits(length-1);
		long long nb = 1;
		nb = (nb*b*n+W-1)/W;
		if(nb > MAXINT) {
			cout << "Memory limit excess (in LCP)" << endl;
			exit(1);
		}
		lcp_array = new uint[(size_t)nb];
		for(size_t i=0; i<(size_t)nb; i++)
			lcp_array[i]=0;
		set_field_64(lcp_array, b, 0, 0);
		val = csa->getSA(0);
		prev = n - val;
		for(size_t i=1; i < length; i++) {
			val = csa->getSA(i);
			act = n - val;
			set_field_64(lcp_array, b, i, lcp_length(text, n-prev, val, min(prev,act)));
			prev = act;
		}
	}

	size_t LCP_naive::get_LCP(size_t i, TextIndex *csa) const
	{
		return  get_field_64(lcp_array, b, i);
	}

	size_t LCP_naive::get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const
	{
		return get_LCP(i,csa);
	}

	size_t LCP_naive::getSize() const
	{
		size_t result = ((b*length+W-1)/W)*sizeof(size_t);
		return result + sizeof(LCP_naive);
	}

	void LCP_naive::save(ofstream & fp) const
	{
		size_t wr = NAIVE;
		saveValue(fp,wr);
		saveValue(fp,length);
		saveValue(fp,b);
		saveValue(fp, lcp_array, ((b*length+W-1)/W));
	}

	LCP_naive* LCP_naive::load(ifstream & fp) {
		LCP_naive *lcp = new LCP_naive();
		size_t type = loadValue<size_t>(fp);
		if(type!=NAIVE) {
			abort();
		}
		lcp->length = loadValue<size_t>(fp);
		lcp->b = loadValue<size_t>(fp);
		lcp->lcp_array = loadValue<uint>(fp, ((lcp->b*lcp->length+W-1)/W));
		return lcp;
	}

	LCP_naive::~LCP_naive() {
		if(lcp_array!=NULL) {
			delete [] lcp_array;
		}
	}
};
