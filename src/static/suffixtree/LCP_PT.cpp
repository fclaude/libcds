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

#include <LCP_PT.h>

const int max_precomputed_cover = 8;
const int coversizes[max_precomputed_cover+1]= {1,2,3,4,5,7,9,13,20};
const unsigned cover0[] = {0};
const unsigned cover1[] = {0,1};
const unsigned cover2[] = {0,1,2};
const unsigned cover3[] = {0,1,2,4};
const unsigned cover4[] = {0,1,2,5,8};
const unsigned cover5[] = {		 //{0,7,8,10,14,19,23};
	0,1,2,3,7,11,19
};
const unsigned cover6[] = {0,1,2,5,14,16,34,42,59};
const unsigned cover7[] = {0,1,3,7,17,40,55,64,75,85,104,109,117};
const unsigned cover8[] = {
	0,1,3,7,12,20,30,44,65,80,89,96,114,122,
	128,150,196,197,201,219
};
const unsigned * _covers[] = {
	cover0, cover1, cover2, cover3, cover4,
	cover5, cover6, cover7, cover8
};

const int _cover_sizes[] = {1,2,3,4,5,7,9,13,20};

namespace cds_static
{

	LCP_PT::LCP_PT() {
		lcp_type = PT;
		logv = 0;
		length = 0;
		mask = v = m = cover_size = 0;
		rev_cover = NULL;
		delta = NULL;
		rev_ess = NULL;
		rmq = NULL;
	}

	LCP_PT::LCP_PT(TextIndex *csa, char *text, size_t n, int _logv) {
		if(_logv > max_precomputed_cover) {
			fprintf(stderr,"Specified DC (%d) greater than max (%d)\n",_logv,max_precomputed_cover);
			exit(1);
		}
		lcp_type = PT;
		logv = _logv;
		v = (1 << _logv);
		mask = v - 1;
		length = n;
		unsigned *_cover = (unsigned *)(_covers[logv]);
		cover_size = _cover_sizes[logv];
		int _m = length/v;
		_m = _m * cover_size;
		//compute rev_cover
		rev_cover = new int[v];
		int j = 0;
		for(int i = 0; i < v; i++) {
			rev_cover[i] = -1;
			if((int)_cover[j] == i) {
				rev_cover[i] = j;
				if(j < (int)(length & mask))
					_m++;
				j++;
			}
		}

		//compute delta
		delta = new int[v];
		for (int i = cover_size-1; i >= 0; i--) {
			for (j = 0; j < cover_size; j++)
				delta[(_cover[j]-_cover[i])%v] = _cover[i];
		}

		m = _m;
		//compute arrays _ess, rev_ess
		int *_ess = (int *)malloc(sizeof(int) * _m);
		rev_ess = new int[_m];
		for(int i=0; i<_m; i++)
			rev_ess[i]=0;
		j = 0;
		for(int i = 0; i < (int)length; i++) {
								 //_sa[i]
			int si = csa->getSA(i);
			if(rev_cover[si&mask] != -1) {
				//this is a sample suffix
				_ess[j] = si;
				rev_ess[(cover_size*(si>>logv)) + rev_cover[si&mask]] = j;
				j++;
			}
		}

		//compute _ell using _ess, _rev_ess, _rev_cover
		int *ell = new int[_m];
		for(int i=0; i<_m;i++)
			ell[i]=0;
		ell[0] = 0;
		int len = 0;
		int computed = 0;
		int *lengths = (int *)malloc(sizeof(int) * cover_size);
		for(int i = 0; i < cover_size; i++) {
			lengths[i] = 0;
		}
		int compares_saved = 0;
		int ihat = 0;
		for(int i = 0; i < _m; i++) {
			ihat = rev_ess[i];
			len = lengths[i%cover_size];
			if(len < 0) len = 0;
			compares_saved += len;
			if(ihat > 0) {
				int j = _ess[ihat-1];
				while(_ess[ihat]+len < (int)length && j+len < (int)length) {
					if(text[_ess[ihat]+len] != text[j+len]) {
						break;
					}
					len++;
				}
			}
			ell[ihat] = len;
			int a = rev_cover[(_ess[ihat])&mask];
			lengths[a] = len - v;
			computed++;
		}
		//we no longer need _ess and lengths.
		free(_ess);
		free(lengths);
		//preprocess _ell for RMQs
		rmq = new RMQ_succinct(ell,_m);
	}

	int LCP_PT::calc_delta(unsigned int i, unsigned int j) const
	{
		return ((delta[(j-i)%v]-i)%v);
	}

	size_t LCP_PT::get_LCP(size_t i, TextIndex *csa) const
	{
		if(i==0)
			return 0;
		int p0 = i-1;
		int p1 = i;
		int j=0;
		while( csa->getT(p0) == csa->getT(p1) && j < v && p0!=-1 && p1!=-1) {
			j++;
			p0 = csa->getPsi(p0);
			p1 = csa->getPsi(p1);
		}
		if(j<v)
			return (size_t)j;
		else {
								 // SA[i-1]
			int s0 = (int)csa->getSA(i-1);
								 //SA[i]
			int s1 = (int)csa->getSA(i);
			int ds0s1 = calc_delta((unsigned int)s0,(unsigned int)s1);
			int a0 = s0 + ds0s1;
			int a1 = s1 + ds0s1;
			int r0 = rev_ess[(cover_size*(a0>>logv)) + rev_cover[a0&mask]];
			int r1 = rev_ess[(cover_size*(a1>>logv)) + rev_cover[a1&mask]];
			int rmin_auto = rmq->a[(rmq->query(r0+1,r1))];
			return (size_t)(ds0s1 + rmin_auto);
		}
	}

	size_t LCP_PT::get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const
	{
		return get_LCP(i,csa);
	}

	size_t LCP_PT::getSize() const
	{
		size_t mem =0;
		mem += sizeof(LCP_PT);
		mem += rmq->getSize();
		//rev_cover
		mem += sizeof(int) * v;
		//delta
		mem += sizeof(int) * v;
		//rev_ess;
		mem += sizeof(int) * m;
		return mem;
	}

	void LCP_PT::save(ofstream & fp) const
	{
		saveValue(fp, lcp_type);
		saveValue(fp, logv);
		saveValue(fp, length);
		saveValue(fp, m);
		saveValue(fp, rev_cover, v);
		saveValue(fp, delta, v);
		saveValue(fp, rev_ess, m);
		rmq->save(fp);
	}

	LCP_PT * LCP_PT::load(ifstream & fp) {
		LCP_PT *lcp = new LCP_PT();
		size_t type = loadValue<size_t>(fp);
		if(type!=PT) {
			abort();
		}
		lcp->logv = loadValue<int>(fp);
		lcp->length = loadValue<size_t>(fp);
		lcp->m = loadValue<int>(fp);
		lcp->v = (1 << lcp->logv);
		lcp->mask = lcp->v - 1;
		lcp->cover_size = _cover_sizes[lcp->logv];
		lcp->rev_cover = loadValue<int>(fp, lcp->v);
		lcp->delta = loadValue<int>(fp, lcp->v);
		lcp->rev_ess = loadValue<int>(fp, lcp->m);
		lcp->rmq = lcp->rmq->load(fp);
		return lcp;
	}

	LCP_PT::~LCP_PT() {
		delete[] rev_cover;
		delete[] delta;
		delete[] rev_ess;
		delete (RMQ_succinct *)rmq;
	}
};
