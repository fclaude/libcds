/* BitSequence.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * BitSequence definition
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

#ifndef _BITSEQUENCE_H
#define _BITSEQUENCE_H

#include <iostream>

#define RRR02_HDR 2
#define BRW32_HDR 3
#define RRR02_LIGHT_HDR 4
#define SDARRAY_HDR 5
#define DARRAY_HDR 6

#include <libcdsBasics.h>
#include <Array.h>
#include <cppUtils.h>

using namespace std;
using namespace cds_utils;

namespace cds_static
{

	/** Base class for static bitsequences, contains many abstract functions, so this can't
	 *  be instantiated. It includes base implementations for rank0, select0 and select1 based
	 *  on rank0.
	 *
	 *  @author Francisco Claude
	 */
	class BitSequence
	{

		public:
			virtual ~BitSequence() {};

			/** Returns the number of zeros until position i */
			virtual size_t rank0(const size_t i) const;

			/** Returns the position of the i-th zero
			 * @return (size_t)-1 if i=0, len if i>num_zeros or the position */
			virtual size_t select0(const size_t i) const;

			/** Returns the number of ones until position i */
			virtual size_t rank1(const size_t i) const;

			/** Returns the position of the i-th one
			 * @return (size_t)-1 if i=0, len if i>num_ones or the position */
			virtual size_t select1(const size_t i) const;

			/** Return the first position starting at i that contains a 1.
			 * In case there are no more ones in the bitsequence, the function
			 * returns the length of the bitmap
			 */
			virtual size_t selectNext1(const size_t i) const;

			/** Return the first position starting at i that contains a 0.
			 * In case there are no more zeros in the bitsequence, the function
			 * returns the length of the bitmap
			 */
			virtual size_t selectNext0(const size_t i) const;

			/** Return the first position starting at i moving to the left that contains a 1.
			 * In case there are no more ones to the left in the bitsequence, the function
			 * returns (size_t)-1.
			 */
			virtual size_t selectPrev1(const size_t i) const;

			/** Return the first position starting at i moving to the left that contains a 0.
			 * In case there are no more zeros to the left in the bitsequence, the function
			 * returns (size_t)-1.
			 */
			virtual size_t selectPrev0(const size_t i) const;

			/** Returns the i-th bit */
			virtual bool access(const size_t i) const;
			virtual bool access(const size_t i, size_t &r) const;

			/** Returns the length in bits of the bitmap */
			virtual size_t getLength() const;

			/** Returns how many ones are in the bitstring */
			virtual size_t countOnes() const;

			/** Returns how many zeros are in the bitstring */
			virtual size_t countZeros() const;

			/** Returns the size of the structure in bytes */
			virtual size_t getSize() const=0;

			/** Stores the bitmap given a file pointer*/
			virtual void save(ofstream & fp) const=0;

			/** Reads a bitmap determining the type */
			static BitSequence * load(ifstream & fp);

		protected:
			/** Length of the bitstring */
			size_t length;
			/** Number of ones in the bitstring */
			size_t ones;

	};

};

#include<BitSequenceRG.h>
#include<BitSequenceRRR.h>
#include<BitSequenceSDArray.h>
#include<BitSequenceDArray.h>
#endif							 /* _STATIC_BITSEQUENCE_H */
