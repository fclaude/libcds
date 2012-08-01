/* alphabet_mapper_cont.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * alphabet_mapper_cont definition
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

#include <MapperCont.h>

namespace cds_static
{

	MapperCont::MapperCont(const Array & seq, const BitSequenceBuilder & bmb) {
		BitString bs(seq.getMax()+1);
		for(size_t i=0;i<seq.getLength();i++)
			bs.setBit(seq[i]);
		m = bmb.build(bs);
	}

	MapperCont::MapperCont(const uint * A, const size_t len, const BitSequenceBuilder & bmb) {
		uint max_v = 0;
		for(uint i=0;i<len;i++)
			max_v = max(max_v,A[i]);
		max_v++;

		BitString bs(max_v);
		for(size_t i=0;i<len;i++)
			bs.setBit(A[i]);
		m = bmb.build(bs);
	}

	MapperCont::MapperCont() {
	}

	MapperCont::~MapperCont() {
		delete m;
	}

	uint MapperCont::map(uint s) const
	{
		return m->rank1(s);
	}

	uint MapperCont::unmap(uint s) const
	{
		return m->select1(s);
	}

	size_t MapperCont::getSize() const
	{
		return sizeof(MapperCont)+m->getSize();
	}

	void MapperCont::save(ofstream & out) const
	{
		assert(out.good());
		uint wr = MAPPER_CONT_HDR;
		saveValue(out,wr);
		m->save(out);
	}

	MapperCont * MapperCont::load(ifstream & input) {
		assert(input.good());
		uint rd = loadValue<uint>(input);
		if(rd!=MAPPER_CONT_HDR) return NULL;
		MapperCont * ret = new MapperCont();
		ret->m = BitSequence::load(input);
		if(ret->m==NULL) {
			delete ret;
			return NULL;
		}
		return ret;
	}
};
