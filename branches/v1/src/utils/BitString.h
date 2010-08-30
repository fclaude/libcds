/* BitString.h
 * Copyright (C) 2009, Francisco Claude, all rights reserved.
 *
 * BitString definition
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

#ifndef BITSTRING_H
#define BITSTRING_H

#include <vector>

#include <libcdsBasics.h>

using namespace std;

namespace cds_utils
{
    /** BitString class
     * @author Francisco Claude
     */
    class BitString
    {
        public:

            /** Reads a BitString from a file stream
             * @param input input file stream
             */
            BitString(ifstream & input);

            /** Creates a BitString with len bits of space */
            BitString(const size_t len);

            /** Creates a bitmap from a vector (up to len bits) */
            BitString(const vector<uint> fields, const size_t len);

            /** Creates a bitmap from an array (len bits) */
            BitString(const uint * array, const size_t len);

            /** Destroys a bitmap */
            ~BitString();

            /** Sets the pos-th bit
             * @param pos position
             * @param bit value [0-1]
             */
            inline void setBit(const size_t pos, const bool bit=true) {
                if(bit) bitset(data,pos);
                else bitclean(data,pos);
            }

            /** Gets the pos-th bit
             * @param pos position
             */
            inline bool getBit(const size_t pos) const
            {
                return bitget(data,pos);
            }

            /** operator [] for getBit
             */
            inline bool operator[](const size_t pos) const
            {
                return bitget(data,pos);
            }

            /** Saves the bitmap to a file
             * @param out file stream
             */
            void save(ofstream & out) const;

            /** Returns the size in bytes of the BitString */
            inline size_t getSize() const
            {
                return uintLength*sizeof(uint)+sizeof(this);
            }

            /** Returns the length in bits of the BitString */
            inline size_t getLength() const
            {
                return length;
            }

            inline uint * getData() const
            {
                return data;
            }

        protected:
            size_t length;
            size_t uintLength;
            uint * data;

            /** Initializes the class fields */
            void initData(const size_t len);
    };
};
#endif
