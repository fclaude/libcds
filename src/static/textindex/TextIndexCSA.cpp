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

#include <TextIndexCSA.h>

namespace cds_static
{

	TextIndexCSA::TextIndexCSA() {
		csa = NULL;
	}

	TextIndexCSA::TextIndexCSA(uchar *text, ulong length, char *build_options) {
		void *index;
		build_index(text, length, build_options, &index);
		csa = (CSA *) index;
	}

	TextIndexCSA::~TextIndexCSA() {
		csa_free(csa);
		/*remove files *.idx and *.psi*/
		/*system("rm -rf *.psi");
		 * system("rm .rf *.idx");*/
	}

	size_t TextIndexCSA::count(uchar *pattern, ulong length, ulong *numocc) const
	{
		return (size_t)cds_static::count(csa, pattern, length, numocc);
	}

	size_t TextIndexCSA::locate (uchar *pattern, ulong length, ulong **occ, ulong *numocc) const
	{
		return (size_t)cds_static::locate (csa, pattern, length, occ, numocc);
	}

	size_t TextIndexCSA::extract (ulong from, ulong to, uchar **snippet, ulong *snippet_length) const
	{
		return (size_t)cds_static::extract (csa, from, to, snippet, snippet_length);
	}

	size_t TextIndexCSA::display (uchar *pattern, ulong length, ulong numc, ulong *numocc, uchar **snippet_text, ulong **snippet_lengths) const
	{
		return (size_t)cds_static::display(csa, pattern, length, numc, numocc, snippet_text, snippet_lengths);
	}

	size_t TextIndexCSA::index_length() const
	{
		return (size_t)(csa->n);
	}

	size_t TextIndexCSA::getSA(size_t i) const
	{
		return csa_lookup(csa,i+1)-1;
	}

	size_t TextIndexCSA::getISA(size_t i) const
	{
		return csa_inverse(csa,i+1)-1;
	}

	size_t TextIndexCSA::getPsi(size_t i) const
	{
		return csa_psi(csa,i+1)-1;
	}

	size_t TextIndexCSA::getT(size_t i) const
	{
		return csa_T(csa,i+1);
	}

	size_t TextIndexCSA::getSize() const
	{
		ulong size;
		index_size(csa, &size);
		return (size_t)size;
	}

	void TextIndexCSA::save(ofstream & fp) const
	{
		uint wr = CSA_HDR;
		saveValue(fp,wr);
		if(csa!=NULL)
			csa_save(csa,fp);
	}

	TextIndexCSA * TextIndexCSA::load(ifstream & fp) {
		uint type = loadValue<uint>(fp);
		if(type!=CSA_HDR) {
			abort();
		}
		TextIndexCSA *ticsa= new TextIndexCSA();
		ticsa->csa = csa_load(fp);
		return ticsa;
	}

};
