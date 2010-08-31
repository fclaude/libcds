/* SequenceGMRChunk.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * gmr_chunk
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
 
#include "SequenceGMRChunk.h"

namespace cds_static {
  
using std::max;

SequenceGMRChunk::SequenceGMRChunk(uint * sequence, uint chunk_length, BitSequenceBuilder *bmb, PermutationBuilder *pmb) : Sequence(chunk_length){
  sigma = 0;
  for(uint i=0;i<chunk_length;i++) {
    sigma = max(sigma,sequence[i]);
  }
  sigma++;
  uint * X_bitmap = new uint[uint_len(1+chunk_length+sigma,1)];
  assert(X_bitmap!=NULL);
  for(uint i=0;i<uint_len(1+chunk_length+sigma,1);i++) X_bitmap[i]=0;
  uint pi_blen = bits(chunk_length-1);
  uint * pi = new uint[uint_len(pi_blen,chunk_length)];
  assert(pi!=NULL);
  for(uint i=0;i<uint_len(pi_blen,chunk_length);i++) pi[i] = 0;
  uint X_pos = 0;
  uint * counter = new uint[sigma+2];
  for(uint c=0;c<=sigma+1;c++) counter[c]=0;
  for(uint i=0;i<chunk_length;i++) counter[sequence[i]+1]++;

  for(uint c=0;c<sigma;c++) {
    X_pos++;
    for(uint i=0;i<counter[c+1];i++) {
      bitset(X_bitmap, X_pos);
      X_pos++;
    }
    counter[c+1]+=counter[c];
  }
  X_pos++;
  for(uint i=0;i<chunk_length;i++) {
    set_field(pi, pi_blen,counter[sequence[i]], i);
    counter[sequence[i]]++;
  }
  this->X = bmb->build(X_bitmap,X_pos); 
  assert(X!=NULL);
  delete [] X_bitmap;
  this->permutation = pmb->build(pi,chunk_length); 
  assert(permutation!=NULL);
  this->sigma = sigma;
	delete [] counter;
}

SequenceGMRChunk::SequenceGMRChunk() : Sequence(0) {
}

SequenceGMRChunk::~SequenceGMRChunk() {
	delete X;
  delete permutation;
}


uint SequenceGMRChunk::access(size_t j2) const {
  uint j = (uint)j2;
  uint invPerm = permutation->revpi(j); 
  uint rank_pos = X->select1(invPerm+1);
  uint ret = rank_pos - X->rank1(rank_pos);
  return ret;
}


size_t SequenceGMRChunk::select(uint i, size_t j2) const {
  uint j = (uint)j2;
  uint pos = X->select0(i+1) + j - i -1;
  return permutation->pi(pos); 
}


size_t SequenceGMRChunk::rank(uint i, size_t j2) const {
  uint j = (uint)j2;
  uint ini = X->select0(i+1)-i;
  uint ini_o = ini;
  uint fin = X->select0(i+2);
	if(fin<i+2) return 0;
	fin = fin-(i+2);
	if(fin<ini) return 0;
	if(permutation->pi(ini) > j) return 0;
	if(permutation->pi(ini) == j) return 1;
	if(ini==fin) return 1;
  while(ini < fin-1) {
		uint med = (ini+fin)/2;
    uint elem = permutation->pi(med); 
    if(elem >= j) fin = med;
    else ini = med;
  }
	while(fin>ini_o && permutation->pi(fin)>j) fin--;
  return fin-ini_o+1;
}


size_t SequenceGMRChunk::getSize() const {
  return sizeof(SequenceGMRChunk)+permutation->getSize()+X->getSize();
}

void SequenceGMRChunk::save(ofstream & fp) const {
  uint wr = GMR_CHUNK_HDR;
  saveValue(fp,wr);
  saveValue(fp,length);
  saveValue(fp,sigma);
  X->save(fp);
  permutation->save(fp);
}

SequenceGMRChunk * SequenceGMRChunk::load(ifstream & fp) {
  uint rd = loadValue<uint>(fp);
  if(rd!=GMR_CHUNK_HDR) return NULL;
  SequenceGMRChunk * ret = new SequenceGMRChunk();
  ret->length = loadValue<size_t>(fp);
  ret->sigma = loadValue<uint>(fp);
  ret->X = BitSequence::load(fp);
  ret->permutation = Permutation::load(fp);
  return ret;
}

};
