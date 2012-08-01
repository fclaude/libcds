/* wt_node_leaf.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
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

#ifndef wt_node_leaf_h
#define wt_node_leaf_h

#include <wt_node.h>
#include <libcdsBasics.h>
#include <wt_coder.h>
#include <cassert>
#include <vector>

namespace cds_static
{

    /** Class for representing leaves of the wavelet tree.
     *
     *  @author Francisco Claude
     */
    class wt_node_leaf: public wt_node
    {
        public:
            wt_node_leaf(uint symbol, size_t count);
            virtual ~wt_node_leaf();
            virtual size_t rank(uint *symbol, size_t pos, uint l, wt_coder *c) const;
            virtual size_t select(uint *symbol, size_t pos, uint l, wt_coder *c) const;
            virtual pair<uint,size_t> quantile_freq(size_t left,size_t right,uint q) const;
            virtual uint access(size_t pos) const;
            virtual uint access(size_t pos, size_t &rank) const;
            virtual size_t getSize() const;
            virtual void save(ofstream & fp) const;
            static wt_node_leaf * load(ifstream & fp);

        protected:
            wt_node_leaf();
            uint symbol;
            size_t count;
    };
};
#endif
