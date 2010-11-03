/* HuffmanCoder.cpp
   Copyright (C) 2008, Francisco Claude, all rights reserved.

   Wrapper for huff written by Gonzalo Navarro

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include <HuffmanCoder.h>

namespace cds_static
{
    using namespace cds_utils;

    HuffmanCoder::HuffmanCoder(uint * symb, size_t n) {
        uint max_v = 0;
        for(size_t i=0;i<n;i++)
            max_v = max(max_v,symb[i]);
        uint * occ = new uint[max_v+1];
        for(size_t i=0;i<max_v+1;i++)
            occ[i] = 0;
        for(size_t i=0;i<n;i++)
            occ[symb[i]]++;
        huff_table = createHuff(occ, max_v);
        delete [] occ;
    }

    HuffmanCoder::HuffmanCoder(uchar * symb, size_t n) {
        uchar max_v = 0;
        for(size_t i=0;i<n;i++)
            max_v = max(max_v,symb[i]);
        uint * occ = new uint[max_v+1];
        for(size_t i=0;i<(uint)max_v+1;i++)
            occ[i] = 0;
        for(size_t i=0;i<n;i++)
            occ[symb[i]]++;
        huff_table = createHuff(occ, max_v);
        delete [] occ;
    }

    HuffmanCoder::HuffmanCoder(Array & seq) {
        uint max_v = seq.getMax();
        uint * occ = new uint[max_v+1];
        for(size_t i=0;i<(uint)max_v+1;i++)
            occ[i] = 0;
        for(size_t i=0;i<seq.getLength();i++)
            occ[seq[i]]++;
        huff_table = createHuff(occ, max_v);
        delete [] occ;
    }

    HuffmanCoder::HuffmanCoder() {
    }

    HuffmanCoder::~HuffmanCoder() {
        freeHuff(huff_table);
    }

    size_t HuffmanCoder::maxLength() const
    {
        return huff_table.depth;
    }

    size_t HuffmanCoder::getSize() const
    {
        return sizeof(HuffmanCoder)+sizeHuff(huff_table);
    }

    size_t HuffmanCoder::encode(uint symb, uint * stream, size_t pos) const
    {
        return encodeHuff(huff_table, symb, stream, pos);
    }

    size_t HuffmanCoder::decode(uint * symb, uint * stream, size_t pos) const
    {
        return decodeHuff(huff_table, symb, stream, pos);
    }

    void HuffmanCoder::save(ofstream & fp) const
    {
        saveValue<uint>(fp,HUFF_HDR);
        saveHuff(huff_table,fp);
    }

    HuffmanCoder * HuffmanCoder::load(ifstream & fp) {
        uint type = loadValue<uint>(fp);
        if(type != HUFF_HDR) {   //throw exception
            return NULL;
        }
        HuffmanCoder * ret = new HuffmanCoder();
        ret->huff_table = loadHuff(fp,1);
        return ret;
    }

};
