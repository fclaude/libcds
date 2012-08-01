/* WaveletTreeNoptrs.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * WaveletTreeNoptrs definition
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

#ifndef _WVTREE_NOPTRSS_H
#define _WVTREE_NOPTRSS_H

#include <iostream>
#include <cassert>
#include <libcdsBasics.h>
#include <BitSequence.h>
#include <BitSequenceBuilder.h>
#include <Sequence.h>
#include <Mapper.h>

using namespace std;

namespace cds_static
{

    class WaveletTreeNoptrsS : public Sequence
    {
        public:

            /** Builds a Wavelet Tree for the string
             * pointed by symbols assuming its length
             * equals n and uses bmb to build the bitsequence
             * @param bmb builder for the bitmaps in each level.
             * @param am alphabet mapper (we need all symbols to be used).
             * */
            WaveletTreeNoptrsS(uint * symbols, size_t n, BitSequenceBuilder * bmb, Mapper * am, bool deleteSymbols = false);
            WaveletTreeNoptrsS(const Array &symb, BitSequenceBuilder * bmb, Mapper * am);
            //
            /** Destroys the Wavelet Tree */
            virtual ~WaveletTreeNoptrsS();

            virtual size_t rank(uint symbol, size_t pos) const;
            virtual size_t select(uint symbol, size_t j) const;
            virtual uint access(size_t pos) const;
            virtual size_t getSize() const;

            size_t count(uint symbol) const;

            virtual void save(ofstream & fp) const;
            static WaveletTreeNoptrsS * load(ifstream & fp);

        protected:
            WaveletTreeNoptrsS();

            Mapper * am;
            /** Only one bit-string for the Wavelet Tree. */
            BitSequence **bitstring, *occ;

            /** Length of the string. */
            size_t n;

            /** Height of the Wavelet Tree. */
            uint height, max_v;

            /** Obtains the maximum value from the string
             * symbols of length n */
            uint max_value(uint *symbols, size_t n);

            /** How many bits are needed to represent val */
            uint bits(uint val);

            /** Returns true if val has its ind-th bit set
             * to one. */
            bool is_set(uint val, uint ind) const;

            /** Sets the ind-th bit in val */
            uint set(uint val, uint ind) const;

            /** Recursive function for building the Wavelet Tree. */
            void build_level(uint **bm, uint *symbols, uint length, uint *occs);
    };
};
#endif
