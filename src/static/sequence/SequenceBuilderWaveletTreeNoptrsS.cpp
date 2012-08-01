/*  SequenceBuilderWaveletTreeNoptrsS.cpp
 * Copyright (C) 2012, Francisco Claude, all rights reserved.
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

#include <SequenceBuilderWaveletTreeNoptrsS.h>

namespace cds_static {

    SequenceBuilderWaveletTreeNoptrsS::SequenceBuilderWaveletTreeNoptrsS(BitSequenceBuilder * bsb, Mapper * am) {
        this->bsb = bsb;
        this->am = am;
        bsb->use();
        am->use();
    }

    SequenceBuilderWaveletTreeNoptrsS::~SequenceBuilderWaveletTreeNoptrsS() {
        bsb->unuse();
        am->unuse();
    }

    Sequence * SequenceBuilderWaveletTreeNoptrsS::build(uint * sequence, size_t len) {
        return new WaveletTreeNoptrsS(sequence, len, bsb, am);
    }
    
    Sequence * SequenceBuilderWaveletTreeNoptrs::build(const Array & seq) {
        return new WaveletTreeNoptrsS(seq, bsb, am);
    }
};


