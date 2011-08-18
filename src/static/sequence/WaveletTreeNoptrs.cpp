/* WaveletTreeNoptrs.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * WaveletTreeNoptrs definition
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

#include <WaveletTreeNoptrs.h>

namespace cds_static
{
    WaveletTreeNoptrs::WaveletTreeNoptrs(const Array & a, BitSequenceBuilder * bmb, Mapper * am) : Sequence(0) {
        bmb->use();
        this->n=a.getLength();
        this->length = n;
        this->am=am;
        am->use();
        uint * symbols = new uint[n];
        for(size_t i=0;i<n;i++)
            symbols[i] = am->map(a[i]);
        max_v=am->map(a.getMax());
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
        build_level(_bm,new_symb,0,new_n,0);
        bitstring = new BitSequence*[height];
        for(uint i=0;i<height;i++) {
            bitstring[i] = bmb->build(_bm[i],new_n);
            delete [] _bm[i];
        }
        delete [] _bm;

        // delete [] new_symb; // already deleted in build_level()!
        delete [] oc;
        bmb->unuse();
    }

    WaveletTreeNoptrs::WaveletTreeNoptrs(uint * symbols, size_t n, BitSequenceBuilder * bmb, Mapper * am, bool deleteSymbols) : Sequence(n) {
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
        build_level(_bm,new_symb,0,new_n,0);
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

    // symbols is an array of elements of "width" bits
    WaveletTreeNoptrs::WaveletTreeNoptrs(uint * symbols, size_t n, uint width, BitSequenceBuilder * bmb, Mapper * am, bool deleteSymbols) : Sequence(n) {
        bmb->use();
        this->n=n;
        this->am=am;
        am->use();
        for(uint i=0;i<n;i++)
            set_field(symbols, width, i, am->map(get_field(symbols, width, i)));
        max_v=max_value(symbols, width, n);
        height=bits(max_v);
        uint *occurrences=new uint[max_v+1];
        for(uint i=0;i<=max_v;i++) occurrences[i]=0;
        for(uint i=0;i<n;i++)
            occurrences[get_field(symbols, width, i)]++;
        uint to_add=0;
        for(uint i=0;i<max_v;i++)
            if(occurrences[i]==0) to_add++;
        uint * new_symb = new uint[((n+to_add)*width)/W + 1];
        for(uint i=0;i<n;i++)
            set_field(new_symb, width, i, get_field(symbols, width, i));

        if (deleteSymbols) {
            delete [] symbols;
            symbols = 0;
        }

        to_add = 0;
        for(uint i=0;i<max_v;i++)
        if(occurrences[i]==0) {
            occurrences[i]++;
            set_field(new_symb, width, n+to_add, i);
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
        build_level(_bm,new_symb,width,0,new_n,0);
        bitstring = new BitSequence*[height];
        for(uint i=0;i<height;i++) {
            bitstring[i] = bmb->build(_bm[i],new_n);
            delete [] _bm[i];
        }
        delete [] _bm;

        if (!deleteSymbols)
            for(uint i=0;i<n;i++)
                set_field(symbols, width, i, am->unmap(get_field(symbols, width, i)));

        // delete [] new_symb; // already deleted in build_level()!
        delete [] oc;
        bmb->unuse();
    }

    WaveletTreeNoptrs::WaveletTreeNoptrs():Sequence(0) {
        bitstring = NULL;
        occ = NULL;
        am = NULL;
    }

    WaveletTreeNoptrs::~WaveletTreeNoptrs() {
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

    void WaveletTreeNoptrs::save(ofstream & fp) const
    {
        uint wr = WVTREE_NOPTRS_HDR;
        saveValue(fp,wr);
        saveValue<size_t>(fp,n);
        saveValue(fp,max_v);
        saveValue(fp,height);
        am->save(fp);
        for(uint i=0;i<height;i++)
            bitstring[i]->save(fp);
        occ->save(fp);
    }

    WaveletTreeNoptrs * WaveletTreeNoptrs::load(ifstream & fp) {
        uint rd = loadValue<uint>(fp);
        if(rd!=WVTREE_NOPTRS_HDR) return NULL;
        WaveletTreeNoptrs * ret = new WaveletTreeNoptrs();
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

    bool WaveletTreeNoptrs::is_set(uint val, uint ind) const
    {
        assert(ind<height);
        return (val & (1<<(height-ind-1)))!=0;
    }

    uint WaveletTreeNoptrs::set(uint val, uint ind) const
    {
        assert(ind<=height);
        return val | (1<<(height-ind-1));
    }

    uint WaveletTreeNoptrs::access(size_t pos) const
    {
        uint level=0;
        uint ret=0;
        size_t start=0;
        size_t end=n-1;
        while(level<height) {
	  if(start==0) before = 0;
	  else before = bitstring[level]->rank1(start-1);
            assert(pos>=start && pos<=end);
            if(bitstring[level]->access(pos)) {
                ret=set(ret,level);
                pos=bitstring[level]->rank1(pos-1)-before;
                start=(bitstring[level]->rank1(end)-before);
                start=end-start+1;
                pos+=start;
            }
            else {
                pos=pos-start-(bitstring[level]->rank1(pos)-before);
                end=end-start-(bitstring[level]->rank1(end)-before);
                end+=start;
                pos+=start;
            }
            level++;
        }
        return am->unmap(ret);
    }

    size_t WaveletTreeNoptrs::rank(uint symbol, size_t pos) const
    {
        symbol = am->map(symbol);
        uint level=0;
        size_t start=0;
        size_t end=n-1;
        size_t count=0;
        size_t before=0;
        while(level<height) {
            if(start==0) before = 0;
            else before = bitstring[level]->rank1(start-1);

            if(is_set(symbol,level)) {
                pos=bitstring[level]->rank1(pos)-before-1;
                count=pos+1;
                start=(bitstring[level]->rank1(end)-before);
                start=end-start+1;
                pos+=start;
            }
            else {
                pos=pos-start+bitstring[level]->rank1(start-1)-
                    bitstring[level]->rank1(pos);
                count=pos+1;
                end=end-start-(bitstring[level]->rank1(end)-before);
                end+=start;
                pos+=start;
            }
            level++;
            if(count==0) return 0;
        }
        return count;
    }

    size_t WaveletTreeNoptrs::select(uint symbol, size_t j) const
    {
        symbol = am->map(symbol);
        uint mask = (1<<height)-2;
        uint sum=2;
        uint level = height-1;
        size_t pos=j;
        while(true) {
            size_t start = get_start(symbol,mask);
            size_t end = min((size_t)max_v+1,start+sum);
            start = (start==0)?0:(occ->select1(start)+1);
            end = occ->select1(end+1)-1;
            if(is_set(symbol,level)) {
                uint ones_start = bitstring[level]->rank1(start-1);
                pos = bitstring[level]->select1(ones_start+pos)-start+1;
            }
            else {
                uint ones_start = bitstring[level]->rank1(start-1);
                pos = bitstring[level]->select0(start-ones_start+pos)-start+1;
            }
            mask <<=1;
            sum <<=1;
            if(level==0) break;
            level--;
        }
        return pos-1;
    }

    size_t WaveletTreeNoptrs::getSize() const
    {
        size_t ptrs = sizeof(WaveletTreeNoptrs)+height*sizeof(Sequence*);
        size_t bytesBitstrings = 0;
        for(uint i=0;i<height;i++)
            bytesBitstrings += bitstring[i]->getSize();
        return bytesBitstrings+occ->getSize()+ptrs;
    }

    void WaveletTreeNoptrs::build_level(uint **bm, uint *symbols, uint level, uint length, uint offset) {
        if(level==height) {
            delete [] symbols;
            return;
        }
        uint cleft=0;
        for(size_t i=0;i<length;i++)
            if(!is_set(symbols[i],level))
                cleft++;
        uint cright=length-cleft;
        uint *left=new uint[cleft], *right=new uint[cright];
        cleft=cright=0;
        for(size_t i=0;i<length;i++)
        if(!is_set(symbols[i],level)) {
            left[cleft++]=symbols[i];
            bitclean(bm[level],offset+i);
        }
        else {
            right[cright++]=symbols[i];
            bitset(bm[level],offset+i);
        }

        delete [] symbols;
        symbols = 0;

        build_level(bm,left,level+1,cleft,offset);
        left = 0;                // Gets deleted in recursion.
        build_level(bm,right,level+1,cright,offset+cleft);
        right = 0;               // Gets deleted in recursion.
        //delete [] left;
        //delete [] right;
    }

    // symbols is an array of elements of "width" bits.
    void WaveletTreeNoptrs::build_level(uint **bm, uint *symbols, unsigned width, uint level, uint length, uint offset) {
        if(level==height) {
            delete [] symbols;
            return;
        }
        uint cleft=0;
        for(size_t i=0;i<length;i++)
            if(!is_set(get_field(symbols, width, i),level))
                cleft++;
        uint cright=length-cleft;
        uint *left=new uint[(cleft*width)/W + 1],
            *right=new uint[(cright*width)/W + 1];
        cleft=cright=0;
        for(size_t i=0;i<length;i++)
        if(!is_set(get_field(symbols,width,i),level)) {
            set_field(left,width,cleft++,get_field(symbols, width,i));
            bitclean(bm[level],offset+i);
        }
        else {
            set_field(right,width,cright++,get_field(symbols,width,i));
            bitset(bm[level],offset+i);
        }

        delete [] symbols;
        symbols = 0;

        build_level(bm,left,width,level+1,cleft,offset);
        left = 0;                // Gets deleted in recursion.
        build_level(bm,right,width,level+1,cright,offset+cleft);
        right = 0;               // Gets deleted in recursion.
        //delete [] left;
        //delete [] right;
    }

    uint WaveletTreeNoptrs::max_value(uint *symbols, size_t n) {
        uint max_v = 0;
        for(size_t i=0;i<n;i++)
            max_v = max(symbols[i],max_v);
        return max_v;
    }

    uint WaveletTreeNoptrs::max_value(uint *symbols, unsigned width, size_t n) {
        uint max_v = 0;
        for(size_t i=0;i<n;i++)
            max_v = max(get_field(symbols, width, i),max_v);
        return max_v;
    }

    uint WaveletTreeNoptrs::bits(uint val) {
        uint ret = 0;
        while(val!=0) {
            ret++;
            val >>= 1;
        }
        return ret;
    }

    size_t WaveletTreeNoptrs::count(uint symbol) const
    {
        return occ->select1(am->map(symbol))-occ->select1(am->map(symbol)-1)+1;
    }

    uint WaveletTreeNoptrs::quantile(size_t left,size_t right,uint q)
    {
        pair<uint,size_t> res = quantile_freq(left,right,q);
        return res.first;
    }

    pair<uint32_t,size_t> WaveletTreeNoptrs::quantile_freq(size_t left,size_t right,uint q)
    {
        /* decrease q as the smallest element q=1 is        
         * found by searching for 0 */
        q--;

        assert( right >= left );
        assert( (right-left+1) >= q );
        assert( right < length );

        uint sym = 0;
        uint freq = 0;
        uint level = 0;
        size_t start = 0, end = n-1;
        size_t before;
        BitSequence* bs;

        while(level<height)
        {
            bs = bitstring[level];

            /* calc start of level bound */
            if(start == 0) before = 0;
            else before = bs->rank1(start-1);

            /* number of 1s before T[l..r] */
            size_t rank_before_left = bs->rank1(start+left-1);
            /* number of 1s before T[r] */
            size_t rank_before_right = bs->rank1(start+right);
            /* number of 1s in T[l..r] */
            size_t num_ones = rank_before_right - rank_before_left;
            /* number of 0s in T[l..r] */
            size_t num_zeros = (right-left+1) - num_ones;

            /* if there are more than q 0s we go right. left otherwise */
            if(q >= num_zeros) { /* go right */
                freq = num_ones; /* calc freq */
                /* set bit to 1 in sym */
                sym = set(sym,level);
                /* number of 1s before T[l..r] within the current node */
                left = rank_before_left - before;
                /* number of 1s in T[l..r] */
                right = rank_before_right - before - 1;
                q = q - num_zeros;
                /* calc starting pos of right childnode */
                start = end - (bs->rank1(end)-before) + 1;
            } else { /* go left q = q // sym == sym */
                freq = num_zeros; /* calc freq */
                /* number of zeros before T[l..r] within the current node */
                left = left - (rank_before_left - before);
                /* number of zeros in T[l..r] + left bound */
                right = right - (rank_before_right - before);
                /* calc end pos of left childnode */
                end = end - (bs->rank1(end) - before);
            }
            level++;
        }

        /* unmap symbol */
        return pair<uint,size_t>(am->unmap(sym),static_cast<uint>(freq));
    }

};
