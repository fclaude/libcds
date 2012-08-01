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

#include <NSV.h>

namespace cds_static
{
	NSV::NSV() {
		r=b=n=b_A=0;
		P=NULL;
		R=NULL;
		A=NULL;
	}

	void NSV::create_first_level(LCP *lcp, TextIndex *csa) {
		uint *P_aux;
		uint *R_aux;
		uint size = (n+W-1)/W;
		uint last_nsv = 0;
		uint j;
		uint lcp_value, nsv_aux;
		P_aux = new uint[size];
		R_aux = new uint[size];

		for(uint i=0; i<size; i++) {
			P_aux[i]=0;
			R_aux[i]=0;
		}
		/*for each i find his NSV position*/
		for(uint i=0; i<n; i++) {
			lcp_value = lcp->get_LCP(i, csa);
			nsv_aux = 0;
			for(j=i+1; j<n; j++) {
				nsv_aux =  lcp->get_LCP(j, csa);
				if(lcp_value > nsv_aux) {
					/*if is not near*/
					if( i/b < j/b ) {
						if( j/b !=  last_nsv/b) {
							/*mark the pioneer in P*/
							bitset(P_aux,i);
							/*mark the pioneer and the NSV of the pioneer in R*/
							bitset(R_aux,i);
							bitset(R_aux,j);
						}
						last_nsv=j;
					}
					break;
				}
			}					 /*close for j*/
			if(j==n) {
				if(i/b < j/b) {
					if(j/b != last_nsv/b) {
						/*mark the pioneer in P*/
						bitset(P_aux,i);
						/*mark the pioneer and the NSV of the pioneer in R*/
						bitset(R_aux,i);
						bitset(R_aux,j);
					}
					last_nsv=j;
				}
			}
		}						 /*close for i*/
		P[0] = new BitSequenceRRR(P_aux, n+1);
		R[0] = new BitSequenceRRR(R_aux, n+1);
		delete [] P_aux;
		delete [] R_aux;
	}

	void NSV::create_level_r(LCP *lcp, size_t level, TextIndex *csa) {
		uint *P_aux;
		uint *R_aux;
		uint size = (n+W-1)/W;
		uint last_nsv = 0;
		uint j, far_aux, lcp_value, nsv_aux;
		P_aux = new uint[size];
		R_aux = new uint[size];

		for(uint i=0; i < size; i++) {
			P_aux[i]=0;
			R_aux[i]=0;
		}

		uint num_elements =  R[level-1]->rank1(n-1);
		uint *lcp_r = new uint[num_elements];
		for(uint i=0; i<num_elements; i++) {
			lcp_r[i] = lcp->get_LCP(R[level-1]->select1(i+1), csa);
		}

		/*for each i find his NSV position*/
		for(uint i=0; i<num_elements; i++) {
			lcp_value = lcp_r[i];
			for(j=i+1; j<num_elements; j++) {
				nsv_aux =  lcp_r[j];
				if(lcp_value > nsv_aux) {
					/*if is not near*/
					if( i/b < j/b ) {
						if( j/b != last_nsv/b) {
							/*mark the pioneer in P*/
							far_aux = R[level-1]->select1(i+1);
							bitset(P_aux,far_aux);
							/*mark the pioneer and the NSV of the pioneer in R*/
							bitset(R_aux, far_aux);
							bitset(R_aux, R[level-1]->select1(j+1));
						}
						last_nsv=j;
					}
					break;
				}
			}					 /*close for j*/

			if(j==num_elements) {
				if( i/b < j/b ) {
					if( j/b !=  last_nsv/b) {
						/*mark the pioneer in P*/
						far_aux = R[level-1]->select1(i+1);
						bitset(P_aux,far_aux);
						/*mark the pioneer and the NSV of the pioneer in R*/
						bitset(R_aux,far_aux);
						bitset(R_aux, n);
					}
					last_nsv=j;
				}
			}
		}						 /*close for i*/
		delete [] lcp_r;
		P[level] = new BitSequenceRRR(P_aux, n);
		R[level] = new BitSequenceRRR(R_aux, n);
		delete[] P_aux;
		delete[] R_aux;
	}

	void NSV::create_last_level(LCP *lcp, TextIndex *csa) {
		uint *lcp_r;
		uint lcp_value, nsv_aux, j;

		/*store the results for the last level*/
		uint num_elements =  R[r-1]->rank1(n-1);

		b_A = bits(num_elements);
		A = new uint[(num_elements*b_A + W -1)/W];
		for(size_t i=0; i<((num_elements*b_A + W -1)/W); i++)
			A[i]=0;

		lcp_r = new uint[num_elements];
		for(uint i=0; i<num_elements; i++) {
			lcp_r[i] = lcp->get_LCP(R[r-1]->select1(i+1), csa);
		}

		for(uint i=0; i<num_elements; i++) {
			lcp_value = lcp_r[i];
			for(j=i+1; j<num_elements; j++) {
				nsv_aux = lcp_r[j];
				if(lcp_value > nsv_aux) {
					set_field_64(A, b_A, i, j);
					break;
				}
			}					 /*close for j*/
			if(j==num_elements)
				set_field_64(A, b_A, i, j);
		}						 /*close for i*/
		delete [] lcp_r;
	}

