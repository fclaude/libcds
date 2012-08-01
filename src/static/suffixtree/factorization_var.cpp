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

#include <factorization_var.h>

namespace cds_static
{

	factorization_var::factorization_var() {
		tamCode = 0;
		iniLevel = NULL;
		base = NULL;
		base_bits = NULL;
		tablebase = NULL;
		tamtablebase = 0;
		listLength = 0;
		nLevels = 0;
		levelsIndex = NULL;
		levels = NULL;
		bS = NULL;
		rankLevels = NULL;
	}

	factorization_var::factorization_var(uint *list, uint l_Length, ushort* kvalues,  uint nkvalues) {
		uint *levelSizeAux;
		uint *cont;
		uint *contB;
		listLength = l_Length;
		register uint i;
		int j, k;
		uint value, newvalue;
		uint bits_BS_len = 0;
		ushort kval;
		uint oldval =0;
		uint newval =0;
		i=0;
		uint multval=1;
		do {
			oldval=newval;
			if(i>=nkvalues)
				kval = 1<<(kvalues[nkvalues-1]);
			else
				kval=1<<(kvalues[i]);
			multval*=kval;
			newval = oldval+multval;
			i++;
		}while(oldval<newval);
		tamtablebase = i;
		tablebase = new uint[tamtablebase];
		levelSizeAux = new uint[tamtablebase];
		cont = new uint[tamtablebase];
		contB = new uint[tamtablebase];
		oldval =0;
		newval =0;
		multval=1;
		for(i=0;i<tamtablebase;i++) {
			oldval=newval;
			if(i>=nkvalues)
				kval = 1<<(kvalues[nkvalues-1]);
			else
				kval=1<<(kvalues[i]);
			multval*=kval;
			newval = oldval+multval;
			tablebase[i]=oldval;
		}
		for(i=0;i<tamtablebase;i++)
			levelSizeAux[i]=0;
		//Space needed for each level
		for (i=0;i<listLength;i++) {
			value = list[i];
			for(j=0; j < (int)tamtablebase;j++)
				if(value>=tablebase[j])
					levelSizeAux[j]++;
		}
		j=0;
		//Contadores
		while((j<(int)tamtablebase)&&(levelSizeAux[j]!=0)) {
			j++;
		}
		nLevels = j;
		levelsIndex = new uint[nLevels+1];
		bits_BS_len =0;
		base = new ushort[nLevels];
		base_bits = new ushort[nLevels];
		for(i=0;i<nLevels;i++) {
			if(i>=nkvalues) {
				base[i]=1<<(kvalues[nkvalues-1]);
				base_bits[i]=kvalues[nkvalues-1];
			}
			else {
				base[i]=1<<(kvalues[i]);
				base_bits[i]=kvalues[i];
			}
		}
		uint tamLevels =0;
		for(i=0;i<nLevels;i++)
			tamLevels+=base_bits[i]*levelSizeAux[i];
		iniLevel = new uint[nLevels];
		tamCode=tamLevels;
		uint indexLevel=0;
		levelsIndex[0]=0;
		for(j=0;j<nLevels;j++) {
			levelsIndex[j+1]=levelsIndex[j] + levelSizeAux[j];
			iniLevel[j] = indexLevel;
			cont[j]=iniLevel[j];
			indexLevel+=levelSizeAux[j]*base_bits[j];
			contB[j]=levelsIndex[j];
		}
		levels = new uint[tamLevels/W+1];
		for(i=0; i< (tamLevels/W+1); i++)
			levels[i] = 0;
		bits_BS_len = levelsIndex[nLevels-1]+1;
		//Se pone el ultimo a 0 para ahorrarnos comparaciones despues en la descompresion
		uint * bits_BS = new uint[bits_BS_len/W+1];
		for(i=0; i<((bits_BS_len)/W+1);i++)
			bits_BS[i]=0;
		for(i=0;i<listLength;i++) {
			value = list[i];
			j=nLevels-1;
			while(j>=0) {
				if(value >= tablebase[j]) {
					newvalue = value- tablebase[j];
					for(k=0;k<j;k++) {
						bitwrite(levels,cont[k],base_bits[k],(byte)newvalue%base[k]);
						cont[k]+=base_bits[k];
						contB[k]++;
						newvalue = newvalue/base[k];
					}
					k=j;
					bitwrite(levels,cont[j],base_bits[j],(byte)newvalue%base[j]);
					cont[j]+=base_bits[j];
					contB[j]++;
					if(j<nLevels-1)
						bitset(bits_BS,contB[j]-1);
					break;
				}
				j--;
			}
			//Para j=0 solo se cubre el bitmap
		}
		//Para simular ultimo array:
		bitset(bits_BS,bits_BS_len-1);
		bS = new BitSequenceRG(bits_BS, bits_BS_len , 4);
		//	for(j=0;j<nLevels;j++)
		//		fprintf(stderr,"inilevel[%d]=%d\n",j,iniLevel[j]);
		rankLevels = new uint[nLevels];
		for(j=0;j<nLevels;j++)
			rankLevels[j]= bS->rank1(levelsIndex[j]-1);
		//	for(i=0;i<nLevels;i++)
		//		fprintf(stderr,"i:%d ranklevel: %d\n",i,rankLevels[i]);
		delete [] cont;
		delete [] contB;
		delete [] levelSizeAux;
		delete [] bits_BS;
	}

