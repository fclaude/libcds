/* WaveletMatrix.cpp
 * Copyright (C) 2012, Francisco Claude & Gonzalo Navarro, all rights reserved.
 *
 * WaveletMatrix definition
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

#include <WaveletMatrix.h>

namespace cds_static
{

    WaveletMatrix::WaveletMatrix(const Array &symbols2, BitSequenceBuilder * bmb, Mapper * am) : Sequence(n) {
        bmb->use();
        n=symbols2.getLength();
        uint *symbols = new uint[n];
        this->am=am;
        am->use();
        for(uint i=0;i<n;i++)
            symbols[i] = am->map(symbols2.getField(i));
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
        delete [] symbols;

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
        C = new uint[height];
        for(uint i=0;i<height;i++) {
            bitstring[i] = bmb->build(_bm[i],new_n);
            C[i] = bitstring[i]->rank0(new_n - 1);
            delete [] _bm[i];
        }
        delete [] _bm;
        delete [] oc;
        bmb->unuse();
    }

    WaveletMatrix::WaveletMatrix(uint * symbols, size_t n, BitSequenceBuilder * bmb, Mapper * am, bool deleteSymbols) : Sequence(n) {
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
            symbols = NULL;
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
        C = new uint[height];
        for(uint i=0;i<height;i++) {
            bitstring[i] = bmb->build(_bm[i],new_n);
            C[i] = bitstring[i]->rank0(new_n - 1);
            // cout << "C=" << C[i] << endl;
            delete [] _bm[i];
        }
        delete [] _bm;

        if (!deleteSymbols)
            for(uint i=0;i<n;i++)
                symbols[i] = am->unmap(symbols[i]);

        // delete [] new_symb; // already deleted in build_level()!
        delete [] oc;
        bmb->unuse();
        // for(uint i=0;i<height;i++)
        //     cout << "C=" << C[i] << endl;
    }



    WaveletMatrix::WaveletMatrix():Sequence(0) {
        bitstring = NULL;
        occ = NULL;
        am = NULL;
    }

    WaveletMatrix::~WaveletMatrix() {
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
        delete [] C;
    }

    void WaveletMatrix::save(ofstream & fp) const
    {
        uint wr = WVMATRIX_HDR;
        saveValue(fp,wr);
        saveValue<size_t>(fp,n);
        saveValue(fp,max_v);
        saveValue(fp,height);
        saveValue(fp, C, height);
        am->save(fp);
        for(uint i=0;i<height;i++)
            bitstring[i]->save(fp);
        occ->save(fp);
    }

    WaveletMatrix * WaveletMatrix::load(ifstream & fp) {
        uint rd = loadValue<uint>(fp);
        if(rd!=WVMATRIX_HDR) return NULL;
        WaveletMatrix * ret = new WaveletMatrix();
        ret->n = loadValue<size_t>(fp);
        ret->length = ret->n;
        ret->max_v = loadValue<uint>(fp);
        ret->height = loadValue<uint>(fp);
        ret->C = loadValue<uint>(fp, ret->height);
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

    bool WaveletMatrix::is_set(uint val, uint ind) const
    {
        assert(ind<height);
        return (val & (1<<(height-ind-1)))!=0;
    }

    uint WaveletMatrix::set(uint val, uint ind) const
    {
        assert(ind<=height);
        return val | (1<<(height-ind-1));
    }

    uint WaveletMatrix::access(size_t pos) const
    {
        uint ret=0;
        for (uint level = 0; level < height; level++) {
            size_t optR = 0;
            if (bitstring[level]->access(pos, optR)) {
                pos = bitstring[level]->rank0(n - 1) + optR - 1;
                ret = ret | (1 << level);
            } else {
                pos = optR - 1;
            }
        }
        return am->unmap(ret);
    }

    size_t WaveletMatrix::rank(uint symbol, size_t pos) const
    {
        symbol = am->map(symbol);
        uint level = 0;
        size_t start = 0;

        while (level < height) {
            if(is_set(symbol, height - level - 1)) {
                size_t C = bitstring[level]->rank0(n - 1);
                start = C + (start == 0? 0 : bitstring[level]->rank1(start - 1));
                pos = bitstring[level]->rank1(pos) + C - 1;
            }
            else {
                start = (start == 0? 0 : bitstring[level]->rank0(start - 1));
                pos = bitstring[level]->rank0(pos) - 1;
            }
            level++;
        }
        return pos + 1 - start;;
    }

    size_t WaveletMatrix::rselect(uint symbol, size_t j, uint level, size_t pos) const {
        size_t C = bitstring[level]->rank0(n - 1);

        if (is_set(symbol, level))
            pos = C + (pos == 0? 0 : bitstring[level]->rank1(pos - 1));
        else
            pos = (pos == 0? 0 : bitstring[level]->rank0(pos - 1));

        if (level == height - 1) {
            if (pos < C) {
                return bitstring[level]->select0(pos + 1 + j);
            } else {
                return bitstring[level]->select1(pos - C + 1 + j);
            }
        }

        size_t pos2 = rselect(symbol, j, level + 1, pos);

        if (pos2 < C) {
            return bitstring[level]->select0(pos2 + 1);
        } else {
            return bitstring[level]->select1(pos2 - C + 1);
        }
    }

    size_t WaveletMatrix::select(uint symbol, size_t j) const
    {
        symbol = am->map(symbol);
        return rselect(symbol, j, 0, 0);
        // size_t pos = (symbol == 0? -1 : occ->select1(symbol)) + j;
        // for (int level = height - 1; level >= 0; level--) {
        //     size_t C = bitstring[level]->rank0(n - 1);
        //     if (pos < C) {  // left
        //         pos = bitstring[level]->select0(pos + 1);
        //     } else { // right
        //         pos = bitstring[level]->select1(pos - C + 1);
        //     }
        // }
        // return pos;
    }

    size_t WaveletMatrix::getSize() const
    {
        size_t ptrs = sizeof(WaveletMatrix)+height*sizeof(Sequence*);
        size_t bytesBitstrings = 0;
        for(uint i=0;i<height;i++)
            bytesBitstrings += bitstring[i]->getSize();
        return bytesBitstrings /* + occ->getSize() */ + ptrs /* + height*sizeof(uint) */;
    }

    void WaveletMatrix::build_level(uint **bm, uint *symbols, uint length, uint *occs) {
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
                    new_symbols[new_pos0++] = symbols[i];
                    bitclean(bm[level], i);
                } else {
                    new_symbols[new_pos1++] = symbols[i];
                    bitset(bm[level], i);
                }
            }
            delete [] symbols;
            symbols = new_symbols;
        }
        delete [] symbols;
        delete [] new_order;
    }

    uint WaveletMatrix::max_value(uint *symbols, size_t n) {
        uint max_v = 0;
        for(size_t i=0;i<n;i++)
            max_v = max(symbols[i],max_v);
        return max_v;
    }

    uint WaveletMatrix::bits(uint val) {
        uint ret = 0;
        while(val!=0) {
            ret++;
            val >>= 1;
        }
        return ret;
    }

    size_t WaveletMatrix::count(uint symbol) const
    {
        return occ->select1(am->map(symbol))-occ->select1(am->map(symbol)-1)+1;
    }


};
