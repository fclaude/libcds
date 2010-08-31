/* BitSequenceRG.h
   Copyright (C) 2005, Rodrigo Gonzalez, all rights reserved.

   New RANK, SELECT, SELECT-NEXT and SPARSE RANK implementations.
   Adaptation to libcds by Francisco Claude

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef _STATIC_BITSEQUENCE_BRW32_H
#define _STATIC_BITSEQUENCE_BRW32_H

#include <libcdsBasics.h>
#include <BitString.h>
#include <BitSequence.h>

namespace cds_static
{

    /////////////
    //Rank(B,i)//
    /////////////
    //_factor = 0  => s=W*lgn
    //_factor = P  => s=W*P
    //Is interesting to notice
    //factor=2 => overhead 50%
    //factor=3 => overhead 33%
    //factor=4 => overhead 25%
    //factor=20=> overhead 5%

    /** Implementation of Rodrigo Gonzalez et al. practical rank/select solution [1].
     *  The interface was adapted.
     *
     *  [1] Rodrigo Gonzalez, Szymon Grabowski, Veli Makinen, and Gonzalo Navarro.
     *      Practical Implementation of Rank and Select Queries. WEA05.
     *
     *  @author Rodrigo Gonzalez
     */
    class BitSequenceRG : public BitSequence
    {
        private:
            uint *data;
            //bool owner;
            size_t n,integers;
            size_t factor,b,s;
            uint *Rs;            //superblock array

                                 //uso interno para contruir el indice rank
            size_t BuildRankSub(size_t ini,size_t fin);
            void BuildRank();    //crea indice para rank
            BitSequenceRG();
            size_t SpaceRequirementInBits() const;
            size_t SpaceRequirement() const;

        public:
            BitSequenceRG(uint *bitarray, size_t n, uint factor);
            BitSequenceRG(const BitString & bs, uint factor);
            ~BitSequenceRG();    //destructor
            virtual bool access(const size_t i) const;
                                 //Nivel 1 bin, nivel 2 sec-pop y nivel 3 sec-bit
            virtual size_t rank1(const size_t i) const;

                                 // gives the largest index i<=start such that IsBitSet(i)=true
            virtual size_t selectPrev1(const size_t start) const;
                                 // gives the smallest index i>=start such that IsBitSet(i)=true
            virtual size_t selectNext1(const size_t start) const;
                                 // gives the position of the x:th 1.
            virtual size_t select0(size_t x) const;
                                 // gives the position of the x:th 1.
            virtual size_t select1(size_t x) const;
            virtual size_t getSize() const;

            /*load-save functions*/
            virtual void save(ofstream & f) const;
            static BitSequenceRG * load(ifstream & f);
    };

}
#endif
