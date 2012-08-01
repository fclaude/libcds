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

#include <LCP_PhiSpare.h>

namespace cds_static
{

	LCP_PhiSpare::LCP_PhiSpare() {
		lcp_type = PHI;
		q = n = 0;
		plcp = NULL;
	}

	LCP_PhiSpare::LCP_PhiSpare(TextIndex *csa, char *text, size_t _n, int _q) {
		if(_q > (int)_n) {
			cout << "Specified q (" << _q << ") greater than string length (" << _n << ")" << endl;
			exit(1);
		}
		lcp_type = PHI;

		int i,h,j,k, m;
		int *sa;
		q = _q;
		n = _n;

		/*q==-1 its mean that for any position we just use psi for calculate each LCP*/
		if(q!=-1) {
			m = 1+(_n-1)/_q;
			plcp = new int[m];	 //space for sampled lcps
			if(plcp==NULL) {
				cout << "Failed to allocate memory for plcp." << endl;
				exit(1);
			}
			// initialize samples to -1
			for(i=0; i < m; i++)
				plcp[i] = -1;
			sa = new int[n];
			//calculate SA
			for(k=0; k < (int)n; k++)
				sa[k] = csa->getSA(k);
			//compute the sparse phi array (in plcp)
			for(k=1; k < (int)n-1; k++) {
				if(sa[k] % q == 0) {
					plcp[sa[k]/q] = sa[k-1];
				}
			}
			//most of sparse phi computed

			//make a special case of the last one
			if(sa[n-1]%q == 0)
				plcp[sa[n-1]/q] = sa[n-2];
			//all of sparse phi computed

			delete [] sa;

			//compute lcp''
			h = 0;
			for(i=0; i < m; i++) {
				j = plcp[i];	 //here plcp[i] = phi'[i]
				k = i*q;
				while(k+h < (int)n && j+h < (int)n && text[k+h] == text[j+h])
					h++;
				plcp[i] = h;	 //now plcp[i] = h
				if(h > 0)
					h -= q;
				if(h < 0)
					h = 0;
			}
		}
	}

	size_t LCP_PhiSpare::get_LCP(size_t i, TextIndex *csa) const
	{
		if(i==0)
			return 0;
		int iq, h, y;
		int sa_i = csa->getSA(i);// SA[i]
		int p0, p1;
		p0 = i-1;
		p1 = i;

		if(q==-1)
			h = 0;
		else {
			//if we've already worked this one out
			if(sa_i % q == 0)
				return plcp[sa_i/q];
			iq = (sa_i/q)*q;	 //smallest multiple of q less than sa_i
			h = plcp[sa_i/q] - (sa_i-iq);
			if(h < 0)
				h = 0;
			if(h != 0) {
								 //SA[i-1]
				y = csa->getSA(i-1);
				if(y < sa_i)
					h=0;
				else {
					//this must be p1=SA_inverse[SA[i]+h] y p0=SA_inverse[SA[i-1]+h]
					p1 = csa->getISA(sa_i+h);
					p0 = csa->getISA(y+h);
				}
			}
		}
		//the total cost of this loop should be O(qn)
		//while(sa_i+h < n && j+h < n && text[i+h] == text[j+h])
		while(csa->getT(p0) == csa->getT(p1) && p0!=-1 && p1!=-1) {
			h++;
			p0 = csa->getPsi(p0);
			p1 = csa->getPsi(p1);
		}
		return h;
	}

	size_t LCP_PhiSpare::get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const
	{
		return get_LCP(i,csa);
	}

	size_t LCP_PhiSpare::getSize() const
	{
		size_t mem =0;
		if(q==-1)
			return 0;
		mem += sizeof(LCP_PhiSpare);
		mem += (1+(n-1)/q)*sizeof(int);
		return mem;
	}

	void LCP_PhiSpare::save(ofstream & fp) const
	{
		saveValue(fp, lcp_type);
		saveValue(fp, q);
		saveValue(fp, n);
		if(q!=-1)
			saveValue(fp, plcp, (1+(n-1)/q));
	}

	LCP_PhiSpare * LCP_PhiSpare::load(ifstream & fp) {
		LCP_PhiSpare *lcp = new LCP_PhiSpare();
		size_t type = loadValue<uint>(fp);
		if(type!=PHI) {
			abort();
		}
		lcp->q = loadValue<int>(fp);
		lcp->n = loadValue<size_t>(fp);
		if(lcp->q!=-1) {
			int size = 1+ (lcp->n-1/lcp->q);
			lcp->plcp = loadValue<int>(fp, size);
		}
		return lcp;
	}

	LCP_PhiSpare::~LCP_PhiSpare() {
		if(q!=-1)
			delete [] plcp;
	}

};
