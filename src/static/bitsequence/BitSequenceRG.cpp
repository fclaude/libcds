/* BitSequenceRG.cpp
   Copyright (C) 2005, Rodrigo Gonzalez, all rights reserved.

   New RANK, SELECT, SELECT-NEXT and SPARSE RANK implementations.
   Addaptation to libcds by Francisco Claude

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

#include <BitSequenceRG.h>
#include <cassert>
#include <cmath>
// #include <sys/types.h>

namespace cds_static
{
	/////////////
	//Rank(B,i)//
	/////////////
	//_factor = 0  => s=W*lgn
	//_factor = P  => s=W*P
	//Is interesting to notice
	//factor=2 => overhead 50%
	//factor=3 => overhead 33%
	//factor=4 => overhead 25%
	//factor=20=> overhead 5%

	BitSequenceRG::BitSequenceRG() {
		data=NULL;
		//  this->owner = true;
		this->n=0;
		this->factor=0;
	}

	BitSequenceRG::BitSequenceRG(const BitString & bs, uint _factor) {
		/*cout << "*****" << endl;
		cout << bitarray << endl;
		cout << _n << endl;
		cout << _factor << endl; */
		const uint * bitarray = bs.getData();
		size_t _n = bs.getLength();
		if(_factor==0) exit(-1);
		data=new uint[_n/W+1];
		for(size_t i=0;i<uint_len(_n,1);i++)
			data[i] = bitarray[i];
		for(size_t i=uint_len(_n,1);i<_n/W+1;i++)
			data[i] = 0;
		//this->owner = true;
		this->n=_n;
		uint lgn=bits(n-1);
		this->factor=_factor;
		if (_factor==0) this->factor=lgn;
		else this->factor=_factor;
		b=32;
		s=b*this->factor;
		integers = n/W+1;
		BuildRank();
		this->length = n;
		this->ones = rank1(n-1);
	}

	BitSequenceRG::BitSequenceRG( uint *bitarray, size_t _n, uint _factor) {
		/*cout << "*****" << endl;
		cout << bitarray << endl;
		cout << _n << endl;
		cout << _factor << endl; */
		if(_factor==0) exit(-1);
		data=new uint[_n/W+1];
		for(size_t i=0;i<uint_len(_n,1);i++)
			data[i] = bitarray[i];
		for(size_t i=uint_len(_n,1);i<_n/W+1;i++)
			data[i] = 0;
		//this->owner = true;
		this->n=_n;
		uint lgn=bits(n-1);
		this->factor=_factor;
		if (_factor==0) this->factor=lgn;
		else this->factor=_factor;
		b=32;
		s=b*this->factor;
		integers = n/W+1;
		BuildRank();
		this->length = n;
		this->ones = rank1(n-1);
	}

	BitSequenceRG::~BitSequenceRG() {
		delete [] Rs;
		delete [] data;
	}

	//Metodo que realiza la busqueda d
	void BitSequenceRG::BuildRank() {
		size_t num_sblock = n/s;
								 // +1 pues sumo la pos cero
		Rs = new uint[num_sblock+5];
		for(uint i=0;i<num_sblock+5;i++)
			Rs[i]=0;
		size_t j;
		Rs[0]=0;
		for (j=1;j<=num_sblock;j++) {
			Rs[j]=Rs[j-1];
			Rs[j]+=BuildRankSub((j-1)*factor,factor);
		}
	}

	size_t BitSequenceRG::BuildRankSub(size_t ini, size_t bloques) {
		uint rank=0,aux;
		for(uint i=ini;i<ini+bloques;i++) {
			if (i < integers) {
				aux=data[i];
				rank+=popcount(aux);
			}
		}
		return rank;			 //retorna el numero de 1's del intervalo

	}

	size_t BitSequenceRG::rank1(const size_t i1) const
	{
		uint i=(uint)i1;
		++i;
		uint resp=Rs[i/s];
		uint aux=(i/s)*factor;
		for (uint a=aux;a<i/W;a++)
			resp+=popcount(data[a]);
		resp+=popcount(data[i/W]  & ((1<<(i & mask31))-1));
		return resp;
	}

	bool BitSequenceRG::access(const size_t i) const
	{
		return (1u << (i % W)) & data[i/W];
	}

	void BitSequenceRG::save(ofstream & f) const
	{
		uint wr = BRW32_HDR;
		saveValue(f,wr);
		saveValue(f,n);
		saveValue(f,factor);
		saveValue(f,data,integers);
		saveValue(f,Rs,n/s+1);
	}

	BitSequenceRG * BitSequenceRG::load(ifstream & f) {
		assert(f.good());
		uint type = loadValue<uint>(f);
		if(type!=BRW32_HDR) {	 // throw exception
			abort();
		}
		BitSequenceRG * ret = new BitSequenceRG();
		ret->n = loadValue<size_t>(f);
		ret->b = 32;
		ret->factor = loadValue<size_t>(f);
		ret->s = ret->b*ret->factor;
		ret->integers = (ret->n+1)/W+((ret->n+1)%W!=0?1:0);
		ret->data = loadValue<uint>(f,ret->integers);
		ret->Rs = loadValue<uint>(f,ret->n/ret->s+1);
		ret->length = ret->n;
		ret->ones = ret->rank1(ret->n-1);
		return ret;
	}

	size_t BitSequenceRG::SpaceRequirementInBits() const
	{
		return uint_len(n,1)*sizeof(uint)*8+(n/s)*sizeof(uint)*8 +sizeof(this)*8;
	}

	size_t BitSequenceRG::getSize() const
	{
		//cout << "n=" << n << " size=" << SpaceRequirementInBits() << endl;
		return /*sizeof(BitSequenceRG)+*/SpaceRequirementInBits()/8;
	}

	size_t BitSequenceRG::SpaceRequirement() const
	{
		return n/8+(n/s)*sizeof(uint)+sizeof(BitSequenceRG);
	}

	size_t BitSequenceRG::selectPrev1(const size_t start) const
	{
		// returns the position of the previous 1 bit before and including start.
		// tuned to 32 bit machine

		uint i = start >> 5;
		int offset = (start % W);
		uint aux2 = data[i] & (-1u >> (31-offset));

		if (aux2 > 0) {
			if ((aux2&0xFF000000) > 0) return i*W+23+prev_tab[(aux2>>24)&0xFF];
			else if ((aux2&0xFF0000) > 0) return i*W+15+prev_tab[(aux2>>16)&0xFF];
			else if ((aux2&0xFF00) > 0) return i*W+7+prev_tab[(aux2>>8)&0xFF];
			else  return i*W+prev_tab[aux2&0xFF]-1;
		}
		for (uint k=i-1;;k--) {
			aux2=data[k];
			if (aux2 > 0) {
				if ((aux2&0xFF000000) > 0) return k*W+23+prev_tab[(aux2>>24)&0xFF];
				else if ((aux2&0xFF0000) > 0) return k*W+15+prev_tab[(aux2>>16)&0xFF];
				else if ((aux2&0xFF00) > 0) return k*W+7+prev_tab[(aux2>>8)&0xFF];
				else  return k*W+prev_tab[aux2&0xFF]-1;
			}
		}
		return 0;
	}

	size_t BitSequenceRG::selectNext1(const size_t k1) const
	{
		uint k = (uint)k1;
		uint count = k;
		uint des,aux2;
		des=count%W;
		aux2= data[count/W] >> des;
		if (aux2 > 0) {
			if ((aux2&0xff) > 0) return count+select_tab[aux2&0xff]-1;
			else if ((aux2&0xff00) > 0) return count+8+select_tab[(aux2>>8)&0xff]-1;
			else if ((aux2&0xff0000) > 0) return count+16+select_tab[(aux2>>16)&0xff]-1;
			else {return count+24+select_tab[(aux2>>24)&0xff]-1;}
		}

		for (uint i=count/W+1;i<integers;i++) {
			aux2=data[i];
			if (aux2 > 0) {
				if ((aux2&0xff) > 0) return i*W+select_tab[aux2&0xff]-1;
				else if ((aux2&0xff00) > 0) return i*W+8+select_tab[(aux2>>8)&0xff]-1;
				else if ((aux2&0xff0000) > 0) return i*W+16+select_tab[(aux2>>16)&0xff]-1;
				else {return i*W+24+select_tab[(aux2>>24)&0xff]-1;}
			}
		}
		return n;
	}

	size_t BitSequenceRG::select1(const size_t x1) const
	{
		uint x=x1;
		// returns i such that x=rank(i) && rank(i-1)<x or n if that i not exist
		// first binary search over first level rank structure
		// then sequential search using popcount over a int
		// then sequential search using popcount over a char
		// then sequential search bit a bit
		if(x>ones) return (uint)(-1);

		//binary search over first level rank structure
		uint l=0, r=n/s;
		uint mid=(l+r)/2;
		uint rankmid = Rs[mid];
		while (l<=r) {
			if (rankmid<x)
				l = mid+1;
			else
				r = mid-1;
			mid = (l+r)/2;
			rankmid = Rs[mid];
		}
		//sequential search using popcount over a int
		uint left;
		left=mid*factor;
		x-=rankmid;
		uint j=data[left];
		uint ones = popcount(j);
		while (ones < x) {
			x-=ones;left++;
			if (left > integers) return n;
			j = data[left];
			ones = popcount(j);
		}
		//sequential search using popcount over a char
		left=left*b;
		rankmid = popcount8(j);
		if (rankmid < x) {
			j=j>>8;
			x-=rankmid;
			left+=8;
			rankmid = popcount8(j);
			if (rankmid < x) {
				j=j>>8;
				x-=rankmid;
				left+=8;
				rankmid = popcount8(j);
				if (rankmid < x) {
					j=j>>8;
					x-=rankmid;
					left+=8;
				}
			}
		}

		// then sequential search bit a bit
		while (x>0) {
			if  (j&1) x--;
			j=j>>1;
			left++;
		}
		return left-1;
	}

	size_t BitSequenceRG::select0(const size_t x1) const
	{
		uint x = (uint)x1;
		// returns i such that x=rank_0(i) && rank_0(i-1)<x or n if that i not exist
		// first binary search over first level rank structure
		// then sequential search using popcount over a int
		// then sequential search using popcount over a char
		// then sequential search bit a bit
		if(x>n-ones) return (uint)(-1);

		//binary search over first level rank structure
		if(x==0) return 0;
		uint l=0, r=n/s;
		uint mid=(l+r)/2;
		uint rankmid = mid*factor*W-Rs[mid];
		while (l<=r) {
			if (rankmid<x)
				l = mid+1;
			else
				r = mid-1;
			mid = (l+r)/2;
			rankmid = mid*factor*W-Rs[mid];
		}
		//sequential search using popcount over a int
		uint left;
		left=mid*factor;
		x-=rankmid;
		uint j=data[left];
		uint zeros = W-popcount(j);
		while (zeros < x) {
			x-=zeros;left++;
			if (left > integers) return n;
			j = data[left];
			zeros = W-popcount(j);
		}
		//sequential search using popcount over a char
		left=left*b;
		rankmid = 8-popcount8(j);
		if (rankmid < x) {
			j=j>>8;
			x-=rankmid;
			left+=8;
			rankmid = 8-popcount8(j);
			if (rankmid < x) {
				j=j>>8;
				x-=rankmid;
				left+=8;
				rankmid = 8-popcount8(j);
				if (rankmid < x) {
					j=j>>8;
					x-=rankmid;
					left+=8;
				}
			}
		}

		// then sequential search bit a bit
		while (x>0) {
			if  (j%2 == 0 ) x--;
			j=j>>1;
			left++;
		}
		left--;
		if (left > n)  return n;
		else return left;
	}
};
