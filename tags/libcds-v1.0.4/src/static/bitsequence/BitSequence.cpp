/* BitSequence.cpp
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

#include "BitSequence.h"

namespace cds_static
{

    size_t BitSequence::rank0(const size_t i) const
    {
        return i+1-rank1(i);
    }

    size_t BitSequence::rank1(const size_t i) const
    {
        if(i>=length) return (size_t)-1;
        if(ones==0) return 0;
        if(ones==length) return i+1;
        size_t ini = 1;
        size_t fin = ones;
        while(ini<fin) {
            size_t pos = (ini+fin)/2;
            size_t bp = select1(pos);
            if(bp==i) return pos;
            if(bp<i)
                ini = pos+1;
            else
                fin = pos-1;
        }
        if(select1(ini)>i) return ini-1;
        return ini;
    }

    size_t BitSequence::select0(const size_t i) const
    {
        if(i>length-ones) return -1;
        if(i==0) return -1;
        if(ones==0) return i-1;
        size_t ini = 0;
        size_t fin = length-1;
        while(ini<fin) {
            size_t pos = (ini+fin)/2;
            size_t br = rank0(pos);
            if(br<i)
                ini = pos+1;
            else
                fin = pos;
        }
        return ini;
    }

    size_t BitSequence::select1(const size_t i) const
    {
        if(i>ones) return -1;
        if(i==0) return -1;
        if(ones==length) return i-1;
        size_t ini = 0;
        size_t fin = length-1;
        while(ini<fin) {
            size_t pos = (ini+fin)/2;
            size_t br = rank1(pos);
            if(br<i)
                ini = pos+1;
            else
                fin = pos;
        }
        return ini;
    }

    size_t BitSequence::selectNext1(const size_t i) const
    {
        return select1((i==0?0:rank1(i-1))+1);
    }

    size_t BitSequence::selectPrev1(const size_t i) const
    {
        size_t v = rank1(i);
        if(v<2) return (size_t)-1;
        return select1(v-1);
    }

    size_t BitSequence::selectNext0(const size_t i) const
    {
        return select0((i==0?0:rank0(i-1))+1);
    }

    size_t BitSequence::selectPrev0(const size_t i) const
    {
        size_t v = rank0(i);
        if(v<2) return (size_t)-1;
        return select0(v-1);
    }

    bool BitSequence::access(const size_t i) const
    {
        return (rank1(i)-(i!=0?rank1(i-1):0))>0;
    }

    size_t BitSequence::getLength() const
    {
        return length;
    }

    size_t BitSequence::countOnes() const
    {
        return ones;
    }

    size_t BitSequence::countZeros() const
    {
        return length-ones;
    }

    BitSequence * BitSequence::load(ifstream & fp) {
        uint r = loadValue<uint>(fp);
        size_t pos = fp.tellg();
        fp.seekg(pos-sizeof(uint));
        switch(r) {
            case RRR02_HDR: return BitSequenceRRR::load(fp);
            case BRW32_HDR: return BitSequenceRG::load(fp);
            //case RRR02_LIGHT_HDR: return BitSequence_rrr02_light::load(fp);
            case SDARRAY_HDR: return BitSequenceSDArray::load(fp);
        }
        return NULL;
    }
};
