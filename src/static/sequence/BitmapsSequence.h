/* static_sequence.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * static_sequence definition
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

#ifndef _BITMAPSSEQUENCE_H
#define _BITMAPSSEQUENCE_H

#include <libcdsBasics.h>
#include <Sequence.h>
#include <BitSequence.h>
#include <BitSequenceBuilder.h>
#include <Mapper.h>

using namespace cds_utils;

namespace cds_static
{

	/** static_sequence represented using one bitmap per symbol, doesn't support efficient access
	 *
	 *  @author Francisco Claude
	 */
	class BitmapsSequence : public Sequence
	{

		public:
			BitmapsSequence(uint * seq, size_t n, Mapper * am, BitSequenceBuilder * bsb);
			BitmapsSequence(const Array & a, Mapper * am, BitSequenceBuilder * bsb);

			virtual ~BitmapsSequence();

			virtual size_t rank(uint c, size_t i) const;

			virtual size_t select(uint c, size_t i) const;
			virtual size_t selectNext(uint c, size_t i) const;

			virtual uint access(size_t i) const;

			virtual size_t getSize() const;

			virtual void save(ofstream & fp) const;

			/** Reads a bitmap determining the type */
			static BitmapsSequence * load(ifstream & fp);

		protected:
			uint sigma;
			BitSequence  ** bitmaps;
			Mapper * am;

			BitmapsSequence();

	};

};
#endif							 /* _BITMAPSSEQUENCE_H */
