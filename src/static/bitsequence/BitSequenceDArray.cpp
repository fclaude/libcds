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

#include <BitSequenceDArray.h>

namespace cds_static
{

	static unsigned int __selecttbl_D[8*256];
	static int built_D = 0;

	void make___selecttbl_D(void) {
		if(built_D) return;
		built_D = 1;
		int i,x,r;
		uint buf[1];
		buf[0] = 0;
		for (x = 0; x < 256; x++) {
			__setbits(buf,0,8,x);
			for (r=0; r<8; r++) __selecttbl_D[(r<<8)+x] = -1;
			r = 0;
			for (i=0; i<8; i++) {
				if (__getbit(buf,i)) {
					__selecttbl_D[(r<<8)+x] = i;
					r++;
				}
			}
		}
	}

	BitSequenceDArray::BitSequenceDArray() {
		length = m = nl = 0;
		a =  NULL;
		lp = NULL;
		sl = NULL;
		ss = NULL;
		p = NULL;
		rl = NULL;
		rs = NULL;
	}

	BitSequenceDArray::BitSequenceDArray(const BitString & bs) {
		uint * tmp_seq = new uint[uint_len(bs.getLength(),1)+1];
		ones = 0;
		for(uint i=0;i<uint_len(bs.getLength(),1)+1;i++)
			tmp_seq[i] = 0;
		for(uint i=0;i<bs.getLength();i++)
		if(bs[i]) {
			__setbit(tmp_seq,i,1);
			ones++;
		}
		if(ones)
			build(tmp_seq, bs.getLength());
	}

	BitSequenceDArray::BitSequenceDArray(uint *buf, size_t n) {
		uint * tmp_seq = new uint[uint_len(n,1)+1];
		ones = 0;
		for(uint i=0;i<uint_len(n,1)+1;i++)
			tmp_seq[i] = 0;
		for(uint i=0;i<n;i++)
		if(bitget(buf,i)) {
			__setbit(tmp_seq,i,1);
			ones++;
		}
		if(ones)
			build(tmp_seq,n);
	}

	void BitSequenceDArray::build(uint *buf, size_t n) {
		int i, j, m2;
		int p2, pp;
		int il, is, ml, ms;
		int r;
		uint*s;

		make___selecttbl_D();

		if(L/LLL == 0) {
			cout << "ERROR: L=" << L << "  LLL=" << LLL << endl;
			exit(1);
		}

		m2 = 0;
		for(i=0; i < (int)n; i++)
			m2 += __getbit(buf,i);

		length = n;
		m = m2;
		a = buf;
		s = new uint[m];
		m2 = 0;
		s_ss = s_sl = 0;
		/*store in s the place where are 1's in the array*/
		for(i=0; i<(int)n; i++) {
			if (__getbit(buf,i)) {
				m2++;
				s[m2-1] = i;
			}
		}

		nl = (m2-1)/L + 1;
		lp = new uint[nl+1];
		p = new uint[nl+1];

		for(i=0; i<(int)(nl+1); i++) {
			lp[i]=0;
			p[i]=0;
		}

		for(r = 0; r < 2; r++) {
			ml = ms = 0;
			for (il = 0; il < (int)nl; il++) {
				pp = s[il*L];
				lp[il] = pp;
				i = min((il+1)*(int)L-1,(int)m-1);
				p2 = s[i];
				if (p2 - pp >= (int)LL) {
					if (r == 1) {
						for (is = 0; is < (int)L; is++) {
							if (il*(int)L+is >= m2)
								break;
							sl[ml*L+is] = s[il*L+is];
						}
					}
					p[il] = -(ml+1);
					ml++;
				}
				else {
					if (r == 1) {
						for (is = 0; is < (int)(L/LLL); is++) {
							if (il*(int)L+is*(int)LLL >= (int)m)
								break;
							ss[ms*(L/LLL)+is] = s[il*L+is*LLL] - pp;
						}
					}
					p[il] = ms;
					ms++;
				}
			}
			if (r == 0) {
				s_sl = ml*L+1;
				s_ss = ms*(L/LLL)+1;
				sl = new uint[s_sl];
				for(uint i_sl=0; i_sl<s_sl; i_sl++)
					sl[i_sl]=0;
				ss = new uint[s_ss];
				for(uint i_ss=0; i_ss<s_ss; i_ss++)
					ss[i_ss]=0;
			}
		}
		delete [] s;
		/*this is for compute rank*/
		rl = new uint[n/RR+2];
		rs = new uchar[n/RRR+2];
		for(i=0; i<(int)(n/RR+2); i++) {
			rl[i]=0;
		}
		r = 0;
		for (i=0; i < (int)n; i+=RR) {
			rl[i/RR] = r;
			m2 = 0;
			for (j=0; j<(int)RR; j++) {
				if (j % RRR == 0)
					rs[(i+j)/RRR] = m2;
				if (i+j < (int)n && __getbit(buf,i+j)==1)
					m2++;
			}
			r += m2;
		}
	}

