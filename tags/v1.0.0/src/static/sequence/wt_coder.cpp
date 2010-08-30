/* wt_coder.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * wt_coder definition
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

#include <wt_coder.h>

namespace cds_static
{

    wt_coder::wt_coder() {
        user_count=0;
    }

    void wt_coder::use() {
        user_count++;
    }

    void wt_coder::unuse() {
        user_count--;
        if(user_count==0) delete this;
    }

    wt_coder * wt_coder::load(ifstream & fp) {
        uint rd = loadValue<uint>(fp);
        size_t pos = fp.tellg();
        fp.seekg(pos-sizeof(uint));
        switch(rd) {
            case WT_CODER_HUFF_HDR: return wt_coder_huff::load(fp);
            case WT_CODER_BINARY_HDR: return wt_coder_binary::load(fp);
        }
        return NULL;
    }
};
