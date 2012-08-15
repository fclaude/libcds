/* wt_node_leaf.cpp
 * Copyright (C) 2008, Francisco Claude.
 * Copyright (C) 2011, Matthias Petri.
 *
 * wt_node_leaf
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

#include <wt_node_leaf.h>

namespace cds_static
{

    wt_node_leaf::wt_node_leaf(uint symbol, size_t count) {
        this->symbol = symbol;
        this->count = count;
    }

    wt_node_leaf::wt_node_leaf() {}

    wt_node_leaf::~wt_node_leaf() {}

    size_t wt_node_leaf::rank(uint *symbol, size_t pos, uint l, wt_coder *c) const
    {
      /*if(symbol!=this->symbol) {
            return 0;
	    }*/
      //pos++;
        return pos+1;
    }

    size_t wt_node_leaf::select(uint *symbol, size_t pos, uint l, wt_coder *c) const
    {
      //if(symbol!=this->symbol) return (size_t)-1;
        if(pos==0 || pos>count) return (size_t)-1;
        return pos;
    }

    uint wt_node_leaf::access(size_t pos) const
    {
        return symbol;
    }

    uint wt_node_leaf::access(size_t pos, size_t &rank) const
    {
        rank = pos+1;
        return symbol;
    }

    pair<uint,size_t> wt_node_leaf::quantile_freq(size_t left,size_t right,uint q) const
    {
        return std::make_pair(symbol,right-left+1);
    }

    size_t wt_node_leaf::getSize() const
    {
        return sizeof(wt_node_leaf);
    }

    void wt_node_leaf::save(ofstream & fp) const
    {
        uint wr = WT_NODE_LEAF_HDR;
        saveValue(fp,wr);
        saveValue(fp,count);
        saveValue(fp,symbol);
    }

    wt_node_leaf * wt_node_leaf::load(ifstream & fp) {
        uint rd = loadValue<uint>(fp);
        if(rd!=WT_NODE_LEAF_HDR) return NULL;
        wt_node_leaf * ret = new wt_node_leaf();
        ret->count = loadValue<size_t>(fp);
        ret->symbol = loadValue<uint>(fp);
        return ret;
    }
};
