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

#include <factorization.h>

#define BASE 16
#define BASE_BITS 4

const unsigned int TABLEBASE[7] = {0,16,272,4368,69904,1118480,17895696};

const unsigned short TABLESUM[256] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
	8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
	9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
	11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
	12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
	13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
	14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
};

namespace cds_static
{

	factorization::factorization() {
		listLength =0;
		nLevels =0;
		levelsIndex = NULL;
		levels = NULL;
		bS = NULL;
		rankLevels = NULL;
	}

	factorization::factorization(uint *list,uint l_Length) {
		uint levelSizeAux[7]={0,0,0,0,0,0,0};
		uint cont[7]={0,0,0,0,0,0,0};
		listLength = l_Length;
		register uint i;
		int j, k;
		uint value, newvalue;
		uint bits_BS_len = 0;

		//space needed for all the levels
		for (i=0; i<listLength; i++) {
			value = list[i];
			for(j=0; j<7; j++)
				if(value>=TABLEBASE[j])
					levelSizeAux[j]++;
		}

		j=0;
		while((j<7)&&(levelSizeAux[j]!=0)) {
			//fprintf(stderr,"levelSize[%d]=%d\n",j,levelSizeAux[j]);
			if(levelSizeAux[j]%2)
				levelSizeAux[j]++;
			j++;
		}
		nLevels = j;
		levelsIndex = new uint[nLevels+1];
		for(j=0; j<= nLevels; j++)
			levelsIndex[j] = 0;
		levelsIndex[0]=0;
		for(j=0; j<nLevels; j++) {
			levelsIndex[j+1] = levelsIndex[j] + levelSizeAux[j];
			cont[j] = levelsIndex[j];
		}
		levels = new byte[levelsIndex[nLevels]/2];
		bits_BS_len = levelsIndex[nLevels-1]+1;
		//The last one is 0. with this we avoid some comparation when decompress
		uint * bits_BS = new uint[((bits_BS_len+W-1)/W)];
		for(i=0; i<(W-1+bits_BS_len)/W;i++)
			bits_BS[i]=0;

		//If cont is odd, then is put in the upper part of the byte(<<BASE_BITS)
		//in other case:
		for(i=0;i<listLength;i++) {
			value = list[i];
			j= nLevels-1;
			while(j>=0) {
				if(value >= TABLEBASE[j]) {
					newvalue = value - TABLEBASE[j];
					for(k=0; k<j; k++) {
						if(cont[k]%2)
							levels[cont[k]/2]=levels[cont[k]/2]+(((byte)newvalue%BASE)<<BASE_BITS);
						else
							levels[cont[k]/2]=(byte)newvalue%BASE;
						cont[k]++;
						newvalue = newvalue/BASE;
					}
					if(cont[j]%2)
						levels[cont[j]/2] = levels[cont[j]/2]+(((byte)newvalue%BASE)<<BASE_BITS);
					else
						levels[cont[j]/2] = (byte)newvalue%BASE;
					cont[k]++;
					if(j<nLevels-1)
						bitset(bits_BS,cont[j]-1);
					break;
				}
				j--;
			}
		}
		//The last array:
		bitset(bits_BS,bits_BS_len-1);
								 //(bitarray, length, factor)
		bS = new BitSequenceRG(bits_BS, bits_BS_len, 4);
		// factor=2 => overhead 50%
		// factor=3 => overhead 33%
		// factor=4 => overhead 25%
		// factor=20=> overhead 5%
		delete [] bits_BS;
		//Contadores
		rankLevels = new uint[nLevels];
		for(j=0; j<nLevels; j++)
			rankLevels[j]= bS->rank1(levelsIndex[j]-1);
	}

	uint factorization::access(uint param) {
		uint mult=0;
		register uint j;
		uint partialSum=0;
		uint ini=param-1;
		uint * bsData = ((BitSequenceRG *)bS)->data;
		byte * level;
		byte readByte;
		uint pos, rankini, cont;

		j=0;
		level= levels + (levelsIndex[j]>>1);
		cont = ini + (levelsIndex[j]&0x1);
		pos= levelsIndex[j]+ini;
		mult=0;
		readByte = ((*(level+(cont>>1)))>>(BASE_BITS*(cont&0x1)))&0xF;
		while((!bitget(bsData,pos))) {
			rankini = bS->rank1(levelsIndex[j]+ini-1) - rankLevels[j];
			ini = ini-rankini;
			partialSum = partialSum + (readByte<<mult);
			j++;
			mult+=BASE_BITS;
			level =levels + (levelsIndex[j]>>1);
			cont=ini+(levelsIndex[j]&0x1);
			pos= levelsIndex[j]+ini;
			readByte = ((*(level+(cont>>1)))>>(BASE_BITS*(cont&0x1)))&0xF;
			if( (int)j >= nLevels-1) {
				break;
			}
		}
		partialSum = partialSum + (readByte<<mult) + TABLEBASE[j];
		return partialSum;
	}

	uint factorization::access_seq(uint param, size_t *next_pos, bool dir) {
		uint mult=0;
		register uint j;
		uint partialSum=0;
		uint ini=param-1;
		uint * bsData = ((BitSequenceRG *)bS)->data;
		byte * level;
		byte readByte;
		uint pos, rankini, cont;

		j=0;
		level= levels + (levelsIndex[j]>>1);
		cont = ini + (levelsIndex[j]&0x1);
		pos= levelsIndex[j]+ini;
		mult=0;
		readByte = ((*(level+(cont>>1)))>>(BASE_BITS*(cont&0x1)))&0xF;
		while((!bitget(bsData,pos))) {
			partialSum = partialSum + (readByte<<mult);

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

			j++;
			mult+=BASE_BITS;
			level =levels + (levelsIndex[j]>>1);
			cont=ini+(levelsIndex[j]&0x1);
			pos= levelsIndex[j]+ini;
			readByte = ((*(level+(cont>>1)))>>(BASE_BITS*(cont&0x1)))&0xF;
			if( (int)j >= nLevels-1) {
				break;
			}
		}
		partialSum = partialSum + (readByte<<mult) + TABLEBASE[j];
		return partialSum;
	}

	void factorization::save(ofstream & fp) {
		saveValue(fp, listLength);
		saveValue(fp, nLevels);
		saveValue(fp, levelsIndex, nLevels+1);
		saveValue(fp, rankLevels, nLevels);
		saveValue(fp, levels, levelsIndex[nLevels]/2);
		bS->save(fp);
	}

	factorization* factorization::load(ifstream & fp) {
		factorization *rep = new factorization();
		rep->listLength = loadValue<uint>(fp);
		rep->nLevels = loadValue<byte>(fp);
		rep->levelsIndex = loadValue<uint>(fp, rep->nLevels+1);
		rep->rankLevels = loadValue<uint>(fp, rep->nLevels);
		rep->levels = loadValue<byte>(fp, (rep->levelsIndex[rep->nLevels]/2));
		rep->bS = BitSequence::load(fp);
		return rep;
	}

	uint factorization::getSize() {
		uint mem=0;
		mem += sizeof(factorization);
		mem += (nLevels+1)*sizeof(uint);
		mem += sizeof(uint)*nLevels;
		mem += sizeof(byte)*(levelsIndex[nLevels]/2);
		mem += bS->getSize();
		return mem;
	}

	factorization:: ~factorization() {
		delete[] levelsIndex;
		delete[] levels;
		delete[] rankLevels;
		delete (BitSequenceRG *)bS;
	}

};
