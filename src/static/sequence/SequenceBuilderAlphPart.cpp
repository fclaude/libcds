/*  SequenceBuilderAlphPart.cpp
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

#include <SequenceBuilderAlphPart.h>

namespace cds_static
{

	SequenceBuilderAlphPart::SequenceBuilderAlphPart(SequenceBuilder * groupIndexBuilder, SequenceBuilder * indexBuilder, uint cut) {
		this->groupIndexBuilder = groupIndexBuilder;
		this->indexBuilder = indexBuilder;
		this->cut = cut;
		groupIndexBuilder->use();
		indexBuilder->use();
	}

	SequenceBuilderAlphPart::~SequenceBuilderAlphPart() {
		groupIndexBuilder->unuse();
		indexBuilder->unuse();
	}

	Sequence * SequenceBuilderAlphPart::build(uint * sequence, size_t len) {
		return new SequenceAlphPart(sequence,len,cut,groupIndexBuilder,indexBuilder);
	}

	Sequence * SequenceBuilderAlphPart::build(const Array & seq) {
		return new SequenceAlphPart(seq,cut,groupIndexBuilder,indexBuilder);
	}
};
