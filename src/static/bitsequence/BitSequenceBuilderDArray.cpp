/*  BitSequenceBuilderDArray.cpp
 * Copyright (C) 2010, Rodrigo Canovas, all rights reserved.
 *
 * Rodrigo Canovas <rcanovas@dcc.uchile.cl>
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

#include <BitSequenceBuilderDArray.h>

namespace cds_static
{

	BitSequenceBuilderDArray::BitSequenceBuilderDArray() {
	}

	BitSequenceBuilderDArray::~BitSequenceBuilderDArray() {
	}

	BitSequence * BitSequenceBuilderDArray::build(uint * bitseq, size_t len) const
	{
		return new BitSequenceDArray(bitseq,len);
	}

	BitSequence * BitSequenceBuilderDArray::build(const BitString & bs) const
	{
		return new BitSequenceDArray(bs);
	}

};
