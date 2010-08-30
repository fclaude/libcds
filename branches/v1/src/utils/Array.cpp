/* Array.cpp
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

#include <Array.h>

namespace cds_utils
{

    Array::Array(ifstream & input) {
        assert(input.good());
        input.read((char*)&length,sizeof(uint));
        input.read((char*)&maxValue,sizeof(uint));
        input.read((char*)&bitsPerItem,sizeof(uint));
        input.read((char*)&uintLength,sizeof(uint));
        data = new uint[uintLength];
        input.read((char*)data,sizeof(uint)*uintLength);
    }

    Array::Array(const vector<uint> & A, uint bpe) {
        size_t n = A.size();
        if(bpe==0) {
            for(size_t k=0;k<n;k++)
                maxValue = max(maxValue,A[k]);
        }
        else {
            maxValue = (1<<(bpe-1));
        }
        length = n;
        initData();
        assert(bpe==0 || bitsPerItem==bpe);
        for(size_t k=0;k<n;k++) {
            assert(A[k] <= maxValue);
            setField(k, A[k]);
        }
    }

    Array::Array(const vector<uint>::iterator & ini, const vector<uint>::iterator & fin, uint bpe) {
        size_t n = 0;
        for(vector<uint>::iterator it = ini; it!=fin ;++it) {
            maxValue = max(maxValue,*it);
            n++;
        }
        if(bpe!=0) {
            maxValue = (1<<(bpe-1));
        }
        length = n;
        initData();
        assert(bpe==0 || bitsPerItem==bpe);
        n = 0;
        for(vector<uint>::iterator it=ini;it!=fin;++it) {
            assert(*it <= maxValue);
            setField(n, *it);
            n++;
        }
    }

    Array::Array(size_t n, uint _maxValue) {
        length = n;
        maxValue = _maxValue;
        initData();
    }

    Array::Array(uint * A, size_t n, uint bpe) {
        maxValue = 0;
        if(bpe==0) {
            for(size_t k=0;k<n;k++)
                maxValue = max(maxValue,A[k]);
        }
        else {
            maxValue = (1<<(bpe-1));
        }
        length = n;
        initData();
        assert(bpe==0 || bitsPerItem==bpe);
        for(size_t k=0;k<n;k++) {
            assert(A[k] <= maxValue);
            setField(k, A[k]);
        }
    }

    Array::Array(uint * A, size_t i, size_t j, uint bpe) {
        maxValue = 0;
        if(bpe==0) {
            for(size_t k=i;k<=j;k++)
                maxValue = max(maxValue,A[k]);
        }
        else {
            maxValue = (1<<(bpe-1));
        }
        length = j-i+1;
        initData();
        assert(bpe==0 || bitsPerItem==bpe);
        for(size_t k=i;k<=j;k++) {
            assert(A[k] <= maxValue);
            setField(k-i, A[k]);
        }
    }

    Array::~Array() {
        delete [] data;
    }

    void Array::save(ofstream & out) const
    {
        assert(out.good());
        out.write((char*)&length,sizeof(uint));
        out.write((char*)&maxValue,sizeof(uint));
        out.write((char*)&bitsPerItem,sizeof(uint));
        out.write((char*)&uintLength,sizeof(uint));
        out.write((char*)data,sizeof(uint)*uintLength);
    }

    void Array::initData() {
        bitsPerItem = bits(maxValue);
        uintLength = uint_len(length,bitsPerItem);
        data = new uint[uintLength];
        for(size_t i=0;i<uintLength;i++)
            data[i] = 0;
    }
};
