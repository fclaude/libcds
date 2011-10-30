/**
 *    ssa.cpp
 *    Copyright (C) 2011  Francisco Claude F.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "ssa.h"

ssa::ssa(uchar *text, uint n, bool free_text) {
  assert(n>0);

  // Initial values and default constructors
  this->n=n;
  this->n1=n+1;
  this->_seq = text;
  this->built = false;
  this->free_text=free_text;

  // Default sampling values
  samplepos = 64;
  samplesuff = 64;

  #ifdef VERBOSE
  cout << "ssa" << endl;
  cout << " n=" << n << endl;
  cout << " free_text=" << free_text << endl;
  #endif

  // Structures that will be built after calling build_index()
  _sa = NULL;
  bwt = NULL;
  _bwt = NULL;
  sampled = NULL;
  pos_sample = NULL;
  suff_sample = NULL;
}


ssa::~ssa() {
  if(_seq!=NULL && free_text)
    delete [] _seq;
  if(_bwt!=NULL)
    delete [] _bwt;
  if(bwt!=NULL)
    delete bwt;
  if(_sa!=NULL)
    delete [] _sa;
  if(pos_sample!=NULL)
    delete [] pos_sample;
  if(suff_sample!=NULL)
    delete [] suff_sample;
  if(sampled!=NULL)
    delete sampled;
  delete [] sbuff;
}

#include <cppUtils.h>
using namespace cds_utils;

void ssa::save(ofstream & fp) {
  saveValue(fp,n);
  saveValue(fp,sigma);
  saveValue(fp,maxV);
  saveValue(fp,samplepos);
  saveValue(fp,samplesuff);
  bwt->save(fp);
  sampled->save(fp);
  saveValue(fp,pos_sample,2+n/samplepos);
  saveValue(fp,suff_sample,1+n/samplesuff);
  saveValue(fp,occ,maxV+1);
}

void ssa::print_bwt() {
  for(size_t i=0;i<=n;i++) {
    uint symbol = bwt->access(i);
    if(symbol>0)
      cout << (uchar)symbol;
  }
}

uint ssa::length() {
  return n;
}


ssa::ssa(ifstream & fp) {
  built = true;
  _sa = NULL;
  bwt = NULL;
  _bwt = NULL;
  sampled = NULL;
  pos_sample = NULL;
  suff_sample = NULL;

  _seq=NULL;

  n = loadValue<uint>(fp);
  sigma = loadValue<uint>(fp);
  maxV = loadValue<uint>(fp);
  samplepos = loadValue<uint>(fp);
  samplesuff = loadValue<uint>(fp);
  bwt = Sequence::load(fp);
  sampled = BitSequence::load(fp);
  pos_sample = loadValue<uint>(fp,2+n/samplepos);
  suff_sample = loadValue<uint>(fp,1+n/samplesuff);
  occ = loadValue<uint>(fp,maxV+1);
  sbuff = new uchar[samplepos+1];
  spos = (uchar)-(samplepos+1);
}


uint ssa::size() {
  uint size = bwt->getSize();
  size += sizeof(uint)*(2+n/samplepos);
  size += sizeof(uint)*(1+n/samplesuff);
  size += sizeof(ssa);
  size += sizeof(uchar)*(1+samplepos);
  size += (1+maxV)*sizeof(uint);
  size += sampled->getSize();
  return size;
}


void ssa::print_stats() {
  cout << "ssa stats:" << endl;
  cout << "****************" << endl;
  cout << "Total space  : " << size() << endl;
  cout << endl;
  cout << " bwt         : " << bwt->getSize() << endl;
  cout << " pos sample  : " << sizeof(uint)*(2+n/samplepos) << endl;
  cout << " suff sample : " << sizeof(uint)*(1+n/samplesuff) << endl;
  cout << " occ         : " << (maxV+1)*sizeof(uint) << endl;
  cout << endl;
}



bool ssa::set_samplepos(uint sample) {
  if(built) return false;
  samplepos = sample;
  return true;
}


bool ssa::set_samplesuff(uint sample) {
  if(built) return false;
  samplesuff = sample;
  return true;
}


bool ssa::build_index(Array * v, BitSequence * b) {
  built = true;
  assert(_seq!=NULL);
  if(bwt!=NULL) {
    delete bwt;
    bwt = NULL;
  }
  #ifdef VERBOSE
  cout << "Building the SA" << endl;
  #endif
  build_sa();
  #ifdef VERBOSE
  cout << "Done with the SA" << endl;
  #endif
  if(v!=NULL)
      for(uint i=0;i<=n;i++)
          v->setField(i,b->rank1(_sa[i]));
  #ifdef VERBOSE
  cout << "Building the BWT" << endl;
  #endif
  build_bwt();
  #ifdef VERBOSE
  cout << "Done with the BWT" << endl;
  #endif
  if(free_text) {
    delete [] _seq;
    _seq = NULL;
  }
  #ifdef VERBOSE
  cout << "Building the WT of the BWT" << endl;
  #endif
  bwt = new WaveletTree(_bwt, n+1, 
			new wt_coder_huff(_bwt, n+1,
					  new MapperNone()),
			new BitSequenceBuilderRRR(32), 
			new MapperNone());

  maxV = 0;
  for(uint i=0;i<n+1;i++)
    maxV = max(_bwt[i],maxV);
  maxV++;

  cout << " Max value: " << maxV << endl;
  occ = new uint[maxV+1];
  for(uint i=0;i<maxV+1;i++)
    occ[i]=0;

  for(uint i=0;i<n+1;i++)
    occ[_bwt[i]+1]++;

  for(uint i=1;i<=maxV;i++)
    occ[i] += occ[i-1];

  #ifdef VERBOSE
  cout << "Done with the WT" << endl;
  #endif
  delete [] _bwt;
  _bwt = NULL;
  sbuff = new uchar[samplepos+1];
  spos = (uchar)-(samplepos+1);
  return true;
}


void ssa::build_bwt() {
  assert(_seq!=NULL);
  if(_bwt!=NULL)
    delete [] _bwt;
  _bwt = new uint[n+2];
  for(uint i=0;i<n+1;i++) {
    if(_sa[i]==0) _bwt[i]=0;
    else _bwt[i] = _seq[_sa[i]-1];
  }
  uint j=0;
  uint * sampled_vector = new uint[uint_len(n+2,1)];
  pos_sample = new uint[(n+1)/samplepos+2];
  suff_sample = new uint[(n+1)/samplesuff+1];
  for(uint i=0;i<uint_len(n+1,1);i++) sampled_vector[i] = 0;
  for(uint i=0;i<n+1;i++) {
    if(_sa[i]%samplesuff==0) {
      suff_sample[j++]=_sa[i];
      bitset(sampled_vector,i);
    }
    if(_sa[i]%samplepos==0) {
      pos_sample[_sa[i]/samplepos]=i;
    }
  }
  pos_sample[n/samplepos+1]=pos_sample[0];
  bitset(sampled_vector,n+1);
  sampled = new BitSequenceRRR(sampled_vector,n+1,32);
  delete [] sampled_vector;
  delete [] _sa;
  _sa = NULL;
}

void ssa::build_sa() {
  assert(_seq!=NULL);
  if(_sa!=NULL)
    delete [] _sa;
  _sa = new uint[n+1];
  for(uint i=0;i<n+1;i++)
    _sa[i] = i;
  sort_sa(0,n);
  assert(_sa[0]==n);
  for(uint i=0;i<n;i++)
    assert(cmp(_sa[i],_sa[i+1])<=0);
}


uint ssa::locate(uchar * pattern, uint m, uint ** occs) {
  assert(m>0);
  assert(pattern!=NULL);
  assert(bwt!=NULL);
  ulong i=m-1;
  uint c = pattern[i]; 
  uint sp = occ[c];
  uint ep = occ[c+1]-1;
  while (sp<=ep && i>=1) {
    c = pattern[--i];
    //cout << "sp=" << sp << " ep=" << ep << endl;
    sp = occ[c]+bwt->rank(c,sp-1);
    ep = occ[c]+bwt->rank(c,ep)-1;
  }
  //cout << "*sp=" << sp << " *ep=" << ep << endl;
  if(sp<=ep) {
    uint matches = ep-sp+1;
    *occs = new uint[matches];
    uint i = sp;
    uint j,dist;
    size_t rank_tmp;
    while(i<=ep) {
      j = i;
      dist = 0;
      while(!sampled->access(j)) {
        c = bwt->access(j,rank_tmp);
        rank_tmp--;
        j = occ[c]+rank_tmp;
        dist++;
      }
      (*occs)[i-sp] = suff_sample[sampled->rank1(j)-1]+dist;
      i++;
    }
    return ep-sp+1;
  }
  else {
    *occs = NULL;
    return 0;
  }
}


uint ssa::count(uchar * pattern, uint m) {
  assert(m>0);
  assert(pattern!=NULL);
  assert(bwt!=NULL);
  ulong i=m-1;
  uint c = pattern[i]; 
  uint sp = occ[c];
  uint ep = occ[c+1]-1;
  while (sp<=ep && i>=1) {
    c = pattern[--i];
    //cout << "sp=" << sp << " ep=" << ep << endl;
    sp = occ[c]+bwt->rank(c,sp-1);
    ep = occ[c]+bwt->rank(c,ep)-1;
  }
  //cout << "*sp=" << sp << " *ep=" << ep << endl;
  if (sp<=ep) {
    return ep-sp+1;
  }
  else {
    return 0;
  }
}


uchar * ssa::rebuild() {
  uchar * text = new uchar[n];
  for(uint i=0;i<n;i++)
    text[i] = extract_pos(i);
  return text;
}


void ssa::fill_buffer(uint i, uint j) {
  assert(j>=i);
  assert(j<n);
  assert(bwt!=NULL);
  uint len = j-i+1;
  uint c;
    size_t rank_tmp;
  uint p = 0;
  if((j/samplepos+1) == ((n)/samplepos+1)) {
    p = pos_sample[0];
  }
  else {
    p = pos_sample[j/samplepos+1];
  }
  for(uint dist=0;dist<len;dist++) {
    c = bwt->access(p,rank_tmp);
    p = occ[c]+rank_tmp;
    sbuff[len-dist-1] = (uchar)(c-1);
  }
}


uchar ssa::extract_pos(uint k) {
  uint block = k/samplepos;
  if(block!=spos)
    fill_buffer(samplepos*block,min(samplepos*block+(samplepos-1),n));
  spos = block;
  return sbuff[samplepos-(samplepos*(block+1)-k)];
}


uchar * ssa::extract(uint i, uint j) {
  assert(j>=i);
  assert(j<n);
  assert(bwt!=NULL);
  uchar *buff = new uchar[j-i+1];
  for(uint k=j;k>=i;k--)
    buff[k-i] = extract_pos(k);
  return buff;
}


void ssa::sort_sa(uint ini, uint fin) {
  if(ini>=fin || ini>=(fin+1)) return;
  uint piv = ini;
  piv = pivot(ini,fin,piv);
  sort_sa(ini,piv-1);
  sort_sa(piv+1,fin);
}


int ssa::cmp(uint i, uint j) {
  while(i<n && j<n) {
    if(_seq[i]!=_seq[j])
      return (int)_seq[i]-_seq[j];
    i++; j++;
  }
  assert(i!=j);
  if(j<i) return -1;
  return 1;
}


void ssa::swap(uint i, uint j) {
  uint tmp = _sa[i];
  _sa[i]=_sa[j];
  _sa[j]=tmp;
}


uint ssa::pivot(uint ini, uint fin, uint piv) {
  swap(ini,piv);
  uint i=ini+1;
  while(i<=fin) {
    if(cmp(_sa[i],_sa[ini])<=0)
      i++;
    else {
      swap(i,fin); fin--;
    }
  }
  swap(fin,ini);
  return fin;
}
