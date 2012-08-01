/* MapperNone.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * mapper definition
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

namespace cds_static
{
	MapperNone::MapperNone() { }

	uint MapperNone::map(const uint s) const {return s;}

	uint MapperNone::unmap(const uint s) const {return s;}

	size_t MapperNone::getSize() const { return sizeof(MapperNone); }

	void MapperNone::save(ofstream & out) const
	{
		uint wr = MAPPER_NONE_HDR;
		out.write((char*)&wr,sizeof(uint));
	}

	MapperNone * MapperNone::load(ifstream & input) {
		uint rd;
		input.read((char*)&rd,sizeof(uint));
		if(rd!=MAPPER_NONE_HDR) return NULL;
		return new MapperNone();
	}
};
