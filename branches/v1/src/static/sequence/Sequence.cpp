/*  Sequence.cpp
 * Copyright (C) 2010, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <Sequence.h>

namespace cds_static
{

    Sequence::Sequence(size_t _length) {
        length = _length;
    }

    size_t Sequence::rank(uint c, size_t i) const
    {
        size_t count = 0;
        for(size_t k=0;k<=i;k++)
            if(access(k)==c) count++;
        return count;
    }

    size_t Sequence::select(uint c, size_t j) const
    {
        size_t count = 0;
        for(size_t k=0;k<length;k++) {
            if(access(k)==c) {
                count++;
                if(count==j) return k;
            }
        }
        return length;
    }

    size_t Sequence::selectNext(uint c, size_t j) const
    {
        uint pos = rank(c,j);
        return select(c,pos+1);
    }

    uint Sequence::access(size_t i) const
    {
        #define prev(k) ((i>0)?rank(k,i-1):0)
        for(uint k=0;k<sigma;k++) {
            if(rank(k,i)>prev(k)) return k;
        }
        // throw exception
        return (uint)-1;
    }

    Sequence * Sequence::load(ifstream & fp) {
        uint type = loadValue<uint>(fp);
	size_t pos = fp.tellg();
	fp.seekg(pos-sizeof(uint),ios::beg);
	switch(type) {
	    case GMR_CHUNK_HDR: return SequenceGMRChunk::load(fp);
	    case GMR_HDR: return SequenceGMR::load(fp);
	    case BS_HDR: return BitmapsSequence::load(fp);
	    case WVTREE_HDR: return WaveletTree::load(fp);
	    case WVTREE_NOPTRS_HDR: return WaveletTreeNoptrs::load(fp);
	}
        return NULL;
    }

};
