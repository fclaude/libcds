/* huff.h
   Copyright (C) 2008, Gonzalo Navarro, all rights reserved.

   Canonical Huffman

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

#ifndef HUFFINCLUDED
#define HUFFINCLUDED

#include <libcdsBasics.h>
#include <cppUtils.h>

using namespace cds_utils;

namespace cds_static
{

	typedef struct
	{							 // maximum symbol (0..max), same excluding zero freqs
		uint max,lim;
		uint depth;				 // max symbol length
		union
		{						 // symbol positions after sorting by decr freq (enc)
			uint *spos;
			uint *symb;			 // symbols sorted by freq (dec)
		} s;
		uint *num;				 // first pos of each length (dec), number of each length (enc)
		uint *fst;				 // first code (numeric) of each length (dec)
		unsigned long long total;// total length to achieve, in bits
	} THuff;

	/** Creates Huffman encoder given symbols 0..lim with frequencies
	 *  freq[i], ready for compression
	 *
	 *  @author Gonzalo Navarro
	 */
	THuff createHuff (uint *freq, uint lim);

	/** Encodes symb using H, over stream[ptr...lim] (ptr and lim are
	 *  bit positions of stream). Returns the new ptr.
	 *
	 *  @author Gonzalo Navarro
	 */
	size_t encodeHuff (const THuff H, uint symb, uint *stream, size_t ptr);

	/** Decodes *symb using H, over stream[ptr...lim] (ptr and lim are
	 *  bit positions of stream). Returns the new ptr.
	 *
	 *  @author Gonzalo Navarro
	 */
	size_t decodeHuff (const THuff H, uint *symb, uint *stream, size_t ptr);

	/** Writes H in file f
	 *
	 *  @author Gonzalo Navarro
	 */
	void saveHuff (const THuff H, ofstream & f);

	/** Size of H written on file
	 *
	 *  @author Gonzalo Navarro
	 */
	uint sizeHuff (const THuff H);

	/** Frees H
	 *
	 *  @author Gonzalo Navarro
	 */
	void freeHuff (THuff H);

	/** Loads H from file f, prepared for encoding or decoding depending
	 *  on enc
	 *
	 *  @author Gonzalo Navarro
	 */
	THuff loadHuff (ifstream &f, int enc);

};
#endif
