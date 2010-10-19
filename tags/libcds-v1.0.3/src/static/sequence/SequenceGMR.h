/* SequenceGMR.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * GMR
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

#ifndef SEQUENCEGMR_H
#define SEQUENCEGMR_H

#include <libcdsBasics.h>
#include <Sequence.h>
#include <BitSequence.h>
#include <SequenceBuilder.h>
#include <BitSequenceBuilder.h>
#include <cassert>
#include <iostream>

using namespace cds_utils;
namespace cds_static {
//using namespace std;

class SequenceGMR : public Sequence {
  public:
      /** Builds the GMR structure.
       * @param sequence Sequence of unsigned ints
       * @param n length of the sequence
       * @param chunk_length length of the regular partitioning (called chunks in the paper)
       * @param bmb builder for the bitmap B (as called in the paper)
       * @param ssb builder for the representation used for each chunk.
       */
    SequenceGMR(uint * sequence, size_t n, uint chunk_length, BitSequenceBuilder * bmb, SequenceBuilder * ssb);

    ~SequenceGMR();
    virtual size_t rank(uint c, size_t j) const;
    virtual size_t select(uint c, size_t j) const;
    virtual uint access(size_t j) const;
    virtual size_t getSize() const;
    virtual void save(ofstream & fp) const;
    /** Loads the SequenceGMR object from the stream. 
     */
    static SequenceGMR * load(ifstream & fp);

  protected:
    SequenceGMR();
    void build(uint * sequence, BitSequenceBuilder * bmb, SequenceBuilder * ssb);
    uint * get_ones(uint * sequence);

    uint sigma, chunk_length;
    Sequence ** chunk;
    BitSequence * B;
};

};

#endif
