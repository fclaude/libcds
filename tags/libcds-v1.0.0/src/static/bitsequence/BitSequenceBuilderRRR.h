/*  BitSequenceBuilderRRR.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
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

#ifndef BITSEQUENCEBUILDERRRR_H
#define BITSEQUENCEBUILDERRRR_H

#include <libcdsBasics.h>
#include <BitSequenceRRR.h>
#include <BitSequenceBuilder.h>

using namespace cds_utils;

namespace cds_static
{
    /** BitSequence builder for RRR BitSequences.
     *
     * @author Francisco Claude
     */
    class BitSequenceBuilderRRR : public BitSequenceBuilder
    {
        public:
            BitSequenceBuilderRRR(uint sample_rate);
            virtual ~BitSequenceBuilderRRR() {}
            virtual BitSequence * build(uint * bitseq, size_t len) const;
            virtual BitSequence * build(const BitString & bs) const;
        protected:
            uint sample_rate;
    };
};
#endif
