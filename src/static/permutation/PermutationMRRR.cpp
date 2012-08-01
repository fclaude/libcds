/* PermutationMRRR.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * Permutation
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

#include <PermutationMRRR.h>

namespace cds_static
{

	PermutationMRRR::PermutationMRRR(uint * elems, uint nelems, uint t, BitSequenceBuilder * bmb) {
		permutation = createPerm(elems, nelems, t, bmb);
	}

	PermutationMRRR::PermutationMRRR() {
	}

	PermutationMRRR::~PermutationMRRR() {
		destroyPerm(permutation);
	}

	size_t PermutationMRRR::getSize() const
	{
		return sizeof(PermutationMRRR)+sizeofPerm(permutation);
	}

	uint PermutationMRRR::pi(uint i) const
	{
		return getelemPerm(permutation,i);
	}

	uint PermutationMRRR::revpi(uint i) const
	{
		return inversePerm(permutation,i);
	}

	void PermutationMRRR::save(ofstream & fp) const
	{
		uint wr = MRRRPERM;
		saveValue(fp,wr);
		savePerm(permutation,fp);
	}

	PermutationMRRR * PermutationMRRR::load(ifstream & fp) {
		uint rd = loadValue<uint>(fp);
		if(rd!=MRRRPERM) return NULL;
		PermutationMRRR * ret = new PermutationMRRR();
		ret->permutation = loadPerm(fp);
		return ret;
	}

};
