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

#include "LCP_Sad.h"

namespace cds_static
{

	LCP_Sad::LCP_Sad() {
		lcp_type = SAD_GON_OS;
		U = NULL;
		U_length = U_type = 0;
	}

	LCP_Sad::LCP_Sad(TextIndex *csa, char *text, size_t n, size_t op_rs) {
		if(op_rs!=BRW32_HDR && op_rs!=DARRAY_HDR) {
			cout << "Error: op_rs must be BRW32_HDR or DARRAY_HDR\n" << endl;
			exit(1);
		}
		lcp_type = SAD_GON_OS;
		U_type = op_rs;
		uint *lcp = create_lcp(csa,text,n,32);
		uint pos =0;
		uint lcp_prev, lcp_act;
		long long nb = 1;
		nb = (nb*2*n+W-1)/W;
		if(nb > MAXINT) {
			cout << "Memory limit excess (in LCP)" << endl;
			exit(1);
		}
		uint *S = new uint[(uint)nb];
		for(uint i=0; i < (uint)nb; i++)
			S[i]=0;
		bitset(S,0);

		lcp_prev = lcp[csa->getISA(0)];
		pos += lcp_prev +1;
		//calculate S and create U
		for(uint i=1; i<n ;i++) {
			bitset(S,pos);
			lcp_act = lcp[csa->getISA(i)];
			pos+= lcp_act - lcp_prev + 2;
			lcp_prev = lcp_act;
		}
		bitset(S,pos);
		if(op_rs==BRW32_HDR)
								 //(bitarray, length, factor)
			U = new BitSequenceRG(S, pos+1, 4);
		else
			U = new BitSequenceDArray(S, pos+1);
		U_length = (size_t)(pos+1);
		delete [] S;
		delete [] lcp;
	}

	LCP_Sad::LCP_Sad(LCP *lcp, TextIndex *csa, size_t n, size_t op_rs) {
		if(op_rs!= BRW32_HDR && op_rs!= DARRAY_HDR) {
			cout << "Error: op_rs must be BRW32_HDR or DARRAY_HDR\n" << endl;
			exit(1);
		}
		lcp_type = SAD_GON_OS;
		U_type = op_rs;
		uint pos =0;
		uint lcp_prev, lcp_act;
		long long nb = 1;
		nb = (nb*2*n+W-1)/W;
		if(nb > MAXINT) {
			cout << "Memory limit excess (in LCP)" << endl;
			exit(1);
		}
		uint *S = new uint[(uint)nb];
		for(uint i=0; i < (uint)nb; i++)
			S[i]=0;
		bitset(S,0);
		lcp_prev = lcp->get_LCP(csa->getISA(0), csa);
		pos += lcp_prev +1;
		//calculate S and create U
		for(uint i=1; i<n ;i++) {
			bitset(S,pos);
			lcp_act = lcp->get_LCP(csa->getISA(i), csa);
			pos+= lcp_act - lcp_prev + 2;
			lcp_prev = lcp_act;
		}
		bitset(S,pos);
		if(op_rs==BRW32_HDR)
								 //(bitarray, length, factor)
			U = new BitSequenceRG(S, pos+1, 4);
		else
			U = new BitSequenceDArray(S, pos+1);
		U_length = (size_t)(pos+1);
		delete [] S;
	}

	size_t LCP_Sad::get_LCP(size_t i, TextIndex *csa) const
	{
		size_t val =   csa->getSA(i);
		if(val > 0)
			return U->select1(val+2)-2*val-1;
		else
			return U->select1(2)-1;
	}

	size_t LCP_Sad::get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const
	{
		return get_LCP(i,csa);
	}

	size_t LCP_Sad::getSize() const
	{
		return U->getSize()+sizeof(LCP_Sad);
	}

	void LCP_Sad::save(ofstream & fp) const
	{
		saveValue(fp, lcp_type);
		saveValue(fp,U_type);
		saveValue(fp,U_length);
		U->save(fp);
	}

	LCP_Sad* LCP_Sad::load(ifstream & fp) {
		LCP_Sad *lcp = new LCP_Sad();
		size_t type = loadValue<size_t>(fp);
		if(type!=SAD_GON_OS) {
			abort();
		}
		lcp->U_type = loadValue<size_t>(fp);
		lcp->U_length = loadValue<size_t>(fp);
		lcp->U = BitSequence::load(fp);
		return lcp;
	}

	LCP_Sad::~LCP_Sad() {
		if(U!=NULL) {
			if(U_type==BRW32_HDR)
				delete (BitSequenceRG *)U;
			else
				delete (BitSequenceDArray *)U;
		}
	}
};
