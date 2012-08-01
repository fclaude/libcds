/* perm.h
 * Copyright (C) 2005, Diego Arroyuelo, all rights reserved.
 * Copyright (C) 2009, Francisco Claude
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

#ifndef PERMINCLUDED
#define PERMINCLUDED

#include <libcdsBasics.h>
#include <BitSequence.h>
#include <BitSequenceBuilder.h>

using namespace cds_utils;

namespace cds_static
{

	typedef struct sperm
	{
		uint *elems;			 // elements of the permutation
		uint nelems;			 // # of elements
		BitSequence * bmap;		 // bitmap allowing rank() queries in O(1) time
		uint *bwdptrs;			 // array of backward pointers
		uint nbits;				 // log(nelems)
		uint nbwdptrs;			 // # of backward pointers
		uint t;
	} *perm;

	typedef struct
	{
		uint key;
		uint pointer;
	} auxbwd;

	/** Creates a permutation
	 *
	 *  @author Diego Arroyuelo
	 */
	perm createPerm(uint *elems, uint nelems, uint t, BitSequenceBuilder * bmb);

	/** Gets the i-th element of the permutation
	 *
	 *  @author Diego Arroyuelo
	 */
	uint getelemPerm(const perm P, uint i);

	/** Destroys a permutation
	 *
	 *  @author Diego Arroyuelo
	 */
	void destroyPerm(perm P);

	/** Get pi(i)^{-1}
	 *
	 *  @author Diego Arroyuelo
	 */
	uint inversePerm(const perm P, uint i);

	/** Saves a permutation
	 *
	 *  @author Diego Arroyuelo
	 */
	uint savePerm(const perm P, ofstream & f);

	/** Loads a permutation
	 *
	 *  @author Diego Arroyuelo
	 */
	perm loadPerm(ifstream & f);

	/** Returns the size of the data structure
	 *
	 *  @author Diego Arroyuelo
	 */
	uint sizeofPerm(const perm P);

};
#endif
