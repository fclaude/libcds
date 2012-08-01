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

#include <LCP_FMN.h>

namespace cds_static
{

	LCP_FMN::LCP_FMN() {
		lcp_type = FMN_RRR_OS;
		O = NULL;
		Z = NULL;
		b_length = OZ_type = 0;
	}

	void LCP_FMN::generate_OZ(BitSequence *U, uint **O, uint **Z, uint length) {
		uint *o, *z;
		bool bit_1 = true;		 //tell if we are counting 1 bits or 0 bits
		uint cont_1 = 0;
		uint cont_0 = 0;
		long long nb = 1;
		nb = (nb*length+W-1)/W;
		if(nb > MAXINT) {
			cout << "Memory limit excess (in LCP)" << endl;
			exit(1);
		}
		o = new uint[(uint)nb];
		z = new uint[(uint)nb];

		for(uint i=0; i< (uint)nb; i++) {
			o[i]=0;
			z[i]=0;
		}

		for(uint i=0; i<length; i++) {
			if( U->access(i)) {
				if(bit_1)
					cont_1++;
				else {
					bit_1 = true;
					cont_1++;
					bitset(z,cont_0-1);
				}
			}
			else {
				if(bit_1) {
					bit_1 = false;
					cont_0++;
					bitset(o,cont_1-1);
				}
				else
					cont_0++;
			}
		}
		if(bit_1)
			bitset(o,cont_1-1);
		else
			bitset(z,cont_0-1);
		*O = o;
		*Z = z;
	}

	LCP_FMN::LCP_FMN(TextIndex *csa, char *text, size_t n, size_t op_rs) {
		if(op_rs!=RRR02_HDR && op_rs!=SDARRAY_HDR) {
			cout << "Error: op_rs must be RRR02_HDR or SDARRAY_HDR\n" << endl;
			exit(1);
		}
		lcp_type = FMN_RRR_OS;
		OZ_type = op_rs;
		LCP_Sad *sa_lcp = new LCP_Sad(csa, text, n);
		uint *O_aux, *Z_aux;
		b_length = sa_lcp->U_length;
		generate_OZ(sa_lcp->U, &O_aux, &Z_aux, b_length);
		if(op_rs==RRR02_HDR) {
			O = new BitSequenceRRR(O_aux, b_length);
			Z = new BitSequenceRRR(Z_aux, b_length);
		}
		else {
			O = new BitSequenceSDArray(O_aux, b_length);
			Z = new BitSequenceSDArray(Z_aux, b_length);
		}
		delete (LCP_Sad *)sa_lcp;
		delete [] O_aux;
		delete [] Z_aux;
	}

	LCP_FMN::LCP_FMN(LCP *lcp, TextIndex *csa, size_t n, size_t op_rs) {
		uint *O_aux, *Z_aux;
		if(op_rs!=RRR02_HDR && op_rs!=SDARRAY_HDR) {
			cout << "Error: op_rs must be RRR02_HDR or SDARRAY_HDR\n" << endl;
			exit(1);
		}
		lcp_type = FMN_RRR_OS;
		OZ_type = op_rs;
		LCP_Sad *sa_lcp = new LCP_Sad(lcp, csa, n);
		b_length = sa_lcp->U_length;
		generate_OZ(sa_lcp->U, &O_aux, &Z_aux, b_length);
		if(op_rs==RRR02_HDR) {
			O = new BitSequenceRRR(O_aux, b_length);
			Z = new BitSequenceRRR(Z_aux, b_length);
		}
		else {
			O = new BitSequenceSDArray(O_aux, b_length);
			Z = new BitSequenceSDArray(Z_aux, b_length);
		}
		delete (LCP_Sad *)sa_lcp;
		delete [] O_aux;
		delete [] Z_aux;
	}

	size_t LCP_FMN::get_LCP(size_t i, TextIndex *csa) const
	{
		size_t val = csa->getSA(i);
		size_t rank_0 = O->rank1(val);
		if(rank_0 > 0) {
			size_t r =   Z->select1(rank_0) - val +1;
			return r;
		}
		else
			return 0;
	}

	size_t LCP_FMN::get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const
	{
		return get_LCP(i,csa);
	}

	size_t LCP_FMN::getSize() const
	{
		return O->getSize() + Z->getSize() + sizeof(LCP_FMN);
	}

	void LCP_FMN::save(ofstream & fp) const
	{
		saveValue(fp,lcp_type);
		saveValue(fp,b_length);
		saveValue(fp,OZ_type);
		O->save(fp);
		Z->save(fp);
	}

	LCP_FMN* LCP_FMN::load(ifstream & fp) {
		LCP_FMN *lcp = new LCP_FMN();
		size_t type = loadValue<size_t>(fp);
		if(type!=FMN_RRR_OS) {
			abort();
		}
		lcp->b_length = loadValue<size_t>(fp);
		lcp->OZ_type = loadValue<size_t>(fp);
		lcp->O = BitSequence::load(fp);
		lcp->Z = BitSequence::load(fp);
		return lcp;
	}

	LCP_FMN::~LCP_FMN() {
		if(O!= NULL) {
			if(OZ_type == RRR02_HDR)
				delete (BitSequenceRRR *)O;
			else
				delete (BitSequenceSDArray *)O;
		}
		if(Z!=NULL) {
			if(OZ_type == RRR02_HDR)
				delete (BitSequenceRRR *)Z;
			else
				delete (BitSequenceSDArray *)Z;
		}
	}

};
