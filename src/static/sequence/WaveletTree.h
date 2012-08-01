/* WaveletTree.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * WaveletTree definition
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

#ifndef STATIC_SEQUENCE_WVTREE_H
#define STATIC_SEQUENCE_WVTREE_H

#include <iostream>
#include <cassert>
#include <libcdsBasics.h>
#include <BitSequence.h>
#include <BitSequenceBuilder.h>
#include <wt_node_internal.h>
#include <wt_coder_binary.h>
#include <Mapper.h>
#include <Sequence.h>

using namespace std;

namespace cds_static
{

	/** Wavelet tree implementation using pointers.
	 *
	 *  @author Francisco Claude
	 */
	class WaveletTree : public Sequence
	{
		public:

			/** Builds a Wavelet Tree for the string stored in a
			 *
			 * @param coder corresponds to the coder used to give the shape to the tree.
			 * @param bmb builder for the bitmaps in each node.
			 * @param am mapper for the alphabet.
			 * */
			WaveletTree(const Array & a, wt_coder * coder, BitSequenceBuilder *bmb, Mapper *am);
			/** Builds a Wavelet Tree for the string
			 * pointed by symbols assuming its length
			 * equals n
			 * @param coder corresponds to the coder used to give the shape to the tree.
			 * @param bmb builder for the bitmaps in each node.
			 * @param am mapper for the alphabet.
			 * */
			WaveletTree(uint * symbols, size_t n, wt_coder * coder, BitSequenceBuilder * bmb, Mapper * am, bool free=false);

			/** Builds a Wavelet Tree for the string
			 * pointed by symbols assuming its length
			 * equals n
			 * @param coder corresponds to the coder used to give the shape to the tree.
			 * @param bmb builder for the bitmaps in each node.
			 * @param am mapper for the alphabet.
			 * */
			WaveletTree(uchar * symbols, size_t n, wt_coder * coder, BitSequenceBuilder * bmb, Mapper * am, bool free=false);

			virtual ~WaveletTree();

			virtual size_t rank(uint symbol, size_t pos) const;

			virtual size_t select(uint symbol, size_t j) const;

			virtual uint access(size_t pos) const;
			virtual uint access(size_t pos, size_t &rank) const;

			/* find the q-th smallest element in T[l..r] */
			uint quantile(size_t left,size_t right,uint q) const;
			/* find the q-th smallest element in T[l..r] and return it's freq */
			pair<uint,size_t> quantile_freq(size_t left,size_t right,uint q) const;

			virtual size_t count(uint s) const;

			virtual size_t getSize() const;

			virtual void save(ofstream & fp) const;
			static WaveletTree * load(ifstream & fp);

		protected:

			WaveletTree();

			wt_node * root;
			wt_coder * c;
			Mapper * am;

			/** Length of the string. */
			size_t n;

			/** Height of the Wavelet Tree. */
			uint max_v;

			/** Flag for testing for correcteness. */
			bool test;

	};

};
#endif							 /* _STATIC_SEQUENCE_WVTREE_H */
