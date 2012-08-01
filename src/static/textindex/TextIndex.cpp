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

/* General interface for using the compressed index libraries */

#include <TextIndex.h>

namespace cds_static
{

	/** Reads a text index determining the type */
	TextIndex * TextIndex::load(ifstream & fp) {
		uint r = loadValue<uint>(fp);
		size_t pos = fp.tellg();
		fp.seekg(pos-sizeof(uint));
		switch(r) {
			case CSA_HDR: return TextIndexCSA::load(fp);
		}
		return NULL;
	}
};
