/* wt_node_internal.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
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

#ifndef wt_node_internal_h
#define wt_node_internal_h

#include <wt_node.h>
#include <libcdsBasics.h>
#include <BitSequence.h>
#include <BitSequenceBuilder.h>
#include <cassert>

namespace cds_static
{

    /** Class for representing internal nodes
     *
     *  @author Francisco Claude
     */
    class wt_node_internal: public wt_node
    {
        public:
            wt_node_internal(uint * seq, size_t n, uint l, wt_coder * c, BitSequenceBuilder * bmb);
            wt_node_internal(uchar * seq, size_t n, uint l, wt_coder * c, BitSequenceBuilder * bmb, size_t left, uint * done);
            virtual ~wt_node_internal();
            virtual size_t rank(uint symbol, size_t pos, uint level, wt_coder * c) const;
            //virtual size_t rankLessThan(uint &symbol, size_t pos) const;
            virtual size_t select(uint symbol, size_t pos, uint level, wt_coder * c) const;
            virtual uint access(size_t pos) const;
            virtual uint access(size_t pos, size_t & rankp) const;
            virtual size_t getSize() const;
            virtual void save(ofstream & fp) const;
            static wt_node_internal * load(ifstream & fp);

        protected:
            wt_node_internal();
            wt_node *left_child, *right_child;
            BitSequence * bitmap;
            //uint length;
    };
};
#endif
