/* SequenceGMR.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * GMR
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

#include <SequenceGMR.h>
namespace cds_static {
  
using std::max;
SequenceGMR::SequenceGMR(uint * sequence, size_t n, uint chunk_length, BitSequenceBuilder * bmb, SequenceBuilder * ssb) : Sequence(n) {
  length = n;
	if(length%chunk_length) length+=chunk_length-length%chunk_length;
	uint * new_seq = new uint[length];
  sigma = 0;
	for(uint i=0;i<n;i++){
		new_seq[i] = sequence[i]+1;
    sigma = max(sigma,new_seq[i]);
	}
  sigma++;
	for(uint i=n;i<length;i++)
		new_seq[i] = sigma;
	if(length!=n) sigma++;
  this->chunk_length = chunk_length;
  build(new_seq,bmb,ssb);
	delete [] new_seq;
}

SequenceGMR::SequenceGMR() : Sequence(0) {
}

SequenceGMR::~SequenceGMR() {
  delete B;
  for (uint i=0;i<length/chunk_length;i++)
    delete chunk[i];
  delete [] chunk;
}


void SequenceGMR::build(uint * sequence, BitSequenceBuilder * bmb, SequenceBuilder * ssb) {
  uint num_chunks = length/chunk_length;
  chunk = new Sequence*[num_chunks];
  assert(chunk!=NULL);
  for (uint i=0;i<num_chunks;i++) {
    chunk[i] = ssb->build(sequence+i*chunk_length, chunk_length);
    assert(chunk[i]!=NULL);
  }
  uint * ones = get_ones(sequence);
  uint *B_bitmap = new uint[(2+length+(unsigned long long)num_chunks*sigma)/W+1];
    assert(B_bitmap!=NULL);
  for (uint i=0;i<(2+length+(unsigned long long)num_chunks*sigma)/W+1;i++)
    B_bitmap[i] = 0;
  uint pos=0;
  for (unsigned long long i=0;i<(unsigned long long)num_chunks*sigma;i++) {
    for (uint j=0;j<ones[i];j++) {
      bitset(B_bitmap, pos);
      pos++;
    }
    pos++;
  }
  pos++;
  B = bmb->build(B_bitmap, pos);
  delete [] B_bitmap;
  delete [] ones;
}


uint * SequenceGMR::get_ones(uint * sequence) {
  uint * ones = new uint[(unsigned long long)(length/chunk_length)*sigma];
  assert(ones!=NULL);
  for (uint i=0;i<(unsigned long long)(length/chunk_length)*sigma;i++) ones[i] = 0;
  for (uint i=0;i<length;i++) {
    uint whichChunk = (uint)(((unsigned long long)sequence[i]*length+i)/chunk_length);
    ones[whichChunk]++;
  }
  return ones;
}


size_t SequenceGMR::rank(uint c, size_t j) const {
  c++;
  size_t i = j/chunk_length;
  size_t bp = (c)*(length/chunk_length);
  size_t rank_pos = B->select0(bp);
  size_t prev = rank_pos-bp+1;
  size_t sum = B->rank1(B->select0(bp+i)) - prev;
  size_t cr = chunk[i]->rank(c,j-i*chunk_length);
	/*if(c==0) {
		cout << "c=" << c << " j=" << j << endl;
		cout << "i=" << i << endl;
		cout << "bp=" << bp << endl;
		cout << "rank_pos=" << rank_pos << endl;
		cout << "prev=" << prev << endl;
		cout << "sum=" << sum << endl;
		cout << "cr=" << cr << endl;
	}*/
  return sum + cr;
}


size_t SequenceGMR::select(uint c, size_t j2) const {
    uint j = (uint)j2;
   c++;
  uint rank_pos = B->select0(c*(length/chunk_length));
  uint prev = B->rank1(rank_pos);
  uint sel = prev+j;
  uint block = (B->select1(sel));
  uint i = block-sel+1;
  uint desp = B->rank1(B->select0((i)))-prev;
  if (desp+1==0) desp=0;
  uint rchunk = i%(length/chunk_length);
	/*if(j==90) {
		cout << "------------------------------" << endl;
		cout << "c=" << c << "  j=" << j << endl;
		cout << "chunk_length=" << chunk_length << endl;
		cout << "rank_pos=" << rank_pos << endl;
		cout << "prev=" << prev << endl;
		cout << "sel=" << sel << endl;
		cout << "block=" << block << endl;
		cout << "i=" << i << endl;
		cout << "desp=" << desp << endl;
		cout << "rchunk=" << rchunk << endl;
		cout << "j-desp=" << j-desp << endl;
	}*/
  return (rchunk*chunk_length)+chunk[rchunk]->select(c, j-desp);
}


uint SequenceGMR::access(size_t j) const{
  return chunk[j/chunk_length]->access(j%chunk_length)-1;
}


size_t SequenceGMR::getSize() const{
  uint s = 0;
  for (uint i=0;i<length/chunk_length;i++)
    s += sizeof(void*)+chunk[i]->getSize();
  return s+B->getSize()+sizeof(SequenceGMR);
}

void SequenceGMR::save(ofstream & fp) const {
  uint wr = GMR_HDR;
  saveValue(fp,wr);
  saveValue(fp,length);
  saveValue(fp,sigma);
  saveValue(fp,chunk_length);
  B->save(fp);
  for(uint i=0;i<length/chunk_length;i++)
    chunk[i]->save(fp);
}

SequenceGMR * SequenceGMR::load(ifstream & fp) {
  uint rd = loadValue<uint>(fp);
  if(rd!=GMR_HDR) return NULL;
  SequenceGMR * ret = new SequenceGMR();
  ret->length = loadValue<uint>(fp);
  ret->sigma = loadValue<uint>(fp);
  ret->chunk_length = loadValue<uint>(fp);
  ret->B = BitSequence::load(fp);
  if(ret->B==NULL) {
    delete ret;
    return NULL;
  }
  ret->chunk = new Sequence*[ret->length/ret->chunk_length];
  for(uint i=0;i<ret->length/ret->chunk_length;i++) {
    ret->chunk[i] = Sequence::load(fp);
    if(ret->chunk[i]==NULL) {
      delete ret;
      return NULL;
    }
  }
  return ret;
}

};
