/*  Permutation.h
 * Copyright (C) 2010, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <libcdsBasics.h>

#ifndef __PERMUTATION_H
#define __PERMUTATION_H

using namespace cds_utils;

namespace cds_static {
    class Permutation {
        public:
            Permutation();
            virtual ~Permutation();
            
            virtual uint pi(uint i) const;
            virtual uint revpi(uint i) const;

            virtual uint pi(uint i, uint k) const;
            virtual uint revpi(uint i, uint k) const;

            virtual size_t getLength() const;
            virtual size_t getSize() const = 0;
            virtual void save(ofstream & fp) const;
            
            static Permutation * load(ifstream & fp); 

        protected:
            size_t length;
    };
};

#define MRRRPERM 1

#include<PermutationMRRR.h>

#endif

