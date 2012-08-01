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

/* General interface for using the compressed index libraries */
#ifndef _TEXTINDEX_H_
#define _TEXTINDEX_H_

#include <libcdsBasics.h>
using namespace cds_utils;
using namespace std;

#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint
#define uint  unsigned int
#endif
#ifndef ulong
#define ulong unsigned long
#endif

namespace cds_static
{

	static const unsigned int CSA_HDR = 2;

	class TextIndex
	{
		public:
			~TextIndex(){};

			/* Writes in numocc the number of occurrences of the substring
			 * pattern[0..length-1] found in the text indexed by index. */
			virtual size_t count (uchar *pattern, ulong length, ulong *numocc) const=0;

			/* Writes in numocc the number of occurrences of the substring
			 * pattern[0..length-1] in the text indexed by index. It also allocates
			 * occ (which must be freed by the caller) and writes the locations of
			 * the numocc occurrences in occ, in arbitrary order.  */
			virtual size_t locate (uchar *pattern, ulong length, ulong **occ, ulong *numocc) const=0;

			/*  Allocates snippet (which must be freed by the caller) and writes
			 *  the substring text[from..to] into it. Returns in snippet_length the
			 *  length of the text snippet actually extracted (that could be less
			 *  than to-from+1 if to is larger than the text size). */
			virtual size_t extract (ulong from, ulong to, uchar **snippet, ulong *snippet_length) const=0;

			/* Displays the text (snippet) surrounding any occurrence of the
			 * substring pattern[0..length-1] within the text indexed by index.
			 * The snippet must include numc characters before and after the
			 * pattern occurrence, totalizing length+2*numc characters, or less if
			 * the text boundaries are reached. Writes in numocc the number of
			 * occurrences, and allocates the arrays snippet_text and
			 * snippet_lengths (which must be freed by the caller). The first is a
			 * character array of numocc*(length+2*numc) characters, with a new
			 * snippet starting at every multiple of length+2*numc. The second
			 * gives the real length of each of the numocc snippets. */

			virtual size_t display (uchar *pattern, ulong length, ulong numc, ulong *numocc, uchar **snippet_text, ulong **snippet_lengths) const=0;

			/*Return the length of the Index*/
			virtual size_t index_length() const=0;

			/***The next four methods (getSA, getISA, and getPsi, getT) are
			 * only valid for text indexes that are base on Suffix
			 * Array*/

			/*Get the suffix array at position i*/
			virtual size_t getSA(size_t i) const=0;

			/*Get the inverse of the suffix array at position i*/
			virtual size_t getISA(size_t i) const=0;

			/*Get Psi at position i*/
			virtual size_t getPsi(size_t i) const=0;

			/*Get the first letter of the i-th suffix in the suffix array*/
			virtual size_t getT(size_t i) const=0;

			/** Returns the size of the structure in bytes */
			virtual size_t getSize() const=0;

			/** Stores the text index in the  given a file pointer*/
			virtual void save(ofstream & fp) const=0;

			/** Reads a text index determining the type */
			static TextIndex * load(ifstream & fp);
	};
};

#include<TextIndexCSA.h>
#endif
