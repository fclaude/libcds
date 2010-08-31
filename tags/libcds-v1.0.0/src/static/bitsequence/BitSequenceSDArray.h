/* BitSequenceSDArray.h
 * Copyright (C) 2009, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This class is a wrapper for the file sdarraySadakane,
 * implemented by K. Sadakane.
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

#ifndef _BITSEQUENCESDARRAY_H
#define _BITSEQUENCESDARRAY_H

#include <libcdsBasics.h>
#include <BitSequence.h>
#include <sdarraySadakane.h>

#include <BitString.h>
using namespace cds_utils;

namespace cds_static
{
    class BitSequenceSDArray: public BitSequence
    {

        public:
            BitSequenceSDArray(const BitString & bs);
            BitSequenceSDArray(uint * buff, size_t len);
            virtual ~BitSequenceSDArray();
            virtual size_t select1(size_t i) const;
            virtual size_t rank1(size_t i) const;
            virtual size_t selectNext1(size_t i) const;
            virtual size_t getSize() const;
            virtual void save(ofstream & fp) const;
            static BitSequenceSDArray * load(ifstream & fp);

        protected:
            selects3 sd;
            BitSequenceSDArray();

    };
};
#endif
