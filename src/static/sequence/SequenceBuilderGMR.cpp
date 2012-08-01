/*  SequenceBuilderGMR.cpp
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

#include <SequenceBuilderGMR.h>

namespace cds_static
{

	SequenceBuilderGMR::SequenceBuilderGMR(BitSequenceBuilder * bsb, SequenceBuilder * sqb, uint chunk_len) {
		this->bsb = bsb;
		this->sqb = sqb;
		this->chunk_len = chunk_len;
		bsb->use();
		sqb->use();
	}

	SequenceBuilderGMR::~SequenceBuilderGMR() {
		bsb->unuse();
		sqb->unuse();
	}

	Sequence * SequenceBuilderGMR::build(uint * sequence, size_t len) {
		uint cl = chunk_len;
		if(chunk_len==0) {
			uint s=0;
			for(size_t i=0;i<len;i++)
				s = max(s,sequence[i]);
			cl = 2*s;
		}
		cl = max(cl,(uint)64);
		return new SequenceGMR(sequence, len, cl, bsb, sqb);
	}

	Sequence * SequenceBuilderGMR::build(const Array & seq) {
		uint cl = chunk_len;
		if(chunk_len==0) {
			uint s=0;
			for(size_t i=0;i<seq.getLength();i++)
				s = max(s,seq[i]);
			cl = 2*s;
		}
		cl = max(cl,(uint)64);
		return new SequenceGMR(seq, cl, bsb, sqb);
	}
};
