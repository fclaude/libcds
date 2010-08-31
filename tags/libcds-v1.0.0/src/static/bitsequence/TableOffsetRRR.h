/* table_offset.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * Table for offsets definition.
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

#ifndef _TABLE_OFFSET_H
#define _TABLE_OFFSET_H

#include <libcdsBasics.h>
#include <iostream>

using namespace cds_utils;

namespace cds_static
{
    /** Universal table required for BitSequenceRRR, Raman, Raman and Rao's [1]
     *  proposal for rank/select capable data structures, it achieves space nH_0,
     *  O(sample_rate) time for rank and O(log len) for select. The practial implementation
     *  is based on [2]
     *
     *  [1] R. Raman, V. Raman and S. Rao. Succinct indexable dictionaries with applications
     *     to encoding $k$-ary trees and multisets. SODA02.
     *  [2] F. Claude and G. Navarro. Practical Rank/Select over Arbitrary Sequences. SPIRE08.
     *
     *  @author Francisco Claude
     */
    class table_offset
    {

        public:
            /** builds a universal table, designed for u<=15 */
            table_offset(uint u);
            ~table_offset();

            /** Increments the counter of users for the table */
            inline void use() {
                users_count++;
            }

            /** Tells the object that the user is not going to need the table anymore. */
            inline table_offset * unuse() {
                users_count--;
                if(!users_count) {
                    delete this;
                    return NULL;
                }
                return this;
            }

            /** Computes binomial(n,k) for n,k<=u */
            inline uint get_binomial(uint n, uint k) {
                return binomial[n][k];
            }

            /** Computes ceil(log2(binomial(n,k))) for n,k<=u */
            inline ushort get_log2binomial(uint n, uint k) {
                return log2binomial[n][k];
            }

            /** Returns the bitmap represented by the given class and inclass offsets */
            inline ushort short_bitmap(uint class_offset, uint inclass_offset) {
                if(class_offset==0) return 0;
                if(class_offset==u) return (ushort)(((uint)1<<u)-1);
                return short_bitmaps[offset_class[class_offset]+inclass_offset];
            }

            /** Returns u */
            inline uint get_u() {
                return u;
            }

            /** Computes the offset of the first u bits of a given bitstring */
            inline ushort compute_offset(ushort v) {
                return rev_offset[v];
            }

            /** Returns the size of the bitmap in bytes */
            uint size();

        protected:
            int users_count;
            uint u;
            uint ** binomial;
            ushort * rev_offset;
            ushort ** log2binomial;
            ushort * offset_class;
            ushort * short_bitmaps;

            void fill_tables();
    };

};
#endif
