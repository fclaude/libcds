/* PermutationMRRR.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * Permutation
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

#ifndef _STATIC_PERMUTATION_MRRR_H
#define _STATIC_PERMUTATION_MRRR_H

#include <libcdsBasics.h>
#include <Permutation.h>
#include <perm.h>

using namespace cds_utils;

namespace cds_static {

/** Wrapper for Diego Arroyuelo's implementation of Munro et al.'s permutations.
 *  @author Francisco Claude
 */
class PermutationMRRR : public Permutation {
  public:
    PermutationMRRR(uint * elems, uint nelems, uint t, BitSequenceBuilder * bmb);
    virtual ~PermutationMRRR();
    /** Computes the i-th element of the permutation */
    virtual uint pi(uint i) const;
    /** Computes the inverse of i */
    virtual uint revpi(uint i) const;
    /** Saves the permutation to fp, returns 0 in case of success */
    virtual void save(ofstream & fp) const;
    /** Returns the size of the permutation */
    virtual size_t getSize() const;
    /** Loads a static_permutation from fp */
    static PermutationMRRR * load(ifstream & fp);
  protected:
    perm permutation;
    PermutationMRRR();
};

};

#endif
