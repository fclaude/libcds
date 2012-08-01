/* Copyright (C) 2010, Rodrigo Cnovas, all rights reserved.
 *
 *This library is free software; you can redistribute it and/or
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

#ifndef _LCP_H
#define _LCP_H

#include <iostream>
#include <libcdsTrees.h>
#include <TextIndex.h>

using namespace std;
using namespace cds_utils;

namespace cds_static
{

	static const size_t NAIVE = 2;
	static const size_t SAD_GON_OS = 3;
	static const size_t FMN_RRR_OS = 4;
	static const size_t PT = 5;
	static const size_t PHI = 6;
	static const size_t DAC = 7;
	static const size_t DAC_VAR = 8;

	/** Base class for LCP, contains many abstract functions, so this can't
	 * be instantiated.
	 * @author Rodrigo Cnovas
	 */
	class LCP
	{
		public:
			size_t lcp_type;

			virtual ~LCP() {};

			/**Return LCP[i], being faster if sequential acceses to the LCP had been done*/
			virtual size_t get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const=0;

			/**Return LCP[i]*/
			virtual size_t get_LCP(size_t i, TextIndex *csa) const= 0;

			/** Returns the size of the structure in bytes */
			virtual size_t getSize() const= 0;

			/**compute the LCP array, using Karkkainen, Manzini, and Puglisi (2009) Algorithm
			 * @param csa The Compressed Suffix Array
			 * @param text The text
			 * @param n The length of the text
			 * @param q Sample interval*/
			virtual  uint * create_lcp(TextIndex *csa, char *text, uint n, int q) const;

			/** Stores the structure given a file pointer*/
			virtual void save(ofstream & fp) const=0;

			/** Reads a LCP determining the type */
			static LCP * load(ifstream & fp);
	};
};

#include <LCP_naive.h>
#include <LCP_Sad.h>
#include <LCP_FMN.h>
#include <LCP_PT.h>
#include <LCP_PhiSpare.h>
#include <LCP_DAC.h>
#include <LCP_DAC_VAR.h>
#endif
