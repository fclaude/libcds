/* WaveletTreeNoptrsSS.cpp
 * Copyright (C) 2012, Francisco Claude, all rights reserved.
 *
 * WaveletTreeNoptrsS definition
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

#include <WaveletTreeNoptrsS.h>

namespace cds_static
{

    WaveletTreeNoptrsS::WaveletTreeNoptrsS(const Array &symb, BitSequenceBuilder * bmb, Mapper * am) : Sequence(n) {
        bmb->use();
        this->n=symb.getLength();
        this->am=am;
        bool deleteSymbols = true;
        am->use();
        // This is terrible
        uint *symbols = new uint[n];
        for(uint i=0;i<n;i++)
            symbols[i] = symb.getField(i);
        for(uint i=0;i<n;i++)
            symbols[i] = am->map(symbols[i]);
        max_v=max_value(symbols,n);
        height=bits(max_v);
        uint *occurrences=new uint[max_v+1];
        for(uint i=0;i<=max_v;i++) occurrences[i]=0;
        for(uint i=0;i<n;i++)
            occurrences[symbols[i]]++;
        uint to_add=0;
        for(uint i=0;i<max_v;i++)
            if(occurrences[i]==0) to_add++;
        uint * new_symb = new uint[n+to_add];
        for(uint i=0;i<n;i++)
            new_symb[i] = symbols[i];

        if (deleteSymbols) {
            delete [] symbols;
            symbols = 0;
        }

        to_add = 0;
        for(uint i=0;i<max_v;i++)
        if(occurrences[i]==0) {
            occurrences[i]++;
            new_symb[n+to_add]=i;
            to_add++;
        }
        uint new_n = n+to_add;
        for(uint i=1;i<=max_v;i++)
            occurrences[i] += occurrences[i-1];
        uint *oc = new uint[(new_n+1)/W+1];
        for(uint i=0;i<(new_n+1)/W+1;i++)
            oc[i] = 0;
        for(uint i=0;i<=max_v;i++)
            bitset(oc,occurrences[i]-1);
        bitset(oc,new_n);
        occ = bmb->build(oc,new_n+1);
        delete [] occurrences;
        this->n = new_n;
        uint ** _bm=new uint*[height];
        for(uint i=0;i<height;i++) {
            _bm[i] = new uint[new_n/W+1];
            for(uint j=0;j<new_n/W+1;j++)
                _bm[i][j]=0;
        }

        build_level(_bm,new_symb,new_n,occurrences);
        bitstring = new BitSequence*[height];
        for(uint i=0;i<height;i++) {
            bitstring[i] = bmb->build(_bm[i],new_n);
            delete [] _bm[i];
        }
        delete [] _bm;

        if (!deleteSymbols)
            for(uint i=0;i<n;i++)
                symbols[i] = am->unmap(symbols[i]);

        // delete [] new_symb; // already deleted in build_level()!
        delete [] oc;
        bmb->unuse();
    }

    WaveletTreeNoptrsS::WaveletTreeNoptrsS(uint * symbols, size_t n, BitSequenceBuilder * bmb, Mapper * am, bool deleteSymbols) : Sequence(n) {
        bmb->use();
        this->n=n;
        this->am=am;
        am->use();
        for(uint i=0;i<n;i++)
            symbols[i] = am->map(symbols[i]);
        max_v=max_value(symbols,n);
        height=bits(max_v);
        uint *occurrences=new uint[max_v+1];
        for(uint i=0;i<=max_v;i++) occurrences[i]=0;
        for(uint i=0;i<n;i++)
            occurrences[symbols[i]]++;
        uint to_add=0;
        for(uint i=0;i<max_v;i++)
            if(occurrences[i]==0) to_add++;
        uint * new_symb = new uint[n+to_add];
        for(uint i=0;i<n;i++)
            new_symb[i] = symbols[i];

        if (deleteSymbols) {
            delete [] symbols;
            symbols = 0;
        }

        to_add = 0;
        for(uint i=0;i<max_v;i++)
        if(occurrences[i]==0) {
            occurrences[i]++;
            new_symb[n+to_add]=i;
            to_add++;
        }
        uint new_n = n+to_add;
        for(uint i=1;i<=max_v;i++)
            occurrences[i] += occurrences[i-1];
        uint *oc = new uint[(new_n+1)/W+1];
        for(uint i=0;i<(new_n+1)/W+1;i++)
            oc[i] = 0;
        for(uint i=0;i<=max_v;i++)
            bitset(oc,occurrences[i]-1);
        bitset(oc,new_n);
        occ = bmb->build(oc,new_n+1);
        delete [] occurrences;
        this->n = new_n;
        uint ** _bm=new uint*[height];
        for(uint i=0;i<height;i++) {
            _bm[i] = new uint[new_n/W+1];
            for(uint j=0;j<new_n/W+1;j++)
                _bm[i][j]=0;
        }

        build_level(_bm,new_symb,new_n,occurrences);
        bitstring = new BitSequence*[height];
        for(uint i=0;i<height;i++) {
            bitstring[i] = bmb->build(_bm[i],new_n);
            delete [] _bm[i];
        }
        delete [] _bm;

        if (!deleteSymbols)
            for(uint i=0;i<n;i++)
                symbols[i] = am->unmap(symbols[i]);

        // delete [] new_symb; // already deleted in build_level()!
        delete [] oc;
        bmb->unuse();
    }



    WaveletTreeNoptrsS::WaveletTreeNoptrsS():Sequence(0) {
        bitstring = NULL;
        occ = NULL;
        am = NULL;
    }

    WaveletTreeNoptrsS::~WaveletTreeNoptrsS() {
        if(bitstring) {
            for(uint i=0;i<height;i++)
                if(bitstring[i])
                    delete bitstring[i];
            delete [] bitstring;
        }
        if(occ)
            delete occ;
        if(am)
            am->unuse();
    }

    void WaveletTreeNoptrsS::save(ofstream & fp) const
    {
        uint wr = WVTREE_NOPTRSS_HDR;
        saveValue(fp,wr);
        saveValue<size_t>(fp,n);
        saveValue(fp,max_v);
        saveValue(fp,height);
        am->save(fp);
        for(uint i=0;i<height;i++)
            bitstring[i]->save(fp);
        occ->save(fp);
    }

    WaveletTreeNoptrsS * WaveletTreeNoptrsS::load(ifstream & fp) {
        uint rd = loadValue<uint>(fp);
        if(rd!=WVTREE_NOPTRSS_HDR) return NULL;
        WaveletTreeNoptrsS * ret = new WaveletTreeNoptrsS();
        ret->n = loadValue<size_t>(fp);
        ret->length = ret->n;
        ret->max_v = loadValue<uint>(fp);
        ret->height = loadValue<uint>(fp);
        ret->am = Mapper::load(fp);
        if(ret->am==NULL) {
            delete ret;
            return NULL;
        }
        ret->am->use();
        ret->bitstring = new BitSequence*[ret->height];
        for(uint i=0;i<ret->height;i++)
            ret->bitstring[i] = NULL;
        for(uint i=0;i<ret->height;i++) {
            ret->bitstring[i] = BitSequence::load(fp);
            if(ret->bitstring[i]==NULL) {
                cout << "damn" << i << " " << ret->height << endl;
                delete ret;
                return NULL;
            }
        }
        ret->occ = BitSequence::load(fp);
        if(ret->occ==NULL) {
            delete ret;
            return NULL;
        }
        return ret;
    }

    inline uint get_start(uint symbol, uint mask) {
        return symbol&mask;
    }

    inline uint get_end(uint symbol, uint mask) {
        return get_start(symbol,mask)+!mask+1;
    }

    bool WaveletTreeNoptrsS::is_set(uint val, uint ind) const
    {
        assert(ind<height);
        return (val & (1<<(height-ind-1)))!=0;
    }

    uint WaveletTreeNoptrsS::set(uint val, uint ind) const
    {
        assert(ind<=height);
        return val | (1<<(height-ind-1));
    }

    uint WaveletTreeNoptrsS::access(size_t pos) const
    {
        uint level=0;
        uint ret=0;
        size_t end=n-1;
	    while(level<height) {
            size_t C = bitstring[level]->rank0(end);
            size_t optR = 0;
            if (bitstring[level]->access(pos, optR)) {
                pos = C + optR - 1;
                ret = (ret << 1) | 1;
            } else {
                pos = optR - 1;
                ret = ret << 1;
            }
            level++;
        }
        uint ret2 = 0;
        for (uint i = 0; i < height; i++) {
            ret2 = (ret2 << 1) | (ret & 1);
            ret /= 2;
        }
        return am->unmap(ret2);
    }

    size_t WaveletTreeNoptrsS::rank(uint symbol, size_t pos) const
    {
        symbol = am->map(symbol);
        uint level = 0;
        size_t end = n - 1;
        // size_t count = 0;
        size_t start = 0;
        while (level < height) {
            size_t C = bitstring[level]->rank0(end);
            if(is_set(symbol, height - level - 1)) {
                start = C + (start == 0? 0 : bitstring[level]->rank1(start - 1));
                pos = bitstring[level]->rank1(pos) + C - 1;
                // count = pos + 1 - start;
            }
            else {
                start = (start == 0? 0 : bitstring[level]->rank0(start - 1));
                pos = bitstring[level]->rank0(pos) - 1;
                // count = pos + 1 - start;
            }
            level++;
            // cout << "pos=" << pos << " start=" << start << endl;
            // if(count==0) return 0;
        }
        return pos + 1 - start;;
    }

    size_t WaveletTreeNoptrsS::select(uint symbol, size_t j) const
    {
        size_t end = n - 1;
        symbol = am->map(symbol);
        size_t pos = (symbol == 0? -1 : occ->select1(symbol)) + j;
        for (int level = height - 1; level >= 0; level--) {
            size_t C = bitstring[level]->rank0(end);
            if (pos < C) {  // left
                pos = bitstring[level]->select0(pos + 1);
            } else { // right
                pos = bitstring[level]->select1(pos - C + 1);
            }
        }
        return pos;
    }

    size_t WaveletTreeNoptrsS::getSize() const
    {
        size_t ptrs = sizeof(WaveletTreeNoptrsS)+height*sizeof(Sequence*);
        size_t bytesBitstrings = 0;
        for(uint i=0;i<height;i++)
            bytesBitstrings += bitstring[i]->getSize();
        return bytesBitstrings+occ->getSize()+ptrs;
    }

    void WaveletTreeNoptrsS::build_level(uint **bm, uint *symbols, uint length, uint *occs) {
            // for (uint i = 0; i < length; i++)
            //     cout << " " << symbols[i];
            // cout << endl;
        uint sigma = max_value(symbols, length);
        uint *new_order = new uint[sigma + 1];
        for (uint level = 0; level < height; level++) {
            uint zeroes = 0;
            for (uint i = 0; i < sigma + 1; i++)
                if (!is_set(i, height - level - 1)) {
                    new_order[i] = 0;
                } else {
                    new_order[i] = 1;
                }
            for (uint i = 0; i < length; i++)
                if (!new_order[symbols[i]])
                    zeroes++;
            uint *new_symbols = new uint[length];
            uint new_pos0 = 0, new_pos1 = zeroes;
            for (uint i = 0; i < length; i++) {
                if (!new_order[symbols[i]]) {
                    //cout << "[" << new_pos0 << "]=" << symbols[i] << endl;
                    new_symbols[new_pos0++] = symbols[i];
                    bitclean(bm[level], i);
                } else {
                    //cout << "[" << new_pos1 << "]=" << symbols[i] << endl;
                    new_symbols[new_pos1++] = symbols[i];
                    bitset(bm[level], i);
                }
            }
            delete [] symbols;
            symbols = new_symbols;
            // for (uint i = 0; i < length; i++)
            //     cout << " " << symbols[i];
            // cout << endl;
        }
        delete [] symbols;
        delete [] new_order;
    }

    uint WaveletTreeNoptrsS::max_value(uint *symbols, size_t n) {
        uint max_v = 0;
        for(size_t i=0;i<n;i++)
            max_v = max(symbols[i],max_v);
        return max_v;
    }

    uint WaveletTreeNoptrsS::bits(uint val) {
        uint ret = 0;
        while(val!=0) {
            ret++;
            val >>= 1;
        }
        return ret;
    }

    size_t WaveletTreeNoptrsS::count(uint symbol) const
    {
        return occ->select1(am->map(symbol))-occ->select1(am->map(symbol)-1)+1;
    }


};
