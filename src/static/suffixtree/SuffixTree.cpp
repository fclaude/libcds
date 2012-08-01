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

#include <SuffixTree.h>

namespace cds_static
{

	SuffixTree * SuffixTree::load(ifstream & fp) {
		size_t r = loadValue<size_t>(fp);
		size_t pos = fp.tellg();
		fp.seekg(pos-sizeof(size_t));
		cout << "Loading "<< r << endl;
		switch(r) {
			case CSTY: return SuffixTreeY::load(fp);
		}
		return NULL;
	}

};
