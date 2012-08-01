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

#include <LCP.h>

namespace cds_static
{

	uint * LCP::create_lcp(TextIndex *csa, char *text, uint n, int q) const
	{
		if(q > (int)n) {
			cout << "Specified q (" << q << ") greater than string length (" << n << ")" << endl;
			return NULL;
		}
		int i,h,j,k, m;
		int *sa = NULL;
		int *plcp = NULL;
		/*q==-1 its mean that for any position we just use psi for calculate each LCP*/
		if(q!=-1) {
			m = 1+(n-1)/q;
			plcp = new int[m];	 //space for sampled lcps
			if(plcp==NULL) {
				cout << "Failed to allocate memory for plcp." << endl;
				return NULL;
			}
			/*initialize samples to -1*/
			for(i=0; i < m; i++)
				plcp[i] = -1;
			sa = new int[n];
			/*calculate SA*/
			for(k=0; k < (int)n; k++)
				sa[k] = csa->getSA(k);
			/*compute the sparse phi array (in plcp)*/
			for(k=1; k < (int)n-1; k++) {
				if(sa[k] % q == 0)
					plcp[sa[k]/q] = sa[k-1];
			}
			/*most of sparse phi computed make a special case of the last one*/
			if(sa[n-1]%q == 0)
				plcp[sa[n-1]/q] = n;
			/*all of sparse phi computed*/

			/*compute lcp''*/
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

		/*compute lcp*/
		uint *lcp = new uint[n];
		lcp[0]=0;
		uint sa_k, sa_ant;
		int iq;
		sa_k = sa[0];
		for(k=1; k<(int)n;k++) {
			sa_ant = sa_k;
			sa_k = sa[k];
			if(sa_k % q == 0)
				lcp[k] = plcp[sa_k/q];
			else {
								 //smallest multiple of q less than i
				iq = (sa_k/q) * q;
				h = plcp[sa_k/q] - (sa_k-iq);
				if(h<0)
					h=0;
				/*the total cost of this loop should be O(qn)*/
				while(sa_k+h < n && sa_ant+h < n && text[sa_k+h]==text[sa_ant+h])
					h++;
				lcp[k] = h;
			}
		}
		delete [] plcp;
		delete [] sa;
		return lcp;
	}

	LCP * LCP::load(ifstream & fp) {
		size_t r = loadValue<size_t>(fp);
		size_t pos = fp.tellg();
		fp.seekg(pos-sizeof(size_t));
		cout << "Loading "<< r << endl;
		switch(r) {
			case NAIVE: return LCP_naive::load(fp);
			case SAD_GON_OS: return LCP_Sad::load(fp);
			case FMN_RRR_OS: return LCP_FMN::load(fp);
			case PT: return LCP_PT::load(fp);
			case PHI: return LCP_PhiSpare::load(fp);
			case DAC: return LCP_DAC::load(fp);
			case DAC_VAR: return LCP_DAC_VAR::load(fp);
		}
		return NULL;
	}
};
