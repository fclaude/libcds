/* BitSequenceSDArray.cpp
 * Copyright (C) 2009, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This class is a wrapper for sdarraySadakane.cpp, which was implemented
 * by K. Sadakane.
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

#include <BitSequenceSDArray.h>

namespace cds_static
{
	BitSequenceSDArray::BitSequenceSDArray(const BitString & bs) {
		uint * tmp_seq = new uint[uint_len(bs.getLength(),1)+1];
		ones = 0;
		for(uint i=0;i<uint_len(bs.getLength(),1)+1;i++)
			tmp_seq[i] = 0;
		for(uint i=0;i<bs.getLength();i++)
		if(bs[i]) {
			__setbit(tmp_seq,i,1);
			ones++;
		}
		if(ones)
			selects3_construct(&sd,bs.getLength(),tmp_seq);
		this->length = bs.getLength();
		delete [] tmp_seq;
	}

	BitSequenceSDArray::BitSequenceSDArray(uint * buff, size_t len) {
		uint * tmp_seq = new uint[uint_len(len,1)+1];
		ones = 0;
		for(uint i=0;i<uint_len(len,1)+1;i++)
			tmp_seq[i] = 0;
		for(uint i=0;i<len;i++)
		if(bitget(buff,i)) {
			__setbit(tmp_seq,i,1);
			ones++;
		}
		if(ones)
			selects3_construct(&sd,len,tmp_seq);
		this->length = len;
		delete [] tmp_seq;
	}

	BitSequenceSDArray::BitSequenceSDArray() {
		make___selecttbl();
	}

	BitSequenceSDArray::~BitSequenceSDArray() {
		if(ones)
			selects3_free(&sd);
	}

	size_t BitSequenceSDArray::rank1(size_t i) const
	{
		if(i>=length) return -1;
		if(ones)
			return selects3_rank(&sd,i);
		else
			return 0;
	}

	size_t BitSequenceSDArray::select1(size_t i) const
	{
		if(i>ones || i==0) return -1;
		if(ones)
			return selects3_select(&sd,(uint)i);
		else
			return (uint)-1;
	}

	size_t BitSequenceSDArray::selectNext1(size_t i) const
	{
		return selects3_selectnext(&sd,(uint)i);
	}

	size_t BitSequenceSDArray::getSize() const
	{
		return sizeof(BitSequenceSDArray)+(ones?(sd.size + sd.sd0->size + sd.sd1->size):0);
	}

	void BitSequenceSDArray::save(ofstream & fp) const
	{
		uint wr = SDARRAY_HDR;
		saveValue(fp,wr);
		saveValue(fp,length);
		saveValue(fp,ones);
		if(ones)
			selects3_save(&sd,fp);
	}

	BitSequenceSDArray * BitSequenceSDArray::load(ifstream & fp) {
		uint id = loadValue<uint>(fp);
		if(id!=SDARRAY_HDR) return NULL;
		BitSequenceSDArray * ret = new BitSequenceSDArray();
		ret->length = loadValue<size_t>(fp);
		ret->ones = loadValue<size_t>(fp);
		if(ret->ones)
			selects3_load(&ret->sd,fp);
		return ret;
	}

};
