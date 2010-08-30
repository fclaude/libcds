/*  BitmapsSequence.cpp
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

#include <BitmapsSequence.h>

namespace cds_static
{
    BitmapsSequence::BitmapsSequence(uint * seq, size_t n, Mapper * am, BitSequenceBuilder * bsb) : Sequence(n) {
        bsb->use();
        sigma = 0;
        //length = n;
        this->am = am;
        am->use();
        for(uint i=0;i<n;i++) sigma=max(sigma,am->map(seq[i]));
        sigma++;
        uint * occ = new uint[sigma+1];
        for(uint i=0;i<=sigma;i++) occ[i] = 0;
        for(uint i=0;i<n;i++) occ[am->map(seq[i])+1]++;
        for(uint i=1;i<sigma;i++) occ[i] += occ[i-1];
        uint * pos = new uint[n];
        for(uint i=0;i<n;i++) pos[i] = 0;
        for(uint i=0;i<n;i++) pos[occ[am->map(seq[i])]++]=i;
        bitmaps = new BitSequence*[sigma];
        uint * bm = new uint[uint_len(n,1)];
        uint pp=0;
        for(uint i=0;i<sigma;i++) {
            for(uint j=0;j<uint_len(n,1);j++)
                bm[j]=0;
            while(pp<occ[i]) {
                bitset(bm,pos[pp]);
                pp++;
            }
            bitmaps[i] = bsb->build(bm,length);
        }
        delete [] bm;
        delete [] occ;
        delete [] pos;
        bsb->unuse();
    }

    BitmapsSequence::BitmapsSequence() : Sequence(0) {
        length = 0;
        sigma = 0;
        bitmaps = NULL;
        am = NULL;
    }

    BitmapsSequence::BitmapsSequence(Array &a, Mapper * am, BitSequenceBuilder * bsb) : Sequence(0) {
        bsb->use();
        length = a.getLength();
        //cout << "length=" << length << endl;
        sigma = a.getMax()+1;
        //cout << "sigma=" << sigma << endl;
        this->am = am;
        am->use();
        uint * occ = new uint[sigma+1];
        for(uint i=0;i<=sigma;i++) occ[i] = 0;
        for(uint i=0;i<length;i++) occ[am->map(a[i])+1]++;
        for(uint i=1;i<sigma;i++) occ[i] += occ[i-1];
        uint * pos = new uint[length];
        for(uint i=0;i<length;i++) pos[i] = 0;
        for(uint i=0;i<length;i++) pos[occ[am->map(a[i])]++]=i;
        bitmaps = new BitSequence*[sigma];
        uint * bm = new uint[uint_len(length,1)];
        uint pp=0;
        for(uint i=0;i<sigma;i++) {
            for(uint j=0;j<uint_len(length,1);j++)
                bm[j]=0;
            while(pp<occ[i]) {
                bitset(bm,pos[pp]);
                pp++;
            }
            //cout << "build " << bm << " len=" << length << " i=" << i << endl;
            bitmaps[i] = bsb->build(bm,length);
        }
        delete [] bm;
        delete [] occ;
        delete [] pos;
        bsb->unuse();
    }

    BitmapsSequence::~BitmapsSequence() {
        if(bitmaps!=NULL) {
            for(uint i=0;i<sigma;i++) {
                if(bitmaps[i]!=NULL) delete bitmaps[i];
            }
            delete [] bitmaps;
        }
        if(am!=NULL) am->unuse();
    }

    size_t BitmapsSequence::rank(uint c, size_t i) const
    {
        if(am->map(c)>=sigma) return (size_t)-1;
        return bitmaps[am->map(c)]->rank1(i);
    }

    size_t BitmapsSequence::select(uint c, size_t i) const
    {
        if(am->map(c)>=sigma) {
            //cout << "c=" << c << " i=" << i << endl;
            return (size_t)-1;
        }
        //cout << "c=" << c << " select=" << bitmaps[am->map(c)]->select1(i) << " rank=" << bitmaps[am->map(c)]->rank1(length-1) << " i=" << i << endl;
        return bitmaps[am->map(c)]->select1(i);
    }

    size_t BitmapsSequence::selectNext(uint c, size_t i) const
    {
        if(am->map(c)>=sigma) return (size_t)-1;
        return bitmaps[am->map(c)]->selectNext1(i);
    }

    uint BitmapsSequence::access(size_t i) const
    {
        for(uint j=0;j<sigma;j++) {
            if(bitmaps[j]->access(i)) return am->unmap(j);
        }
        return (uint)(-1);
    }

    size_t BitmapsSequence::getSize() const
    {
        size_t size = sizeof(BitmapsSequence)+am->getSize();
        for(uint i=0;i<sigma;i++)
            size += bitmaps[i]->getSize();
        return size;
    }

    void BitmapsSequence::save(ofstream & fp) const
    {
        uint wr = BS_HDR;
        saveValue(fp,wr);
        saveValue(fp,length);
        saveValue(fp,sigma);
        for(uint i=0;i<sigma;i++)
            bitmaps[i]->save(fp);
        am->save(fp);
    }

    BitmapsSequence * BitmapsSequence::load(ifstream & fp) {
        uint type = loadValue<uint>(fp);
        assert(type==BS_HDR);
        BitmapsSequence * ret = new BitmapsSequence();
        ret->length = loadValue<uint>(fp);
        ret->sigma = loadValue<uint>(fp);
        ret->bitmaps = new BitSequence*[ret->sigma];
        for(uint i=0;i<ret->sigma;i++)
            ret->bitmaps[i] = BitSequence::load(fp);
        ret->am = Mapper::load(fp);
        ret->am->use();
        return ret;
    }

};
