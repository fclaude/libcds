/* MapperRev.cpp
 * Copyright (C) 2012, Francisco Claude, all rights reserved.
 *
 * MapperRev definition
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

#include <MapperNone.h>
#include <Array.h>

 using namespace cds_utils;

namespace cds_static
{

	// based on http://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith64BitsDiv
	uint reverse(const uint v) {
		uint res = v;
		const uchar *c = (const uchar*)&v;
		uchar *c2 = (uchar*)&res;
		c2[0] = (c[3] * 0x0202020202ULL & 0x010884422010ULL) % 1023;
		c2[1] = (c[2] * 0x0202020202ULL & 0x010884422010ULL) % 1023;
		c2[2] = (c[1] * 0x0202020202ULL & 0x010884422010ULL) % 1023;
		c2[3] = (c[0] * 0x0202020202ULL & 0x010884422010ULL) % 1023;
		return res;
	}

	MapperRev::MapperRev() { }
	MapperRev::MapperRev(const Array &a) {
		uint maxv = 0;
		for(uint i=0; i < a.getLength(); i++) 
			maxv = max(maxv, a.getField(i));
		this->bits = cds_utils::bits(maxv);
	}

	MapperRev::MapperRev(const uint *A, const size_t len) {
		uint maxv = 0;
		for(uint i=0; i < len; i++) 
			maxv = max(maxv, A[i]);
		this->bits = cds_utils::bits(maxv);
	}

	uint MapperRev::map(const uint s) const {
		uint rev = reverse(s);
		return rev >> (31 - this->bits);
	}

	uint MapperRev::unmap(const uint s) const {return map(s);}

	size_t MapperRev::getSize() const { return sizeof(MapperNone); }

	void MapperRev::save(ofstream & out) const
	{
		uint wr = MAPPER_REV_HDR;
		out.write((char*)&wr,sizeof(uint));
		out.write((char*)&this->bits,sizeof(uint));
	}

	MapperRev * MapperRev::load(ifstream & input) {
		uint rd;
		input.read((char*)&rd,sizeof(uint));
		if(rd!=MAPPER_REV_HDR) return NULL;
		MapperRev * ret = new MapperRev();
		input.read((char*)&ret->bits, sizeof(uint));
		return ret;
	}
};