	size_t NSV::find_NSV_r(size_t v, size_t level, TextIndex *csa, LCP *lcp) const
	{
		if(level == r) {
			return get_field_64(A, b_A, v);
		}

		/*look in the same block*/
		size_t num_elements =  R[level-1]->rank1(n-1);
		size_t pos_v = R[level-1]->select1(v+1);
		size_t value_v = lcp->get_LCP(pos_v, csa);
		size_t aux_nsv;
		size_t pioneer, nsv_pioneer;
		size_t end_block = b*((v+b)/b);

		if(end_block > num_elements)
			end_block = num_elements;

		for(size_t i=v+1; i < end_block; i++) {
			aux_nsv = lcp->get_LCP(R[level-1]->select1(i+1),csa);
			if(aux_nsv < value_v)
				return i;
		}

		if(end_block==num_elements) {
			return num_elements;
		}

		/*find the pioneer*/
		pioneer = P[level]->select1(P[level]->rank1(pos_v));
		pioneer = R[level]->rank1(pioneer)-1;
		/*resolve the pioneer recursive*/
		nsv_pioneer = find_NSV_r(pioneer, level+1, csa, lcp);
		nsv_pioneer = R[level]->select1(nsv_pioneer+1);
		if(nsv_pioneer==(size_t)-1)
			nsv_pioneer = n;
		nsv_pioneer = R[level-1]->rank1(nsv_pioneer)-1;
		/*seek the answer in the same block of the nsv(las_pioneer)*/
		for(size_t i=(nsv_pioneer/b)*b  ; i<=nsv_pioneer; i++) {
			aux_nsv = lcp->get_LCP(R[level-1]->select1(i+1),csa);
			if(aux_nsv < value_v)
				return i;
		}
		return num_elements;
	}

	NSV::NSV(LCP *lcp, size_t levels, size_t block, TextIndex *csa) {
		/*levels>=1*/
		n = csa->index_length();
		r = levels;
		b = block;
		P = new BitSequence*[r];
		R = new BitSequence*[r];
		create_first_level(lcp, csa);
		/*create the rest of the levels */
		for(size_t i=1; i < r; i++) {
			create_level_r(lcp, i, csa);
		}
		/*create the last level*/
		create_last_level(lcp, csa);
	}

	size_t NSV::find_NSV(size_t v, TextIndex *csa, LCP *lcp) const
	{
		/*look in the same block*/
		size_t n_next = 0;
		size_t *next;
		size_t value_v = lcp->get_seq_LCP(v, csa, &next, &n_next, 1);
		size_t aux_nsv;
		size_t pioneer, nsv_pioneer;
		size_t end_block = b*((v+b)/b);
		if(end_block>n)
			end_block = n;

		for(size_t i=v+1; i < end_block; i++) {
			aux_nsv = lcp->get_seq_LCP(i,csa, &next, &n_next, 1);
			if(aux_nsv < value_v)
				return i;
		}
		if(end_block==n)
			return n;
		/*find the pioneer*/
		pioneer = P[0]->select1(P[0]->rank1(v));
		pioneer = R[0]->rank1(pioneer)-1;
		/*resolve the pioneer recursive*/
		nsv_pioneer = find_NSV_r(pioneer, 1, csa, lcp);
		nsv_pioneer = R[0]->select1(nsv_pioneer+1);
		if(nsv_pioneer==(size_t)-1)
			nsv_pioneer = n;
		/*seek the answer in the same block of the nsv(las_pioneer)*/
		n_next = 0;
		for(uint i=(nsv_pioneer/b)*b  ; i<=nsv_pioneer; i++) {
			aux_nsv = lcp->get_seq_LCP(i,csa, &next, &n_next, 1);
			if(aux_nsv < value_v)
				return i;
		}
		return n;
	}

	void NSV::save(ofstream & fp) const
	{
		saveValue(fp, r);
		saveValue(fp, b);
		saveValue(fp, n);
		saveValue(fp, b_A);

		for(size_t i=0;  i<r;i++) {
			P[i]->save(fp);
			R[i]->save(fp);
		}
		size_t num_elements =  R[r-1]->rank1(n-1);
		saveValue(fp, A, (num_elements*b_A + W -1)/W);
	}

	NSV* NSV::load(ifstream & fp) {
		NSV *nsv = new NSV();
		nsv->r = loadValue<size_t>(fp);
		nsv->b = loadValue<size_t>(fp);
		nsv->n = loadValue<size_t>(fp);
		nsv->b_A = loadValue<size_t>(fp);
		nsv->P = new BitSequence*[nsv->r];
		nsv->R = new BitSequence*[nsv->r];
		for(size_t i=0; i<nsv->r; i++) {
			nsv->P[i] = BitSequence::load(fp);
			nsv->R[i] = BitSequence::load(fp);
		}
		size_t tam_A = nsv->R[nsv->r-1]->rank1(nsv->n-1);
		tam_A = (tam_A*nsv->b_A + W -1)/W;
		nsv->A = loadValue<uint>(fp, tam_A);
		return nsv;
	}

	size_t NSV::getSize() const
	{
		/*sum of all the levels*/
		size_t size =0;
		size +=  sizeof(NSV);
		for(size_t i=0; i<r; i++) {
			size += R[i]->getSize() + P[i]->getSize();
		}
		size +=  ((b_A*R[r-1]->rank1(n-1) + W -1)/W)*sizeof(uint);
		return size;
	}

	NSV::~NSV() {
		delete [] A;
		for(uint i=0; i<r ; i++) {
			delete (BitSequenceRRR *)P[i];
			delete (BitSequenceRRR *)R[i];
		}
		delete[] P;
		delete[] R;
	}

};
