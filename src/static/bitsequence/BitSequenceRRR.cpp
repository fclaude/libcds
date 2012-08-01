/* BitSequenceRRR.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * BitSequenceRRR definition
 *
 * This library is free software; you can redistribute it and/or
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

#include <BitSequenceRRR.h>

namespace cds_static
{

	table_offset * BitSequenceRRR::E = NULL;

	BitSequenceRRR::BitSequenceRRR() {
		ones=0;
		length=0;
		if(E==NULL) E = new table_offset(BLOCK_SIZE);
		E->use();
		C = NULL;
		O = NULL;
		C_sampling = NULL;
		O_pos = NULL;
		sample_rate = DEFAULT_SAMPLING;
		C_len = O_len = C_sampling_len = O_pos_len = 0;
		O_bits_len = C_sampling_field_bits = O_pos_field_bits = 0;
	}

	BitSequenceRRR::BitSequenceRRR(uint * bitseq, size_t len, uint sample_rate) {
		build(bitseq,len,sample_rate);
	}

	BitSequenceRRR::BitSequenceRRR(const BitString & bs, uint sample_rate) {
		build(bs.getData(),bs.getLength(),sample_rate);
	}

	void BitSequenceRRR::build(const uint * bitseq, size_t len, uint sample_rate) {
		ones = 0;
		this->length = len;
		if(E==NULL) E = new table_offset(BLOCK_SIZE);
		E->use();

		// Table C
		C_len = len/BLOCK_SIZE + (len%BLOCK_SIZE!=0);
		C_field_bits = bits(BLOCK_SIZE);
		C = new uint[uint_len(C_len,C_field_bits)];
		for(uint i=0;i<uint_len(C_len,C_field_bits);i++)
			C[i] = 0;
		O_bits_len = 0;
		for(uint i=0;i<C_len;i++) {
			uint value = popcount(get_var_field(bitseq,i*BLOCK_SIZE,min((uint)len-1,(i+1)*BLOCK_SIZE-1)));
			assert(value<=BLOCK_SIZE);
			set_field(C,C_field_bits,i,value);
			ones += value;
			O_bits_len += E->get_log2binomial(BLOCK_SIZE,value);
		}

		// Table O
		O_len = uint_len(1,O_bits_len);
		O = new uint[O_len];
		for(uint i=0;i<O_len;i++)
			O[i] = 0;
		uint O_pos = 0;
		for(uint i=0;i<C_len;i++) {
			uint value = (ushort)get_var_field(bitseq,i*BLOCK_SIZE,min((uint)len-1,(i+1)*BLOCK_SIZE-1));
			set_var_field(O,O_pos,O_pos+E->get_log2binomial(BLOCK_SIZE,popcount(value))-1,E->compute_offset((ushort)value));
			O_pos += E->get_log2binomial(BLOCK_SIZE,popcount(value));
		}
		C_sampling = NULL;
		this->O_pos = NULL;

		create_sampling(sample_rate);
	}

	void BitSequenceRRR::create_sampling(uint sample_rate) {
		this->sample_rate = sample_rate;
		/*cout << "===================" << endl;
		cout << "length = " << length << endl;
		cout << "ones = " << ones << endl;
		cout << "C_len = " << C_len << endl;
		cout << "C_field_bits = " << C_field_bits << endl;
		cout << "O_len = " << O_len << endl;
		cout << "O_bits_len = " << O_bits_len << endl;
		cout << "sample_rate = " << sample_rate << endl;
		cout << "C_alen = " << uint_len(C_len,C_field_bits) << endl;
		cout << "O_alen = " << O_len << endl;*/

		// Sampling for C
		C_sampling_len = C_len/sample_rate+2;
		C_sampling_field_bits = bits(ones);
		if(C_sampling!=NULL) delete [] C_sampling;
		C_sampling = new uint[max((uint)1,uint_len(C_sampling_len,C_sampling_field_bits))];
		for(uint i=0;i<max((uint)1,uint_len(C_sampling_len,C_sampling_field_bits));i++)
			C_sampling[i] = 0;
		uint sum = 0;
		for(uint i=0;i<C_len;i++) {
			if(i%sample_rate==0)
				set_field(C_sampling,C_sampling_field_bits,i/sample_rate,sum);
			sum += get_field(C,C_field_bits,i);
		}
		for(uint i=(C_len-1)/sample_rate+1;i<C_sampling_len;i++)
			set_field(C_sampling,C_sampling_field_bits,i,sum);

		// Sampling for O (table S) (Code separated from previous construction for readability)
		O_pos_len = C_len/sample_rate+1;
		O_pos_field_bits = bits(O_bits_len);
		if(O_pos!=NULL) delete [] O_pos;
		O_pos = new uint[uint_len(O_pos_len,O_pos_field_bits)];
		for(uint i=0;i<uint_len(O_pos_len,O_pos_field_bits);i++)
			O_pos[i] = 0;
		uint pos = 0;
		for(uint i=0;i<C_len;i++) {
			if(i%sample_rate==0)
				set_field(O_pos,O_pos_field_bits,i/sample_rate,pos);
			pos += E->get_log2binomial(BLOCK_SIZE,get_field(C,C_field_bits,i));
		}
	}

	bool BitSequenceRRR::access(const size_t i) const
	{
		size_t nearest_sampled_value = i/BLOCK_SIZE/sample_rate;
		size_t pos_O = get_field(O_pos,O_pos_field_bits,nearest_sampled_value);
		size_t pos = i/BLOCK_SIZE;
		assert(pos<=C_len);
		for(size_t k=nearest_sampled_value*sample_rate;k<pos;k++) {
			size_t aux = get_field(C,C_field_bits,k);
			pos_O += E->get_log2binomial(BLOCK_SIZE,aux);
		}
		size_t c = get_field(C,C_field_bits,pos);
		return ((1<<(i%BLOCK_SIZE))&E->short_bitmap(c,get_var_field(O,pos_O,pos_O+E->get_log2binomial(BLOCK_SIZE,c)-1)))!=0;
	}

	bool BitSequenceRRR::access(const size_t i, size_t &r) const
	{
		if(i+1==0) return 0;
								 // patch for 32-64 bits bad coding, to be removed in the future
		if((uint)(i+1)==0) return 0;
		uint nearest_sampled_value = i/BLOCK_SIZE/sample_rate;
		uint sum = get_field(C_sampling,C_sampling_field_bits,nearest_sampled_value);
		uint pos_O = get_field(O_pos,O_pos_field_bits,nearest_sampled_value);
		uint pos = i/BLOCK_SIZE;
		uint k=nearest_sampled_value*sample_rate;
		if(k%2==1 && k<pos) {
			uint aux = get_field(C,C_field_bits,k);
			sum += aux;
			pos_O += E->get_log2binomial(BLOCK_SIZE,aux);
			k++;
		}
		unsigned char * a = (unsigned char *)C;
		size_t mask = 0x0F;
		a += k/2;
		while(k<(uint)max(0,(int)pos-1)) {
			assert(((*a)&mask)==get_field(C,C_field_bits,k));
			assert((*a)/16==get_field(C,C_field_bits,k+1));
			sum += ((*a)&mask)+(*a)/16;
			pos_O += E->get_log2binomial(BLOCK_SIZE,((*a)&mask))+E->get_log2binomial(BLOCK_SIZE,((*a)/16));
			a++;
			k+=2;
		}
		if(k<pos) {
			size_t aux = get_field(C,C_field_bits,k);
			sum += aux;
			pos_O += E->get_log2binomial(BLOCK_SIZE,aux);
			k++;
		}
		size_t c = get_field(C,C_field_bits,pos);
		short v = E->short_bitmap(c,get_var_field(O,pos_O,pos_O+E->get_log2binomial(BLOCK_SIZE,c)-1));
		sum += popcount(((2<<(i%BLOCK_SIZE))-1) & v);
		r = sum;
		if( ((1<<(i%BLOCK_SIZE)) & v) != 0 ) {
			return true;
		}
		else {
			r = i-r+1;
			return false;
		}
	}

	size_t BitSequenceRRR::rank0(size_t i) const
	{
		if(i+1==0) return 0;
		return 1+i-rank1(i);
	}

	size_t BitSequenceRRR::rank1(size_t i) const
	{
		if(i+1==0) return 0;
								 // patch for 32-64 bits bad coding, to be removed in the future
		if((uint)(i+1)==0) return 0;
		uint nearest_sampled_value = i/BLOCK_SIZE/sample_rate;
		uint sum = get_field(C_sampling,C_sampling_field_bits,nearest_sampled_value);
		uint pos_O = get_field(O_pos,O_pos_field_bits,nearest_sampled_value);
		uint pos = i/BLOCK_SIZE;
		uint k=nearest_sampled_value*sample_rate;
		if(k%2==1 && k<pos) {
			uint aux = get_field(C,C_field_bits,k);
			sum += aux;
			pos_O += E->get_log2binomial(BLOCK_SIZE,aux);
			k++;
		}
		unsigned char * a = (unsigned char *)C;
		size_t mask = 0x0F;
		a += k/2;
		while(k<(uint)max(0,(int)pos-1)) {
			assert(((*a)&mask)==get_field(C,C_field_bits,k));
			assert((*a)/16==get_field(C,C_field_bits,k+1));
			sum += ((*a)&mask)+(*a)/16;
			pos_O += E->get_log2binomial(BLOCK_SIZE,((*a)&mask))+E->get_log2binomial(BLOCK_SIZE,((*a)/16));
			a++;
			k+=2;
		}
		if(k<pos) {
			size_t aux = get_field(C,C_field_bits,k);
			sum += aux;
			pos_O += E->get_log2binomial(BLOCK_SIZE,aux);
			k++;
		}
		size_t c = get_field(C,C_field_bits,pos);
		sum += popcount(((2<<(i%BLOCK_SIZE))-1) & E->short_bitmap(c,get_var_field(O,pos_O,pos_O+E->get_log2binomial(BLOCK_SIZE,c)-1)));
		return sum;
	}

	size_t BitSequenceRRR::select0(size_t i) const
	{
		if(i==0) return (uint)-1;
		if(i>length-ones) return (uint)-1;

		// Search over partial sums
		size_t start=0;
		size_t end=C_sampling_len-1;
		size_t med, acc=0, pos;
		while(start<end-1) {
			med = (start+end)/2;
			acc = med*sample_rate*BLOCK_SIZE-get_field(C_sampling,C_sampling_field_bits,med);
			if(acc<i) {
				if(med==start) break;
				start=med;
			}
			else {
				if(end==0) break;
				end = med-1;
			}
		}
		acc = get_field(C_sampling,C_sampling_field_bits,start);
		while(start<C_len-1 && acc+sample_rate*BLOCK_SIZE==get_field(C_sampling,C_sampling_field_bits,start+1)) {
			start++;
			acc +=sample_rate*BLOCK_SIZE;
		}
		acc = start*sample_rate*BLOCK_SIZE-acc;
		pos = (start)*sample_rate;
		size_t pos_O = get_field(O_pos,O_pos_field_bits,start);

		// Sequential search over C
		size_t s = 0;
		for(;pos<C_len;pos++) {
			s = get_field(C,C_field_bits,pos);
			if(acc+BLOCK_SIZE-s>=i) break;
			pos_O += E->get_log2binomial(BLOCK_SIZE,s);
			acc += BLOCK_SIZE-s;
		}
		pos = (pos)*BLOCK_SIZE;

		// Search inside the block
		while(acc<i) {
			size_t new_posO = pos_O+E->get_log2binomial(BLOCK_SIZE,s);
			size_t block = E->short_bitmap(s,get_var_field(O,pos_O,new_posO-1));
			pos_O = new_posO;
			new_posO = 0;
			while(acc<i && new_posO<BLOCK_SIZE) {
				pos++;new_posO++;
				acc += (((block&1)==0)?1:0);
				block = block/2;
			}
		}
		pos--;
		assert(acc==i);
		assert(rank0(pos)==i);
		assert(!access(pos));
		return pos;
	}

	size_t BitSequenceRRR::select1(size_t i) const
	{
		if(i==0) return -1;
		if(i>ones) return -1;
		// Search over partial sums
		size_t start=0;
		size_t end=C_sampling_len-1;
		size_t med, acc=0, pos;
		while(start<end-1) {
			med = (start+end)/2;
			acc = get_field(C_sampling,C_sampling_field_bits,med);
			if(acc<i) {
				if(med==start) break;
				start=med;
			}
			else {
				if(end==0) break;
				end = med-1;
			}
		}
		acc = get_field(C_sampling,C_sampling_field_bits,start);
		while(start<C_len-1 && acc==get_field(C_sampling,C_sampling_field_bits,start+1)) start++;
		pos = (start)*sample_rate;
		size_t pos_O = get_field(O_pos,O_pos_field_bits,start);
		acc = get_field(C_sampling,C_sampling_field_bits,start);

		// Sequential search over C
		size_t s = 0;
		for(;pos<C_len;pos++) {
			s = get_field(C,C_field_bits,pos);
			if(acc+s>=i) break;
			pos_O += E->get_log2binomial(BLOCK_SIZE,s);
			acc += s;
		}
		pos = (pos)*BLOCK_SIZE;

		// Search inside the block
		while(acc<i) {
			size_t new_posO = pos_O+E->get_log2binomial(BLOCK_SIZE,s);
			size_t block = E->short_bitmap(s,get_var_field(O,pos_O,new_posO-1));
			pos_O = new_posO;
			new_posO = 0;
			while(acc<i && new_posO<BLOCK_SIZE) {
				pos++;new_posO++;
				acc += (((block&1)!=0)?1:0);
				block = block/2;
			}
		}
		pos--;
		assert(acc==i);
		assert(rank1(pos)==i);
		assert(access(pos));
		return pos;
	}

	// bool show_once = false;
	size_t BitSequenceRRR::getSize() const
	{
		// if(!show_once) {
		// 	show_once=true;
		// 	cout << "This bitmap has " << length << " bits, ie ~" << length/8 << " bytes plain" << endl;
		// 	cout << "C     :   " << uint_len(C_len,C_field_bits)*sizeof(uint) << endl;
		// 	cout << "O     :   " << O_len*sizeof(uint) << endl;
		// 	cout << "SampC :   " << uint_len(C_sampling_len,C_sampling_field_bits)*sizeof(uint) << endl;
		// 	cout << "SampO :   " << uint_len(O_pos_len,O_pos_field_bits)*sizeof(uint) << endl;
		// }
		// we consider E to be free (64K shared among all the RRR02 bitmaps)
		size_t sum = sizeof(BitSequenceRRR);
		sum += uint_len(C_len,C_field_bits)*sizeof(uint);
		sum += O_len*sizeof(uint);
		sum += uint_len(C_sampling_len,C_sampling_field_bits)*sizeof(uint);
		sum += uint_len(O_pos_len,O_pos_field_bits)*sizeof(uint);
		return sum;
	}

	BitSequenceRRR::~BitSequenceRRR() {
		if(C!=NULL) delete [] C;
		if(O!=NULL) delete [] O;
		if(C_sampling!=NULL) delete [] C_sampling;
		if(O_pos!=NULL) delete [] O_pos;
		E = E->unuse();
	}

	void BitSequenceRRR::save(ofstream & f) const
	{
		/*cout << "===================" << endl;
		cout << "length = " << length << endl;
		cout << "ones = " << ones << endl;
		cout << "C_len = " << C_len << endl;
		cout << "C_field_bits = " << C_field_bits << endl;
		cout << "O_len = " << O_len << endl;
		cout << "O_bits_len = " << O_bits_len << endl;
		cout << "sample_rate = " << sample_rate << endl;
		cout << "C_alen = " << uint_len(C_len,C_field_bits) << endl;
		cout << "O_alen = " << O_len << endl;*/
		uint wr = RRR02_HDR;
		saveValue(f,wr);
		saveValue(f,length);
		saveValue(f,ones);
		saveValue(f,C_len);
		saveValue(f,C_field_bits);
		saveValue(f,O_len);
		saveValue(f,O_bits_len);
		saveValue(f,sample_rate);
		saveValue(f,C,uint_len(C_len,C_field_bits));
		saveValue(f,O,O_len);
	}

	BitSequenceRRR * BitSequenceRRR::load(ifstream & f) {
		BitSequenceRRR * ret = NULL;
		try
		{
			ret = new BitSequenceRRR();
			uint type = loadValue<uint>(f);
			// TODO:throw an exception!
			if(type!=RRR02_HDR) {
				abort();
			}
			ret->length = loadValue<size_t>(f);
			//cout << ret->length << endl;
			ret->ones = loadValue<size_t>(f);
			//cout << ret->ones << endl;
			ret->C_len = loadValue<uint>(f);
			//cout << ret->C_len << endl;
			ret->C_field_bits = loadValue<uint>(f);
			//cout << ret->C_field_bits << endl;
			ret->O_len = loadValue<uint>(f);
			//cout << ret->O_len << endl;
			ret->O_bits_len = loadValue<uint>(f);
			//cout << ret->O_bits_len << endl;
			ret->sample_rate = loadValue<uint>(f);
			//cout << ret->sample_rate << endl;
			//ret->length = loadValue<uint>(f);
			ret->C = loadValue<uint>(f,uint_len(ret->C_len,ret->C_field_bits));
			ret->O = loadValue<uint>(f,ret->O_len);
			ret->create_sampling(ret->sample_rate);
			return ret;
		}
		catch(exception e) {
			delete ret;
		}
		return NULL;
	}
}
