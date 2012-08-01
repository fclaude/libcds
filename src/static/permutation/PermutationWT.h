/*  Permutation.h
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

#ifndef __PERMUTATIONWT_H
#define __PERMUTATIONWT_H

#include <libcdsBasics.h>

#include <Sequence.h>
#include <BitSequence.h>
#include <Permutation.h>

using namespace cds_utils;

namespace cds_static
{
	class WaveletTree;

	class PermutationWT:public Permutation
	{
		public:
			PermutationWT(uint * perm, size_t len);
			virtual ~PermutationWT();

			/** Compute the value at position i
			 */
			virtual uint pi(uint i) const;

			/** Compute the position where i appears */
			virtual uint revpi(uint i) const;

			/** Size in bytes
			 */
			virtual size_t getSize() const;

			/** Saves permutation to stream */
			virtual void save(ofstream & fp) const;

			/** Loads the permutation */
			static PermutationWT * load(ifstream & fp);

		protected:
			PermutationWT() {}
			BitSequence * marks;
			Sequence * wt;
			uint runs;
	};
};
#endif
