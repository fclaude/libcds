/* wt_node_internal.cpp
 * Copyright (C) 2008, Francisco Claude.
 * Copyright (C) 2011, Matthias Petri.
 *
 * wt_node_internal
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

#include <wt_node_internal.h>

namespace cds_static
{

    wt_node_internal::wt_node_internal(uint * symbols, size_t n, uint l, wt_coder * c, BitSequenceBuilder * bmb) {
        uint * ibitmap = new uint[n/W+1];
        for(uint i=0;i<n/W+1;i++)
            ibitmap[i]=0;
        for(uint i=0;i<n;i++) {
            if(c->is_set(symbols[i],l))
                bitset(ibitmap,i);
        }
        bitmap = bmb->build(ibitmap, n);
        delete [] ibitmap;
        size_t count_right = bitmap->rank1(n-1);
        size_t count_left = n-count_right+1;
        uint * left = new uint[count_left+1];
        uint * right = new uint[count_right+1];
        count_right = count_left = 0;
        bool match_left = true, match_right = true;
        for(uint i=0;i<n;i++) {
            if(bitmap->access(i)) {
                right[count_right++]=symbols[i];
                if(count_right>1)
                    if(right[count_right-1]!=right[count_right-2])
                        match_right = false;
            }
            else {
                left[count_left++]=symbols[i];
                if(count_left>1)
                    if(left[count_left-1]!=left[count_left-2])
                        match_left = false;
            }
        }
        if(count_left>0) {
            if(match_left/* && c->done(left[0],l+1)*/)
                left_child = new wt_node_leaf(left[0], count_left);
            else
                left_child = new wt_node_internal(left, count_left, l+1, c, bmb);
        }
        else {
            left_child = NULL;
        }
        delete [] left;

        if(count_right>0) {
            if(match_right/* && c->done(right[0],l+1)*/)
                right_child = new wt_node_leaf(right[0], count_right);
            else
                right_child = new wt_node_internal(right, count_right, l+1, c, bmb);
        }
        else {
            right_child = NULL;
        }
        delete [] right;
    }

    wt_node_internal::wt_node_internal(uchar * symbols, size_t n, uint l, wt_coder * c, BitSequenceBuilder * bmb, size_t left, uint *done) {
        uint * ibitmap = new uint[n/W+1];
        for(size_t i=0;i<n/W+1;i++)
            ibitmap[i]=0;
        for(size_t i=0;i<n;i++)
            if(c->is_set((uint)symbols[i + left],l))
                bitset(ibitmap,i);
        bitmap = bmb->build(ibitmap, n);
        delete [] ibitmap;

        size_t count_right = bitmap->rank1(n-1);
        size_t count_left = n-count_right;

        for (size_t i=0;i<n;i++)
            set_field(done, 1, i+left, 0);

        for (size_t i = 0; i < n; ) {
            size_t j = i;
            uchar swap = symbols[j+left];
                                 // swapping
            while (!get_field(done, 1, j+left)) {
                uint k = j;
                if (!c->is_set(swap,l))
                    j = bitmap->rank0(k)-1;
                else
                    j = count_left + bitmap->rank1(k)-1;
                uchar temp = symbols[j+left];
                symbols[j+left] = swap;
                swap = temp;
                set_field(done,1,k+left,1);
            }

            while (get_field(done,1,i+left))
                ++i;
        }

        bool match_left = true, match_right = true;
        for (size_t i=1; i < count_left; i++)
            if (symbols[i+left] != symbols[i+left-1])
                match_left = false;
        for (size_t i=count_left + 1; i < n; i++)
            if (symbols[i+left] != symbols[i+left-1])
                match_right = false;

        if(count_left>0) {
            if(match_left/* && c->done(left[0],l+1)*/)
                left_child = new wt_node_leaf((uint)symbols[left], count_left);
            else
                left_child = new wt_node_internal(symbols, count_left, l+1, c, bmb, left, done);
        }
        else {
            left_child = NULL;
        }
        if(count_right>0) {
            if(match_right/* && c->done(right[0],l+1)*/)
                right_child = new wt_node_leaf((uint)symbols[left+count_left], count_right);
            else
                right_child = new wt_node_internal(symbols, count_right, l+1, c, bmb, left+count_left, done);
        }
        else {
            right_child = NULL;
        }
    }

    wt_node_internal::wt_node_internal() { }

    wt_node_internal::~wt_node_internal() {
        delete bitmap;
        if(right_child!=NULL) delete right_child;
        if(left_child!=NULL) delete left_child;
    }

    size_t wt_node_internal::rank(uint * symbol, size_t pos, uint l, wt_coder *c) const
    {
        bool is_set = c->is_set(symbol,l);
        if(!is_set) {
	  /*if(left_child==NULL) {
                cout << "symbol1=" << symbol << endl;
                return 0;
		}*/
            return left_child->rank(symbol, bitmap->rank0(pos)-1,l+1, c);
        }
        else {
	  /*if(right_child==NULL) {
                cout << "symbol2=" << symbol << endl;
                return 0;
		}*/
            return right_child->rank(symbol, bitmap->rank1(pos)-1,l+1, c);
        }
    }

    size_t wt_node_internal::select(uint * symbol, size_t pos, uint l, wt_coder *c) const
    {
      bool is_set = c->is_set(symbol,l); //c->is_set(symbol, l);
        size_t ret = 0;
        if(!is_set) {
	  /*if(left_child==NULL)
	    return (size_t)(-1);*/
            size_t new_pos = left_child->select(symbol, pos, l+1, c);
            if(new_pos+1==0) return (uint)(-1);
            ret = bitmap->select0(new_pos)+1;
        }
        else {
	  /*if(right_child==NULL)
	    return (size_t)(-1);*/
            size_t new_pos = right_child->select(symbol, pos, l+1, c);
            if(new_pos+1==0) return (uint)(-1);
            ret = bitmap->select1(new_pos)+1;
        }
        if(ret==0) return (size_t)-1;
        return ret;
    }

    uint wt_node_internal::access(size_t pos) const
    {
      size_t rOpt = 0;
      bool is_set = bitmap->access(pos,rOpt);
        if(!is_set) {
            assert(left_child!=NULL);
            return left_child->access(rOpt-1);
        }
        else {
            assert(right_child!=NULL);
            return right_child->access(rOpt-1);
        }
    }

    // Returns the value at given position and its rank
    uint wt_node_internal::access(size_t pos, size_t &rankp) const
    {
      size_t rOpt = 0;
      bool is_set = bitmap->access(pos,rOpt);
        if(!is_set) {
            // recurse left
            return left_child->access(rOpt-1, rankp);
        }
        else {
            // recurse right
            return right_child->access(rOpt-1, rankp);
        }
    }

    pair<uint,size_t> wt_node_internal::quantile_freq(size_t left,size_t right,uint q) const
    {
        /* number of 1s before T[l..r] */
        size_t rank_before_left = bitmap->rank1(left-1);
        /* number of 1s before T[r] */
        size_t rank_before_right = bitmap->rank1(right);
        /* number of 1s in T[l..r] */
        size_t num_ones = rank_before_right - rank_before_left;
        /* number of 0s in T[l..r] */
        size_t num_zeros = (right-left+1) - num_ones;

        if(q >= num_zeros) {
            return right_child->quantile_freq(rank_before_left,rank_before_left+num_ones-1,q-num_zeros);
        } else {
            return left_child->quantile_freq((left-rank_before_left),(left-rank_before_left)+num_zeros-1,q);
        }
    }

    size_t wt_node_internal::getSize() const
    {
        uint s = bitmap->getSize()+sizeof(wt_node_internal);
        if(left_child!=NULL)
            s += left_child->getSize();
        if(right_child!=NULL)
            s += right_child->getSize();
        return s;
    }

    void wt_node_internal::save(ofstream & fp) const
    {
        uint wr = WT_NODE_INTERNAL_HDR;
        saveValue(fp,wr);
        bitmap->save(fp);
        if(left_child!=NULL) {
            left_child->save(fp);
        }
        else {
            wr = WT_NODE_NULL_HDR;
            saveValue(fp,wr);
        }
        if(right_child!=NULL) {
            right_child->save(fp);
        }
        else {
            wr = WT_NODE_NULL_HDR;
            saveValue(fp,wr);
        }
    }

    wt_node_internal * wt_node_internal::load(ifstream & fp) {
        uint rd = loadValue<uint>(fp);
        if(rd!=WT_NODE_INTERNAL_HDR) return NULL;
        wt_node_internal * ret = new wt_node_internal();
        ret->bitmap = BitSequence::load(fp);
        ret->left_child = wt_node::load(fp);
        ret->right_child = wt_node::load(fp);
        return ret;
    }

};
