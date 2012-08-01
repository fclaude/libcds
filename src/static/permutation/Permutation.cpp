/*  Permutation.cpp
 * Copyright (C) 2010, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <Permutation.h>

namespace cds_static
{

	Permutation::Permutation() { length = 0; }

	Permutation::~Permutation() {}

	uint Permutation::pi(uint i) const
	{
		return pi(i,1);
	}

	uint Permutation::revpi(uint i) const
	{
		return revpi(i,1);
	}

	uint Permutation::pi(uint i, uint k) const
	{
		uint ret = i;
		while(k-->0)
			ret = pi(ret);
		return ret;
	}

	uint Permutation::revpi(uint i, uint k) const
	{
		uint ret = i;
		while(k-->0)
			ret = revpi(ret);
		return ret;
	}

	size_t Permutation::getLength() const
	{
		return length;
	}

	void Permutation::save(ofstream & fp) const
	{
		saveValue(fp,length);
	}

	Permutation * Permutation::load(ifstream & fp) {
		uint rd = loadValue<uint>(fp);
		size_t pos = fp.tellg();
		fp.seekg(pos - sizeof(uint),ios::beg);
		switch(rd) {
			case MRRRPERM: return PermutationMRRR::load(fp);
			break;
			case WTPERM: return PermutationWT::load(fp);
			break;
		}
		return NULL;
	}

};
