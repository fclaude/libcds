/* array.h
 * Copyright (C) 2009, Francisco Claude, all rights reserved.
 *
 * Array interface
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

#include <fstream>
#include <vector>

#ifndef ARRAY_H
#define ARRAY_H

#include <libcdsBasics.h>

using namespace std;

namespace cds_utils
{
    /** Class for arrays with variable bit-width
     * @author Francisco Claude
     */
    class Array
    {
        public:
            class ArrayModifier
            {
                protected:
                    uint * data;
                    size_t len;
                    uint bitsPerItem;
                    size_t pos;
                public:
                    ArrayModifier(uint * _data, size_t _len, uint _bitsPerItem, size_t _pos)
                        : data(_data), len(_len), bitsPerItem(_bitsPerItem), pos(_pos) {}
                    inline uint operator=(const uint v) {
                        set_field(data, bitsPerItem, pos, v);
                        return v;
                    }
            };
        public:
            /** Reads and Array from a file stream
             * @param input input file stream
             */
            Array(ifstream & input);

            /** Creates an array from a vector
             * @param A vector with the elements
             * @bpe bits per element
             */
            Array(const vector<uint> & A, uint bpe=0);

            /** Creates an array from a vector iterator
             * @param ini initial position
             * @param fin final position
             * @bpe bits per element
             */
            Array(const vector<uint>::iterator & ini, const vector<uint>::iterator & fin, uint bpe=0);

            /** Creates an array copying part of a previously existing array
             * @param A source array
             * @param n length of the source array
             * @param bpe bits per element (0 uses the max in A[i,j]
             */
            Array(uint * A, size_t n, uint bpe = 0);

            /** Creates an array copying part of a previously existing array
             * @param A source array
             * @param i initial position
             * @param j final position
             * @param bpe bits per element (0 uses the max in A[i,j]
             */
            Array(uint * A, size_t i, size_t j, uint bpe = 0);

            /** Creates and array with n elements that can store elements between 0 and _maxValue
             * @param n length (in elements) of the array
             * @param _maxValue maximum value that could be stored in the array
             */
            Array(size_t n, uint _maxValue);

            /** Destroys the array */
            ~Array();

            /** Retrieves Array[pos]
             * @paran pos position
             * @return Array[pos]
             */
            inline uint getField(const size_t pos) const
            {
                assert(pos<length);
                return get_field(data, bitsPerItem, pos);
            }

            /** Assigns v to Array[pos]
             * @paran pos position
             * @param v value
             * @return Array[pos]
             */
            inline uint setField(const size_t pos, const uint v) {
                assert(pos<length); assert(v<=maxValue);
                set_field(data, bitsPerItem, pos, v);
                return v;
            }

            /*destroys the access :-(
            inline ArrayModifier operator[](const uint pos) {
              return ArrayModifier(data,length,bitsPerItem,pos);
            }*/

            /** operator [] for getField
             */
            inline uint operator[](const size_t pos) const
            {
                assert(pos<length);
                return get_field(data, bitsPerItem, pos);
            }

            /** Saves the array into a file */
            void save(ofstream & out) const;

            /** Returns the size of the array in bytes
             */
            inline size_t getSize() const
            {
                return sizeof(uint)*uintLength+sizeof(this);
            }

            /** Returns the length of the array
             */
            inline size_t getLength() const
            {
                return length;
            }

            inline uint getMax() const
            {
                uint maxV = 0;   // default max
                for(size_t i=0; i<length; i++)
                    maxV = max(maxV,getField(i));
                return maxV;
            }

        protected:
            /** Array where the data is stored */
            uint * data;
            /** Length of the array in number of elements */
            size_t length;
            /** Length in bits of each field */
            uint bitsPerItem;
            /** Maximum value stored in the array */
            uint maxValue;
            /** Length of the array in number of uints */
            size_t uintLength;

            /** Initializes the array, all the values to 0
             */
            void initData();
    };
}
#endif
