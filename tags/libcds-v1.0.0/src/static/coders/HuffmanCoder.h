/* HuffmanCoder.h
   Copyright (C) 2008, Francisco Claude, all rights reserved.

   Wrapper for huff written by Gonzalo Navarro

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

#ifndef HUFFMAN_CODES_H
#define HUFFMAN_CODES_H

#include <libcdsBasics.h>
#include <huff.h>
#include <Array.h>
#include <Coder.h>

using namespace cds_utils;
namespace cds_static
{

    #define HUFF_HDR 1

    /** Wrapper for the canonical huffman implementation of Gonzalo Navarro.
     *
     *  @author Francisco Claude
     */
    class HuffmanCoder : public Coder
    {

        public:
            /** Creates the codes for the sequence seq of length n */
            HuffmanCoder(uint * seq, size_t n);
            HuffmanCoder(uchar * seq, size_t n);
            HuffmanCoder(Array & seq);

            virtual ~HuffmanCoder();

            /** Encodes symb into stream at bit-position pos,
             * returns the ending position (bits) */
            virtual size_t encode(uint symb, uint * stream, size_t pos) const;

            /** decodes into symb from stream at bit-position
             * pos, returns the new position */
            virtual size_t decode(uint * symb, uint * stream, size_t pos) const;

            /** Returns the maximum length of a code */
            virtual size_t maxLength() const;

            /** Returns the size of the table */
            virtual size_t getSize() const;

            /** Saves the coder to a file */
            virtual void save(ofstream & fp) const;

            /** Loads a coder from a file */
            static HuffmanCoder * load(ifstream & fp);

        protected:
            HuffmanCoder();
            THuff huff_table;
    };

};
#endif
