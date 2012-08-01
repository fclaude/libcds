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

#include <PSV.h>

namespace cds_static
{
	PSV::PSV() {
		r=b=n=b_A=0;
		P=NULL;
		R=NULL;
		A=NULL;
	}

	void PSV::create_first_level(LCP *lcp, TextIndex *csa) {
		uint *P_aux;
		uint *R_aux;
		/*we move all the result 1 space to de rigth*/
		uint size = (n+W-1)/W;
		uint last_psv = n;
		uint j, lcp_value, psv_aux;
		P_aux = new uint[size];
		R_aux = new uint[size];

		for(uint i=0; i<size; i++) {
			P_aux[i]=0;
			R_aux[i]=0;
		}
		/*for each i find his PSV position*/
		for(uint i=n; i>0; i--) {
			lcp_value = lcp->get_LCP(i-1,csa);
			psv_aux = 0;
			for(j=i-1; j>0; j--) {
				psv_aux =  lcp->get_LCP(j-1,csa);
				if(lcp_value > psv_aux) {
					/*if is not near*/
					if( i/b !=  j/b ) {
						if( j/b != last_psv/b) {
							/*mark the pioneer in P*/
							bitset(P_aux,i);
							/*mark the pioneer and the PSV of the pioneer in R*/
							bitset(R_aux,i);
							bitset(R_aux,j);
						}
						last_psv=j;
					}
					break;
				}
			}					 /*close for j*/
			if(j==0) {
				if( i/b != j/b ) {
					if( j/b !=  last_psv/b) {
						/*mark the pioneer in P*/
						bitset(P_aux,i);
						/*mark the pioneer and the NSV of the pioneer in R*/
						bitset(R_aux,i);
						bitset(R_aux,j);
					}
					last_psv=j;
				}
			}
		}						 /*close for i*/
		P[0] = new BitSequenceRRR(P_aux, n+1);
		R[0] = new BitSequenceRRR(R_aux, n+1);
		delete[] P_aux;
		delete[] R_aux;
	}

	void PSV::create_level_r(LCP *lcp, size_t level, TextIndex *csa) {
		uint *P_aux, *R_aux;
		uint size = (n+W-1)/W;
		uint last_psv=n, ini=0;
		uint far_aux, lcp_value, psv_aux;
		int j;
		P_aux = new uint[size];
		R_aux = new uint[size];
		for(uint i=0; i < size; i++) {
			P_aux[i]=0;
			R_aux[i]=0;
		}
		uint num_elements =  R[level-1]->rank1(n);
		uint *lcp_r = new uint[num_elements];
		if(R[level-1]->access(0))
			ini = 1;

		for(uint i=ini; i<num_elements; i++) {
			lcp_r[i] = lcp->get_LCP(R[level-1]->select1(i+1)-1, csa);
		}

		/*for each i find his PSV position*/
		for(uint i= num_elements-1; i>0; i--) {
			lcp_value = lcp_r[i];
			for(j=i-1; j>0; j--) {
				psv_aux =  lcp_r[j];
				if(lcp_value > psv_aux) {
					/*if is not near*/
					if( i/b !=  j/b ) {
						if( j/b != last_psv/b) {
							/*mark the pioneer in P*/
							far_aux = R[level-1]->select1(i+1);
							bitset(P_aux,far_aux);
							/*mark the pioneer and the PSV of the pioneer in R*/
							bitset(R_aux, far_aux);
							bitset(R_aux, R[level-1]->select1(j+1));
						}
						last_psv=j;
					}
					break;
				}
			}					 /*close for j*/

			if(j==0) {
				if(ini==1) {
					/*if is not near*/
					if( i/b !=  j/b ) {
						if( j/b != last_psv/b) {
							/*mark the pioneer in P*/
							far_aux = R[level-1]->select1(i+1);
							bitset(P_aux,far_aux);
							/*mark the pioneer and the PSV of the pioneer in R*/
							bitset(R_aux, far_aux);
							bitset(R_aux, 0);
						}
						last_psv=j;
					}
				}
				else {
					psv_aux =  lcp_r[j];
					if(lcp_value > psv_aux) {
						/*if is not near*/
						if( i/b !=  j/b ) {
							if( j/b != last_psv/b) {
								/*mark the pioneer in P*/
								far_aux = R[level-1]->select1(i+1);
								bitset(P_aux,far_aux);
								/*mark the pioneer and the PSV of the pioneer in R*/
								bitset(R_aux, far_aux);
								bitset(R_aux, R[level-1]->select1(j+1));
							}
							last_psv=j;
						}
					}
				}
			}
		}						 /*close for i*/

		delete [] lcp_r;
		P[level] = new BitSequenceRRR(P_aux, n);
		R[level] = new BitSequenceRRR(R_aux, n);
		delete[] P_aux;
		delete[] R_aux;
	}

	void PSV::create_last_level(LCP *lcp, TextIndex *csa) {
		size_t *lcp_r;
		uint lcp_value, psv_aux;
		int j, ini=0;

		/*store the results for the last level*/
		uint num_elements =  R[r-1]->rank1(n);
		b_A = bits(num_elements);
		A = new uint[(num_elements*b_A + W -1)/W];
		for(size_t i=0; i<((num_elements*b_A + W -1)/W); i++)
			A[i]=0;
		lcp_r = new size_t[num_elements];

		if(R[r-1]->access(0))
			ini = 1;

		for(uint i=ini; i<num_elements; i++) {
			lcp_r[i] = lcp->get_LCP(R[r-1]->select1(i+1)-1, csa);
		}

		for(int i=num_elements-1; i>0; i--) {
			lcp_value = lcp_r[i];
			for(j=i-1; j>0; j--) {
				psv_aux = lcp_r[j];
				if(lcp_value > psv_aux) {
					set_field_64(A, b_A, i, j);
					break;
				}
			}					 /*close for j*/
			if(j==0) {
				if(ini)
					set_field_64(A, b_A, i, 0);
				else {
					psv_aux = lcp_r[j];
					if(lcp_value > psv_aux)
						set_field_64(A, b_A, i, j);
					else
						set_field_64(A, b_A, i, 0);
				}
			}
		}						 /*close for i*/
		set_field_64(A, b_A, 0, 0);
		delete [] lcp_r;
	}

