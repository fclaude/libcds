/* Sequence.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
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

#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include <cppUtils.h>
#include <libcdsBasics.h>

using namespace cds_utils;
using namespace std;

namespace cds_static
{

    #define BS_HDR 1
    #define WVTREE_HDR 2
    #define WVTREE_NOPTRS_HDR 3
    #define GMR_HDR 4
    #define GMR_CHUNK_HDR 5

    /** Sequence base class
     *
     *  @author Francisco Claude
     */
    class Sequence
    {

        public:
            Sequence(size_t length);
            ~Sequence() {}

            virtual size_t rank(uint c, size_t i) const;
            virtual size_t select(uint c, size_t i) const;
            virtual size_t selectNext(uint c, size_t i) const;
            virtual uint access(size_t i) const;

            virtual size_t getSize() const = 0;
            virtual size_t getLength() const { return length; }

            virtual void save(ofstream & fp) const = 0;

            /** Reads a bitmap determining the type */
            static Sequence * load(ifstream & fp);

        protected:
            size_t length;
            uint sigma;

    };

};

#include <BitmapsSequence.h>
#include <WaveletTree.h>
#include <WaveletTreeNoptrs.h>
#include <SequenceGMR.h>
#include <SequenceGMRChunk.h>

#include <SequenceBuilder.h>
#endif                           /* _SEQUENCE_H */
