/*  SequenceBuilderWaveletTree.h
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

#include <libcdsBasics.h>

#include <Mapper.h>
#include <SequenceBuilder.h>
#include <BitSequenceBuilder.h>

#ifndef SEQUENCEBUILDERWAVELETTREE_H
#define SEQUENCEBUILDERWAVELETTREE_H

namespace cds_static
{
	class SequenceBuilderWaveletTree : public SequenceBuilder
	{
		public:
			SequenceBuilderWaveletTree(BitSequenceBuilder * bsb, Mapper * am, wt_coder * wc=NULL);
			virtual ~SequenceBuilderWaveletTree();
			virtual Sequence * build(uint * seq, size_t len);
			virtual Sequence * build(const Array & seq);

		protected:
			BitSequenceBuilder * bsb;
			Mapper * am;
			wt_coder * wc;
	};
};
#endif