	PSV::PSV(LCP *lcp, size_t levels, size_t block, TextIndex *csa) {
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

	size_t PSV::find_PSV_r(size_t v, size_t level, TextIndex *csa, LCP *lcp) const
	{
		if(level == r) {
			return get_field_64(A, b_A, v);
		}
		/*look in the same block*/
		size_t pos_v = R[level-1]->select1(v+1);
		size_t value_v = lcp->get_LCP(pos_v-1, csa);
		size_t aux_psv;
		size_t pioneer, psv_pioneer;
		size_t ini_block = b*(v/b);
		size_t ini_search;

		if(ini_block==0)
			ini_block = 1;

		for(size_t i=v; i >= ini_block; i--) {
			aux_psv = lcp->get_LCP( R[level-1]->select1(i+1)-1,csa);
			if(aux_psv < value_v)
				return i;
		}

		if(ini_block==1)
			return 0;

		/*find the pioneer*/
		if(P[level]->access(pos_v))
			pioneer = pos_v;
		else
			pioneer = P[level]->select1(P[level]->rank1(pos_v)+1);

		pioneer = R[level]->rank1(pioneer)-1;
		/*resolve the pioneer recursive*/
		psv_pioneer = find_PSV_r(pioneer, level+1, csa, lcp);
		psv_pioneer = R[level-1]->rank1(R[level]->select1(psv_pioneer+1))-1;
		if(psv_pioneer==0)
			psv_pioneer = 1;

		ini_search = b*((psv_pioneer+b)/b)-1;

		/*seek the answer in the same block of the nsv(las_pioneer)*/
		for(size_t i= ini_search; i >= psv_pioneer; i--) {
			aux_psv = lcp->get_LCP(R[level-1]->select1(i+1)-1,csa);
			if(aux_psv < value_v)
				return i;
		}
		return 0;
	}

	size_t PSV::find_PSV(size_t v, TextIndex *csa, LCP *lcp) const
	{
		/*look in the same block*/
		size_t n_next = 0;
		size_t *next;
		size_t value_v = lcp->get_seq_LCP(v, csa, &next, &n_next, 0);
		size_t aux_psv;
		size_t pioneer, psv_pioneer;
		size_t ini_block = b*((v+1)/b);
		size_t ini_search;
		if(ini_block==0)
			ini_block = 1;

		for(size_t i=v; i >= ini_block; i--) {
			aux_psv = lcp->get_seq_LCP(i-1,csa, &next, &n_next, 0);
			if(aux_psv < value_v)
				return i;
		}

		if(ini_block==1)
			return 0;

		/*find the pioneer*/
		if(P[0]->access(v+1))
			pioneer = v+1;
		else
			pioneer = P[0]->select1(P[0]->rank1(v+1)+1);
		/*imaginary position in R[0]*/
		pioneer = R[0]->rank1(pioneer)-1;
		/*resolve the pioneer recursive*/
		psv_pioneer = find_PSV_r(pioneer, 1, csa, lcp);
		psv_pioneer = R[0]->select1(psv_pioneer+1);
		if(psv_pioneer==0)
			psv_pioneer = 1;

		/*seek the answer in the same block of the psv(las_pioneer)*/
		ini_search = b*((psv_pioneer+b)/b) -1;
		n_next = 0;
		for(size_t i=ini_search; i>=psv_pioneer; i--) {
			aux_psv = lcp->get_seq_LCP(i-1,csa, &next, &n_next, 0);
			if(aux_psv < value_v)
				return i;
		}
		return 0;
	}

	void PSV::save(ofstream & fp) const
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

	PSV* PSV::load(ifstream & fp) {
		PSV *psv = new PSV();
		psv->r = loadValue<size_t>(fp);
		psv->b = loadValue<size_t>(fp);
		psv->n = loadValue<size_t>(fp);
		psv->b_A = loadValue<size_t>(fp);
		psv->P = new BitSequence*[psv->r];
		psv->R = new BitSequence*[psv->r];
		for(size_t i=0; i<psv->r; i++) {
			psv->P[i] = BitSequence::load(fp);
			psv->R[i] = BitSequence::load(fp);
		}
		size_t tam_A = psv->R[psv->r-1]->rank1(psv->n-1);
		tam_A = (tam_A*psv->b_A + W -1)/W;
		psv->A = loadValue<uint>(fp, tam_A);
		return psv;
	}

	size_t PSV::getSize() const
	{
		size_t size = 0;
		size +=  sizeof(PSV);
		for(size_t i=0; i<r; i++) {
			size += R[i]->getSize() + P[i]->getSize();
		}
		size +=  ((b_A*R[r-1]->rank1(n-1) + W -1)/W)*sizeof(uint);
		return size;
	}

	PSV::~PSV() {
		delete [] A;
		for(uint i=0; i<r ; i++) {
			delete (BitSequenceRRR *)P[i];
			delete (BitSequenceRRR *)R[i];
		}
		delete[] P;
		delete[] R;
	}

};
