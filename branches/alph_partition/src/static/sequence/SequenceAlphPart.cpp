/*  SequenceAlphaPart.cpp
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

#include <SequenceAlphPart.h>

#include <iostream>
#include <algorithm>

using namespace std;

namespace cds_static
{

    uint group(uint x, uint c) {
        uint ret = 0;
        if(x<=c)
            ret = x;
        else
            ret = bits(x-c)-1+c;
        return ret;
    }

    uint offset(uint c, uint x, uint g) {
        uint ret = 0;
        if(x<=c) 
            ret = 0;
        else
            ret = x-c+1-(1<<g);
        return ret;
    }

    uint group_length(uint c, uint pos) {
        uint ret = 0;
        if(pos<=c)
            ret = 1;
        else
            ret = 1<<(pos-c);
        return ret;
    }

    SequenceAlphPart::SequenceAlphPart(uint * seq, size_t n, uint cut, SequenceBuilder * lenIndexBuilder, SequenceBuilder * seqsBuilder) : Sequence(n) { 
        
        // Say we are using the builders
        lenIndexBuilder->use();
        seqsBuilder->use();
        
        // Compute the size of the alphabet
        sigma = 0;
        for(uint i=0;i<n;i++) sigma=max(sigma,seq[i]);
        
        // Compute the frequency of each symbol
        size_t * occ = new size_t[sigma+1];
        for(uint i=0;i<sigma;i++) occ[i] = 0;
        for(uint i=0;i<n;i++) occ[seq[i]]++;

        // Create pairs (frequency,symbol) and then sort by frequency
        pair<size_t,uint> * pairs = new pair<size_t,uint>[sigma+2];
        for(uint i=0;i<=sigma;i++)
            pairs[i] = pair<size_t,uint>(occ[i],i);
        pairs[sigma+1] = pair<size_t,uint>(0,sigma+1);
        sort(pairs,pairs+sigma+1,greater<pair<size_t,uint> >());

        revPermFreq = new uint[sigma+1];
        sigma = 0;
        while(pairs[sigma].first>0) {
            revPermFreq[pairs[sigma].second]=sigma;
            sigma++;
        }
        
        // We don't need occ anymore
        delete [] occ;

        // Fill alphSortedbyFreq and lengthForSymb
        alphSortedByFreq = new uint[sigma+1];
        lengthForSymb = new uint[sigma+1];

        // We estimate maxLen, it may be smaller if many symbols have 0 frequency
        maxLen = group(sigma,cut);

        // Initialize the lengths of each sequence in indexesByLength
        uint * lenLength = new uint[maxLen];
        for(uint i=0;i<=sigma;i++)
            lenLength[i] = 0;

        // Compute the actual value for lenLengths and maxLen
        for(uint i=0;i<=sigma;i++) {
            if(pairs[i].first==0) break;
            alphSortedByFreq[i] = pairs[i].second;
            uint sl = group(i,cut);
            lengthForSymb[pairs[i].second] = sl; 
            lenLength[sl]++;
            maxLen = sl;
        }

        // Now we build lengthsIndex
        uint * tmpSeq = new uint[n];
        for(uint i=0;i<n;i++)
            tmpSeq[i] = lengthForSymb[seq[i]];
        lengthsIndex = lenIndexBuilder->build(tmpSeq,n);
        delete [] tmpSeq;

        // Now we build the other sequences
        uint ** seqs = new uint*[maxLen-cut+1];
        for(uint i=0;i<maxLen-cut+1;i++) 
            seqs[i] = new uint[lenLength[i+cut]];
        // Lets compute the offsets
        symbOffset = new uint[sigma+1];
        for(uint i=0;i<maxLen;i++)
            lenLength[i] = 0;
        for(uint i=0;i<=sigma;i++)
            symbOffset[alphSortedByFreq[i]]=lenLength[lengthForSymb[alphSortedByFreq[i]]]++;
        // We maintain the positions in lenLenght (not needed anymore)
        for(uint i=0;i<maxLen;i++)
            lenLength[i] = 0;
        for(uint i=0;i<n;i++) {
            if(seq[i]>=cut) {
                seqs[lengthForSymb[seq[i]]][lenLength[lengthForSymb[seq[i]]]++] = symbOffset[seq[i]];
            }
        }

        // We are done with the builders
        lenIndexBuilder->unuse();
        seqsBuilder->unuse();
    }

    SequenceAlphPart::SequenceAlphPart() : Sequence(0) {
        length = 0;
        sigma = 0;
    }

    SequenceAlphPart::~SequenceAlphPart() {
    }

    size_t SequenceAlphPart::rank(uint c, size_t i) const
    {
        uint pos = revPermFreq[c];
        if(pos<=cut) {
            return lengthsIndex->rank(pos,i);
        }
        uint g = group(pos,cut);
        uint o = offset(cut,pos,g);
        return indexesByLength[g]->rank(o,lengthsIndex->rank(g,i));
    }

    size_t SequenceAlphPart::select(uint c, size_t i) const
    {
        uint pos = revPermFreq[c];
        if(pos<=cut) {
            return lengthsIndex->select(pos,i);
        }
        uint g = group(pos,cut);
        uint o = offset(cut,pos,g);
        return lengthsIndex->select(g,indexesByLength[g]->select(o,i));
    }

    uint SequenceAlphPart::access(size_t i) const
    {
        uint g = lengthsIndex->access(i);
        if(g<=cut) return alphSortedByFreq[g];
        uint o = indexesByLength[g]->access(lengthsIndex->rank(g,i));
        return alphSortedByFreq[(1<<(g-cut-1))+o];
    }

    size_t SequenceAlphPart::getSize() const
    {
        return 0;
    }

    void SequenceAlphPart::save(ofstream & fp) const
    {
    }

    SequenceAlphPart * SequenceAlphPart::load(ifstream & fp) {
        uint type = loadValue<uint>(fp);
        return NULL;
    }

};