	BitSequenceDArray::~BitSequenceDArray() {
		delete [] a;
		delete [] lp;
		delete [] sl;
		delete [] ss;
		delete [] p;
		delete [] rl;
		delete [] rs;
	}

	size_t BitSequenceDArray::select1(size_t i) const
	{
		int p2,r;
		int il;
		int rr;
		uint x;
		uint *q;
		if (i == 0)
			return (uint)-1;
		i--;
		il = p[i>>logL];
		if (il < 0) {
			il = -il-1;
			p2 = sl[(il<<logL)+(i & (L-1))];
		}
		else {
			p2 = lp[i>>logL];
			p2 += ss[(il<<(logL-logLLL))+(i & (L-1))/LLL];
			r = i - (i & (LLL-1));
			q = &(a[p2>>logD]);

			//		if (f == 1) {
			rr = p2 & (D-1);
			r -= __popCount(*q >> (D-1-rr));
			p2 = p2 - rr;
			while (1) {
				rr = __popCount(*q);
				if (r + rr >= (int)i)
					break;
				r += rr;
				p2 += D;
				q++;
			}
			x = *q;
			while (1) {
				rr = _popCount[x >> (D-8)];
				if (r + rr >= (int)i)
					break;
				r += rr;
				p2 += 8;
				x <<= 8;
			}
			p2 += __selecttbl_D[((i-r-1)<<8)+(x>>(D-8))];
			//	}
		}
		return p2;
	}

	size_t BitSequenceDArray::rank0(size_t i) const
	{
		if(i+1==0)
			return 0;
		return 1+i-rank1(i);
	}

	size_t BitSequenceDArray::rank1(size_t i) const
	{
		if(i+1 == 0)
			return 0;
		int r,j;
		uint *p2;
		//sum pre-calculated values
		// i>>logRR = i/(2^logRR)
		r = rl[i>>logRR] + rs[i>>logRRR];
								 //a + ( (i/2^6)*2^(1?))
		p2 = a + ((i>>logRRR) << (logRRR-logD));
		j = i & (RRR-1);
		if (j < (int)D) {
			r += __popCount(*p2 >> (D-1-j));
		}
		else {
			r += __popCount(*p2) + __popCount(p2[1] >> (D-1-(j-D)));
		}
		return r;
	}

	size_t BitSequenceDArray::getSize() const
	{
		size_t mem = 0;
		mem += sizeof(BitSequenceDArray);
		mem += length/8;		 //a
								 // lp
		mem += (nl+1)*sizeof(uint);
								 // p
		mem += (nl+1)*sizeof(uint);
								 //ss
		mem +=  s_ss*sizeof(uint);
		mem += s_sl*sizeof(uint);//sl
								 //rl
		mem += (length/RR+2)*sizeof(uint);
								 //rs
		mem += (length/RRR+2)*sizeof(uchar);
		return mem;
	}

	void BitSequenceDArray::save(ofstream & fp) const
	{
		uint wr = DARRAY_HDR;
		saveValue(fp, wr);
		saveValue(fp, length);
		saveValue(fp, ones);
		saveValue(fp, m);
		saveValue(fp, nl);
		saveValue(fp, s_ss);
		saveValue(fp, s_sl);
		long long nb = 1;
		nb = (nb*length+W-1)/W;
		saveValue(fp, a, (uint)nb);
		saveValue(fp, lp, nl+1);
		saveValue(fp, sl, s_sl);
		saveValue(fp, ss, s_ss);
		saveValue(fp, p, nl+1);
		saveValue(fp, rl, (length/RR+2));
		saveValue(fp, rs, (length/RRR+2));
	}

	BitSequenceDArray *  BitSequenceDArray::load(ifstream & fp) {
		uint id = loadValue<uint>(fp);
		if(id!=DARRAY_HDR) return NULL;
		BitSequenceDArray * ret = new BitSequenceDArray();
		ret->length = loadValue<size_t>(fp);
		ret->ones = loadValue<size_t>(fp);
		ret->m = loadValue<uint>(fp);
		ret->nl = loadValue<uint>(fp);
		ret->s_ss = loadValue<uint>(fp);
		ret->s_sl = loadValue<uint>(fp);
		long long nb = 1;
		nb = (nb*ret->length+W-1)/W;
		ret->a = loadValue<uint>(fp, (uint)nb);
		ret->lp = loadValue<uint>(fp, ret->nl+1);
		ret->sl = loadValue<uint>(fp, ret->s_sl);
		ret->ss = loadValue<uint>(fp, ret->s_ss);
		ret->p = loadValue<uint>(fp, ret->nl+1);
		ret->rl = loadValue<uint>(fp, (ret->length/RR+2));
		ret->rs = loadValue<uchar>(fp, (ret->length/RRR+2));
		make___selecttbl_D();
		return ret;
	}

};
