/**
 *    PermutationWT.cpp
 *    Copyright (C) 2011  Francisco Claude F.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include<PermutationWT.h>

namespace cds_static
{

	PermutationWT::PermutationWT(uint *perm, size_t len) {
		uint b = bits(len-1);
		uint * seq = new uint[len];
		uint * marker = new uint[uint_len(len,1)];
		for(size_t i=0;i<uint_len(len,1);i++)
			marker[i] = 0;

		runs = 0;
		uint last = get_field(perm,b,0);
		seq[get_field(perm,b,0)] = 0;
		bitset(marker,0);

		for(size_t i=1;i<len;i++) {
			if(last > get_field(perm,b,i)) {
				runs++;
				bitset(marker,i);
			}
			seq[get_field(perm,b,i)] = runs;
			last = get_field(perm,b,i);
		}

		wt = new WaveletTreeNoptrs(seq, len, new BitSequenceBuilderRRR(40), new MapperNone());
		marks = new BitSequenceRG(marker, len, 20);
		delete [] seq;
	}

	PermutationWT::~PermutationWT() {
		delete wt;
	}

	uint PermutationWT::pi(uint k) const
	{
		uint v = (uint)marks->rank1(k);
		return (uint)wt->select(v-1, k - marks->select1(v) + 1);
	}

	uint PermutationWT::revpi(uint k) const
	{
		size_t val = 0;
		uint s = wt->access(k, val);
		return marks->select1(s+1) + val - 1;
	}

	size_t PermutationWT::getSize() const
	{
		return marks->getSize()+wt->getSize()+sizeof(PermutationWT);
	}

	void PermutationWT::save(ofstream & out) const
	{
		saveValue(out,WTPERM);
		saveValue(out, length);
		wt->save(out);
		marks->save(out);
	}

	PermutationWT * PermutationWT::load(ifstream &in) {
		uint rd = loadValue<uint>(in);
		if(rd!=WTPERM) return NULL;
		PermutationWT * ret = new PermutationWT();
		ret->length = loadValue<size_t>(in);
		ret->wt = Sequence::load(in);
		ret->marks = BitSequence::load(in);
		return ret;
	}
};