	uint factorization_var::access(uint param) {
		uint mult=0;
		register uint j;
		uint partialSum=0;
		uint ini = param-1;
		byte readByte;
		uint cont,pos, rankini;
		partialSum=0;
		j=0;
		pos=levelsIndex[j]+ini;
		mult=0;
		cont = iniLevel[j]+ini*base_bits[j];
		readByte = bitread(levels,cont,base_bits[j]);
		while((!bitget(((BitSequenceRG *)bS)->data,pos))) {
			rankini = bS->rank1(levelsIndex[j]+ini-1) - rankLevels[j];
			ini = ini-rankini;
			partialSum = partialSum+ (readByte<<mult);
			mult+=base_bits[j];
			j++;
			cont = iniLevel[j]+ini*base_bits[j];
			pos=levelsIndex[j]+ini;
			readByte = bitread(levels,cont,base_bits[j]);
			if(j==(uint)nLevels-1)
				break;
		}
		partialSum = partialSum + (readByte<<mult) + tablebase[j];
		return partialSum;
	}

	uint factorization_var::access_seq(uint param, size_t *next_pos, bool dir) {
		uint mult=0;
		register uint j;
		uint partialSum=0;
		uint ini = param-1;
		byte readByte;
		uint cont,pos, rankini;
		partialSum=0;
		j=0;
		pos=levelsIndex[j]+ini;
		mult=0;
		cont = iniLevel[j]+ini*base_bits[j];
		readByte = bitread(levels,cont,base_bits[j]);
		while((!bitget(((BitSequenceRG *)bS)->data,pos))) {
			partialSum = partialSum+ (readByte<<mult);
			if(next_pos[j]==0) {
				rankini = bS->rank1(levelsIndex[j]+ini-1) - rankLevels[j];
				ini = ini-rankini;
				if(dir)
					next_pos[j] = ini+1;
				else
					next_pos[j] = ini-1;
			}
			else {
				ini = next_pos[j];
				if(dir)
					next_pos[j] = next_pos[j]+1;
				else
					next_pos[j] = next_pos[j]-1;
			}

			mult+=base_bits[j];
			j++;
			cont = iniLevel[j]+ini*base_bits[j];
			pos=levelsIndex[j]+ini;
			readByte = bitread(levels,cont,base_bits[j]);
			if(j==(uint)nLevels-1)
				break;
		}
		partialSum = partialSum + (readByte<<mult) + tablebase[j];
		return partialSum;
	}

	uint factorization_var::getSize() {
		uint mem = sizeof(factorization_var);
								 //iniLevel
		mem += sizeof(uint)*nLevels;
								 //base
		mem += sizeof(ushort)*nLevels;
								 //base_bits
		mem += sizeof(ushort)*nLevels;
								 //tablebase
		mem += sizeof(uint)*tamtablebase;
								 //levelsIndex
		mem += sizeof(uint)*(nLevels+1);
								 //levels
		mem += sizeof(uint)*(tamCode/W+1);
		mem += bS->getSize();
								 //	rankLevels
		mem += sizeof(uint)*nLevels;
		return mem;
	}

	void factorization_var::save(ofstream & fp) {
		saveValue(fp, tamCode);
		saveValue(fp, tamtablebase);
		saveValue(fp, listLength);
		saveValue(fp, nLevels);
		saveValue(fp, iniLevel, nLevels);
		saveValue(fp, base, nLevels);
		saveValue(fp, base_bits, nLevels);
		saveValue(fp, tablebase, tamtablebase);
		saveValue(fp, levelsIndex, nLevels+1);
		saveValue(fp, levels, tamCode/W+1);
		saveValue(fp, rankLevels, nLevels);
		bS->save(fp);
	}

	factorization_var* factorization_var::load(ifstream & fp) {
		factorization_var *rep = new factorization_var();
		rep->tamCode = loadValue<uint>(fp);
		rep->tamtablebase = loadValue<uint>(fp);
		rep->listLength = loadValue<uint>(fp);
		rep->nLevels = loadValue<byte>(fp);
		rep->iniLevel = loadValue<uint>(fp, rep->nLevels);
		rep->base = loadValue<ushort>(fp,rep->nLevels);
		rep->base_bits = loadValue<ushort>(fp, rep->nLevels);
		rep->tablebase = loadValue<uint>(fp, rep->tamtablebase);
		rep->levelsIndex = loadValue<uint>(fp, rep->nLevels+1);
		rep->levels = loadValue<uint>(fp, rep->tamCode/W+1);
		rep->rankLevels = loadValue<uint>(fp, rep->nLevels);
		rep->bS = BitSequence::load(fp);
		return rep;
	}

	factorization_var::~factorization_var() {
		delete [] iniLevel;
		delete [] base;
		delete [] base_bits;
		delete [] tablebase;
		delete [] levelsIndex;
		delete [] levels;
		delete [] rankLevels;
		delete (BitSequenceRG *)bS;
	}

};
