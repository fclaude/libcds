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

#ifndef _TEXTINDEXCSA_H_
#define _TEXTINDEXCSA_H_

#include <TextIndex.h>
#include <interface.h>
#include <comparray4.h>

namespace cds_static
{

	class TextIndexCSA: public TextIndex
	{
		private:
			CSA *csa;
			TextIndexCSA();

		public:

			/*K. Sadakane's CSA (refactory by Rodrigo Cnovas)
			 * @param text The text.
			 * @param length Length of the text.
			 * @param build_options The options could be:
			 * 												free_text: The text will be freed immediately after using it.
			 * 												samplerate=<number>: samplerate will be interval between two indices
			 * 												                     of the suffix array stored explicitly. That is,
			 * 												                     SA[i*samplerate] is stored explicitly. The default
			 * 												                     value is 16.
			 * 												samplepsi=<number>: samplepsi will be interval between two indices of the
			 * 																						psi function stored explicitly. That is, Psi[i*samplepsi]
			 * 																						is stored explicitly. The default value is 128.
			 **/
			TextIndexCSA(uchar *text, ulong length, char *build_options);

			~TextIndexCSA();

			/* Writes in numocc the number of occurrences of the substring
			 * pattern[0..length-1] found in the text indexed by index. */
			virtual size_t count (uchar *pattern, ulong length, ulong *numocc) const;

			/* Writes in numocc the number of occurrences of the substring
			 * pattern[0..length-1] in the text indexed by index. It also allocates
			 * occ (which must be freed by the caller) and writes the locations of
			 * the numocc occurrences in occ, in arbitrary order.  */
			virtual size_t locate (uchar *pattern, ulong length, ulong **occ, ulong *numocc) const;

			/*  Allocates snippet (which must be freed by the caller) and writes
			 *  the substring text[from..to] into it. Returns in snippet_length the
			 *  length of the text snippet actually extracted (that could be less
			 *  than to-from+1 if to is larger than the text size). */
			virtual size_t extract (ulong from, ulong to, uchar **snippet, ulong *snippet_length) const;

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

			virtual size_t display (uchar *pattern, ulong length, ulong numc, ulong *numocc, uchar **snippet_text, ulong **snippet_lengths) const;

			/*Return the length of the Index*/
			virtual size_t index_length() const;

			/*Get the suffix array at position i*/
			virtual size_t getSA(size_t i) const;

			/*Get the inverse of the suffix array at position i*/
			virtual size_t getISA(size_t i) const;

			/*Get Psi at position i*/
			virtual size_t getPsi(size_t i) const;

			/*Get the first letter of the i-th suffix in the suffix array*/
			virtual size_t getT(size_t i) const;

			/** Returns the size of the structure in bytes */
			virtual size_t getSize() const;

			/** Stores the text index in the  given a file pointer*/
			virtual void save(ofstream & fp) const;

			/** Reads a text index determining the type */
			static TextIndexCSA * load(ifstream & fp);

	};
};
#endif
