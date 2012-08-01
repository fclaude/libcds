/* wt_node.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * wt_node
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

#ifndef wt_node_h
#define wt_node_h

#include <libcdsBasics.h>
#include <wt_coder.h>
#include <vector>

using namespace cds_utils;

namespace cds_static
{

    #define WT_NODE_NULL_HDR 0
    #define WT_NODE_INTERNAL_HDR 2
    #define WT_NODE_LEAF_HDR 3

    /** Base clase for nodes in the wavelet tree
     *
     *  @author Francisco Claude
     */
    class wt_node
    {
        public:
            virtual ~wt_node() {}
            virtual size_t rank(uint *symbol, size_t pos, uint l, wt_coder *c) const = 0;
            virtual size_t select(uint *symbol, size_t pos, uint l, wt_coder *c) const = 0;
            virtual pair<uint,size_t> quantile_freq(size_t left,size_t right,uint q) const = 0;
            virtual uint access(size_t pos) const = 0;
            virtual uint access(size_t pos, size_t & rankp) const = 0;
            virtual size_t getSize() const = 0;
            virtual void save(ofstream & fp) const = 0;
            static wt_node * load(ifstream & fp);
    };

};

#include <wt_node_internal.h>
#include <wt_node_leaf.h>
#endif
