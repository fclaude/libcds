/* mapper.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * static_bitsequence definition
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

#include <Mapper.h>

namespace cds_static
{
	Mapper::Mapper() {
		userCount=0;
	}

	void Mapper::use() {
		userCount++;
	}

	void Mapper::unuse() {
		userCount--;
		assert(userCount>=0);
		if(userCount==0)
			delete this;
	}

	Mapper * Mapper::load(ifstream & input) {
		uint rd;
		input.read((char*)&rd,sizeof(uint));
		size_t pos = input.tellg();
		input.seekg(pos-sizeof(uint));
		switch(rd) {
			case MAPPER_NONE_HDR: return MapperNone::load(input);
			case MAPPER_CONT_HDR: return MapperCont::load(input);
			case MAPPER_REV_HDR: return MapperRev::load(input);
		}
		return NULL;
	}
};
