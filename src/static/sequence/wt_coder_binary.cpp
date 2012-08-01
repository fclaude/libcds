/* wt_coder_binary.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * wt_coder_binary definition
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

#include <wt_coder_binary.h>

namespace cds_static
{

    wt_coder_binary::wt_coder_binary(const Array & a, Mapper *am) {
        //am->use();
        uint maxv = 0;
        for(size_t i=0;i<a.getLength();i++)
            maxv = max(maxv,a[i]);
        h = bits(maxv);
        //am->unuse();
    }

    wt_coder_binary::wt_coder_binary(uint * seq, size_t n, Mapper * am) {
        uint max_v = 0;
        for(uint i=0;i<n;i++)
            max_v = max(am->map(seq[i]),max_v);
        h=bits(max_v);
    }

    wt_coder_binary::wt_coder_binary(uchar * seq, size_t n, Mapper * am) {
        uint max_v = 0;
        for(uint i=0;i<n;i++)
            max_v = max(am->map((uint)seq[i]),max_v);
        h=bits(max_v);
    }

    wt_coder_binary::wt_coder_binary() {}

    wt_coder_binary::~wt_coder_binary() {}

    bool wt_coder_binary::is_set(uint symbol, uint l) const
    {
        if((1<<(h-l-1))&symbol) return true;
        return false;
    }

    bool wt_coder_binary::is_set(uint *symbol, uint l) const {
        if(bitget(symbol, h - l - 1)) return true;
        return false;
    }

  uint * wt_coder_binary::get_symbol(uint symbol) const {
    uint * ret = new uint[1];
    *ret = symbol;
    return ret;
  }

    bool wt_coder_binary::done(uint symbol, uint l) const
    {
        if(l==h) return true;
        return false;
    }

    size_t wt_coder_binary::getSize() const
    {
        return sizeof(wt_coder_binary);
    }

    void wt_coder_binary::save(ofstream & fp) const
    {
        uint wr = WT_CODER_BINARY_HDR;
        saveValue(fp,wr);
        saveValue(fp,h);
    }

    wt_coder_binary * wt_coder_binary::load(ifstream & fp) {
        uint rd = loadValue<uint>(fp);
        if(rd!=WT_CODER_BINARY_HDR) return NULL;
        wt_coder_binary * ret = new wt_coder_binary();
        ret->h = loadValue<uint>(fp);
        return ret;
    }
};
