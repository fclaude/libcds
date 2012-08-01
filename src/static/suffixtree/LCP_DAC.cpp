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

#include <LCP_DAC.h>

namespace cds_static
{
	/*global variable for do sequential access*/

	/*LCP using Directly Addressable Variable-Length Codes */
	LCP_DAC::LCP_DAC() {
		lcp_type = DAC;
		rep = NULL;
		n = 0;
		next_p = NULL;
	}

	LCP_DAC::LCP_DAC(TextIndex *csa, char *text, size_t length) {
		lcp_type = DAC;
		n = length;
		uint *lcp = create_lcp(csa,text,n,32);
		rep = new factorization(lcp, n);
		next_p = new size_t[(uint)rep->nLevels -1];
		delete [] lcp;
	}

	size_t LCP_DAC::get_LCP(size_t i, TextIndex *csa) const
	{
		return (size_t)rep->access(i+1);
	}

	size_t LCP_DAC::get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const
	{
		if(*n_next==0) {
			*n_next = (size_t)rep->nLevels -1;
			for(size_t j=0; j< *n_next; j++)
				next_p[j] =0;
			*next_pos = next_p;
		}
		return (size_t)rep->access_seq(i+1, *next_pos, dir);
	}

	size_t LCP_DAC::getSize() const
	{
		size_t mem = 0;
		mem += sizeof(LCP_DAC);
		mem += sizeof(size_t)*((uint)rep->nLevels -1);
		mem += rep->getSize();
		return mem;
	}

	void LCP_DAC::save(ofstream & fp) const
	{
		saveValue(fp, lcp_type);
		saveValue(fp, n);
		rep->save(fp);
	}

	LCP_DAC* LCP_DAC::load(ifstream & fp) {
		LCP_DAC *lcp = new LCP_DAC();
		size_t type = loadValue<size_t>(fp);
		if(type!=DAC) {
			abort();
		}
		lcp->n = loadValue<size_t>(fp);
		lcp->rep = factorization::load(fp);
		lcp->next_p = new size_t[(uint)((lcp->rep)->nLevels) -1];
		return lcp;
	}

	LCP_DAC::~LCP_DAC() {
		if(rep!=NULL)
			delete (factorization *)rep;
		if(next_p!=NULL)
			delete [] next_p;
	}

};
