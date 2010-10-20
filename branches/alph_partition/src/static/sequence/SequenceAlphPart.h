/* SequenceAlphPart.h
 * Copyright (C) 2010, Francisco Claude, all rights reserved.
 *
 * Sequence definition
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

#ifndef _SEQUENCEALPHPART_H
#define _SEQUENCEALPHPART_H

#include <Sequence.h>

using namespace cds_utils;
using namespace std;

namespace cds_static
{

    /** Sequence Alphabet Partitioning
     *
     *  @author Francisco Claude
     */
    class SequenceAlphPart
    {

        public:

            SequenceAlphPart(uint * seq, size_t n, uint cut, SequenceBuilder * lenIndexBuilder, SequenceBuilder * seqsBuilder);

            virtual ~SequenceAlphPart();

            virtual size_t rank(uint c, size_t i) const;

            virtual size_t select(uint c, size_t i) const;

            virtual size_t selectNext(uint c, size_t i) const;

            virtual uint access(size_t i) const;

            virtual size_t getSize() const = 0;

            virtual size_t getLength() const { return length; }

            virtual void save(ofstream & fp) const = 0;

            static SequenceAlphPart * load(ifstream & fp);

        protected:
            // symbols sorted by frequency
            uint * alphSortedByFreq;
            // offset for each symbol inside their lenght
            uint * symbOffset;
            // length for each symbol in the alphabet
            uint * lengthForSymb;
            // the sequence indexing the lengths of the symbol in each position
            Sequence * lengthsIndex;
            // sequences indexing the elements groupes by length
            Sequence ** indexesByLength;
            // cut corresponds to the number of symbols that are not represented in indexesByLength (the most frequent ones)
            uint cut;
            // maxLen corresponds to the maximum length of a symbol in bits, 
            // this-cut+1 corresponds to the maximum index in indexesByLength.
            uint maxLen;

    };

};

#endif                           /* _SEQUENCEALPHPART_H */
