/* bitstring.cpp
 * Copyright (C) 2009, Francisco Claude, all rights reserved.
 *
 * bitstring definition
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

#include <libcdsBasics.h>
#include <BitString.h>

namespace cds_utils
{

    BitString::BitString(ifstream & input) {
        assert(input.good());
        input.read((char*)&length,sizeof(size_t));
        input.read((char*)&uintLength,sizeof(size_t));
        data = new uint[uintLength];
        input.read((char*)data,uintLength*sizeof(uint));
    }

    void BitString::initData(const size_t len) {
        length = len;
        uintLength = length/W+1;
        data = new uint[uintLength];
        for(uint i=0;i<uintLength;i++)
            data[i] = 0;
    }

    BitString::BitString(const size_t len) {
        initData(len);
    }

    BitString::BitString(const vector<uint> fields, const size_t len) {
        initData(len);
        for(size_t i=0; i<uintLength; i++)
            data[i] = fields[i];
    }

    BitString::BitString(const uint * array, const size_t len) {
        initData(len);
        for(size_t i=0; i<uintLength; i++)
            data[i] = array[i];
    }

    BitString::~BitString() {
        delete [] data;
    }

    void BitString::save(ofstream & out) const
    {
        assert(out.good());
        out.write((char*)&length,sizeof(size_t));
        out.write((char*)&uintLength,sizeof(size_t));
        out.write((char*)data,uintLength*sizeof(uint));
    }

};
