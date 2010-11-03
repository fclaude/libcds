/* BitSequenceRRR.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * RRR02 Bitsequence -
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

#ifndef _STATIC_BITSEQUENCE_RRR02_H
#define _STATIC_BITSEQUENCE_RRR02_H

#include <libcdsBasics.h>
#include <BitString.h>
#include <BitSequence.h>
#include <TableOffsetRRR.h>

using namespace cds_utils;

namespace cds_static
{

    // block size can't be changed in this implementation
    // it would require more than just changing the constant
    #define BLOCK_SIZE 15

    #define DEFAULT_SAMPLING 32

    /** Implementation of Raman, Raman and Rao's [1] proposal for rank/select capable
     *  data structures, it achieves space nH_0, O(sample_rate) time for rank and O(log len)
     *  for select. The practial implementation is based on [2]
     *
     *  [1] R. Raman, V. Raman and S. Rao. Succinct indexable dictionaries with applications
     *     to encoding $k$-ary trees and multisets. SODA02.
     *  [2] F. Claude and G. Navarro. Practical Rank/Select over Arbitrary Sequences. SPIRE08.
     *
     *  @author Francisco Claude
     */
    class BitSequenceRRR : public BitSequence
    {
        public:
            /** Builds an RRR bitsequence.
             * @param bitseq bitstring
             * @param len lenght of the bitstring
             * @param sample_rate sampling for partial sums
             */
            BitSequenceRRR(uint * bitseq, size_t len, uint sample_rate=DEFAULT_SAMPLING);

            /** Builds an RRR bitsequence.
             * @param bs bitstring
             * @param sample_rate sampling for partial sums
             */
            BitSequenceRRR(const BitString & bs, uint sample_rate=DEFAULT_SAMPLING);

            virtual ~BitSequenceRRR();

            virtual size_t rank0(const size_t i) const;

            virtual size_t rank1(const size_t i) const;

            virtual size_t select0(const size_t i) const;

            virtual size_t select1(const size_t i) const;

            virtual bool access(const size_t i) const;

            virtual size_t getSize() const;

            virtual void save(ofstream & fp) const;

            /** Reads the bitmap from a file pointer, returns NULL in case of error */
            static BitSequenceRRR * load(ifstream & fp);

            /** Creates a new sampling for the queries */
            void create_sampling(uint sampling_rate);

            /** Frees the space required by the table E, which is static and global
             *  to all instances.
             */
            static void delete_E() {
                delete E;
            }

        protected:
            /** Internal building function, same parameters as the base constructor. */
            void build(const uint * bitseq, size_t len, uint sample_rate=DEFAULT_SAMPLING);

            /** Protected constructor for loaders, you have to initialize data before
             * using an object built with this constructor.
             */
            BitSequenceRRR();

            /** Classes and offsets */
            uint *C, *O;
            /** Length of C and O (in uints) */
            uint C_len, O_len;
            /** Bits required per field for C and in total for O */
            uint C_field_bits, O_bits_len;
            /** C and O samplings */
            uint *C_sampling, *O_pos;
            /** Length of the samplings */
            uint C_sampling_len,O_pos_len;
            /** Lenght in bits per field */
            uint C_sampling_field_bits,O_pos_field_bits;
            /** Sample rate */
            uint sample_rate;

            static table_offset * E;
    };

}
#endif                           /* _STATIC_BITSEQUENCE_RRR02_H */
