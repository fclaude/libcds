/*  PermutationBuilderMRRR.h
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

#ifndef PERMUTATIONBUILDERMRRR_H
#define PERMUTATIONBUILDERMRRR_H

#include <libcdsBasics.h>
#include <Permutation.h>
#include <PermutationBuilder.h>

namespace cds_static
{

	class PermutationBuilderMRRR : public PermutationBuilder
	{
		public:
			PermutationBuilderMRRR(uint sample, BitSequenceBuilder * bmb);
			virtual ~PermutationBuilderMRRR();
			virtual Permutation * build(uint * perm, uint len) const;
		protected:
			uint sample;
			BitSequenceBuilder * bmb;
	};
};
#endif
