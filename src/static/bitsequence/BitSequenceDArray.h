/* BitSequenceSDArray.h
 * Copyright (C) 2009, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This class is a wrapper for the file sdarraySadakane,
 * implemented by K. Sadakane.
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

#ifndef _BITSEQUENCEDARRAY_H
#define _BITSEQUENCEDARRAY_H

#include <libcdsBasics.h>
#include <BitSequence.h>
#include <libcdsSDArray.h>
#include <BitString.h>

using namespace cds_utils;

namespace cds_static
{
	class BitSequenceDArray: public BitSequence
	{

		public:
			/** Builds the DArray (Sadakane's dense version for rank/select*/
			BitSequenceDArray(const BitString & bs);
			/** Builds the DArray */
			BitSequenceDArray(uint * buff, size_t len);
			virtual ~BitSequenceDArray();
			virtual size_t select1(size_t i) const;
			virtual size_t rank0(size_t i) const;
			virtual size_t rank1(size_t i) const;
			virtual size_t getSize() const;
			virtual void save(ofstream & fp) const;
			static BitSequenceDArray * load(ifstream & fp);

		protected:
			uint m;				 //number of 1's
			uint nl, s_ss, s_sl;
			uint *a;			 //bitarray
			uint *lp;
			uint *sl;
			uint *ss;
			uint *p;
			uint *rl;
			uchar *rs;

			/** Internal building function, same parameters as the base constructor. */
			void build(uint *buff, size_t len);

			/** Protected constructor for loaders, you have to initialize data before
			 *using an object built with this constructor.
			 */
			BitSequenceDArray();

	};
};
#endif
