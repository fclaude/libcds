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

#include <LCP_DAC_VAR.h>

namespace cds_static
{

	/*LCP using Directly Addressable Variable-Length Codes variable******************************************/
	LCP_DAC_VAR::LCP_DAC_VAR() {
		lcp_type = DAC_VAR;
		next_p_var = NULL;
		rep = NULL;
		n = 0;
	}

	LCP_DAC_VAR::LCP_DAC_VAR(TextIndex *csa, char *text, size_t length) {
		lcp_type = DAC_VAR;
		n = length;
		uint *lcp;
		uint *frec_acu = new uint[n];
		uint max_lcp = 0;
		ushort *kvalues;
		uint nkvalues;
		/*create the LCP array and frec_acu array*/
		lcp = create_lcp(csa,text,n,32);
		for(uint i=0; i<n; i++)
			frec_acu[i]=0;

		frec_acu[0]++;
		for(uint i=0; i < n; i++) {
			frec_acu[lcp[i]]++;
			if(max_lcp<lcp[i])
				max_lcp = lcp[i];
		}
		for(uint i=1; i<=max_lcp; i++)
			frec_acu[i] = frec_acu[i] + frec_acu[i-1];

		/*calculate the kvalues*/
		kvalues = NULL;
		calc_kvalues(max_lcp, frec_acu, &kvalues, &nkvalues);
		rep = new factorization_var(lcp, n, kvalues, nkvalues);
		next_p_var = new size_t[(uint)rep->nLevels -1];
		delete [] kvalues;
		delete [] frec_acu;
		delete [] lcp;
	}

	void LCP_DAC_VAR::calc_kvalues(uint max_lcp, uint *frec_acu, ushort **kvalues, uint *nkvalues) const
	{
		uint list_length=0;
		uint nBits;

		/*calculate the kvalues*/
		list_length = frec_acu[max_lcp];
		nBits = bits(max_lcp);
		//This table will contain the size of the best option for store the firts x bits
		long * tableSize = new long[nBits+1];
		uint * tableNLevels = new uint[nBits+1];
		uint ** tableKvalues = new uint*[nBits+1];
		uint j;
		ulong maxSize=0, maxPos=0;
		uint posVocInf, posVocSup;
		ulong currentSize;
		tableSize[0]=0;
		tableNLevels[0]=0;
		tableKvalues[0]=NULL;

		for(uint i=1;i<=nBits;i++) {
			maxSize=-1;
			maxPos=0;
			for(j=0;j<i;j++) {
				if(i==nBits)
					posVocInf=0;
				else
					posVocInf=1<<(nBits-i);
				posVocSup=(1<<(nBits-j));
				if(posVocSup>= max_lcp)
					posVocSup= max_lcp;
				if(j==0)
					currentSize = tableSize[j]+ ((ulong)(frec_acu[max_lcp]-frec_acu[posVocInf]))*((i-j));
				else
					currentSize = tableSize[j]+ ((ulong)(frec_acu[max_lcp]-frec_acu[posVocInf]))*((i-j)+1)+(frec_acu[max_lcp]-frec_acu[posVocInf])/FACT_RANK;
				if(maxSize>currentSize) {
					maxSize=currentSize;
					maxPos=j;
				}
			}
			tableSize[i]=maxSize;
			tableNLevels[i]=tableNLevels[maxPos]+1;
			tableKvalues[i]=new uint[tableNLevels[i]];
			for(j=0;j<tableNLevels[i]-1;j++)
				tableKvalues[i][j]=tableKvalues[maxPos][j];
			tableKvalues[i][tableNLevels[i]-1]=i-maxPos;
		}

		ulong sumaTotal=0;
		int bitCountInf=0, bitCountSup=0, bitsCount;
		for(j=0;j<tableNLevels[nBits];j++) {
			bitsCount =tableKvalues[nBits][tableNLevels[nBits]-1-j];
			bitCountSup+=bitsCount;
			if(bitCountInf==0)
				posVocInf=0;
			else
				posVocInf=1<<bitCountInf;
			posVocSup=(1<<bitCountSup);
			if(posVocSup>=max_lcp)
				posVocSup=max_lcp;
			if(j==tableNLevels[nBits])
				sumaTotal +=  ((ulong)(frec_acu[max_lcp]-frec_acu[posVocInf]))*bitsCount;
			else
				sumaTotal += ((ulong)(frec_acu[max_lcp]-frec_acu[posVocInf]))*(bitsCount+1)+(frec_acu[max_lcp]-frec_acu[posVocInf])/FACT_RANK;
			bitCountInf+=bitsCount;
		}

		*nkvalues = tableNLevels[nBits];
		ushort *_kvalues = new ushort[*nkvalues];
		cout << endl;
		for(j=0;j<tableNLevels[nBits];j++) {
			bitsCount =tableKvalues[nBits][tableNLevels[nBits]-1-j];
			_kvalues[j] = bitsCount;
			//cout << "kvalue[" << j << "] : " << _kvalues[j] << endl;
			bitCountInf+=bitsCount;
		}

		*kvalues = _kvalues;

		delete [] tableSize;
		delete [] tableNLevels;
		for(uint i=0; i<nBits+1; i++)
			delete [] tableKvalues[i];
		delete [] tableKvalues;
	}

	size_t LCP_DAC_VAR::get_LCP(size_t i, TextIndex *csa) const
	{
		return (size_t)rep->access(i+1);
	}

	size_t LCP_DAC_VAR::get_seq_LCP(size_t i, TextIndex *csa, size_t **next_pos, size_t *n_next, bool dir) const
	{
		if(*n_next==0) {
			*n_next = (uint)rep->nLevels -1;
			for(uint j=0; j< *n_next; j++)
				next_p_var[j] =0;
			*next_pos = next_p_var;
		}
		return (size_t)rep->access_seq(i+1, *next_pos, dir);
	}

	size_t LCP_DAC_VAR::getSize() const
	{
		size_t mem = 0;
		mem += sizeof(LCP_DAC_VAR);
		mem += sizeof(size_t)*((uint)rep->nLevels -1);
		mem += rep->getSize();
		return mem;
	}

	void LCP_DAC_VAR::save(ofstream & fp) const
	{
		saveValue(fp, lcp_type);
		saveValue(fp, n);
		rep->save(fp);
	}

	LCP_DAC_VAR* LCP_DAC_VAR::load(ifstream & fp) {
		LCP_DAC_VAR *lcp = new LCP_DAC_VAR();
		size_t type = loadValue<size_t>(fp);
		if(type!=DAC_VAR) {
			abort();
		}
		lcp->n = loadValue<size_t>(fp);
		lcp->rep = factorization_var::load(fp);
		lcp->next_p_var = new size_t[(size_t)((lcp->rep)->nLevels) -1];
		return lcp;
	}

	LCP_DAC_VAR::~LCP_DAC_VAR() {
		if(rep != NULL)
			delete (factorization_var *)rep;
		if(next_p_var != NULL)
			delete [] next_p_var;
	}

};
