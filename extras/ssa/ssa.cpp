
#include <ssa.h>

ssa_words::ssa_words(uint *text, uint n, bool free_text) {
  assert(n>0);

  // Initial values and default constructors
  this->n=n;
  this->n1=n+1;
  this->_seq = text;
  this->built = false;
  this->free_text=free_text;
  _sbb = new BitSequenceBuilderRG(20);
  _ssb = new SequenceBuilderWaveletTreeNoptrs(_sbb,new MapperNone());

  // Default sampling values
  samplepos = 64;
  samplesuff = 64;

  #ifdef VERBOSE
  cout << "ssa_words" << endl;
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


ssa_words::~ssa_words() {
  if(_seq!=NULL && free_text)
    delete [] _seq;
  if(_bwt!=NULL)
    delete [] _bwt;
  if(_ssb!=NULL)
    delete _ssb;
  if(bwt!=NULL)
    delete bwt;
  if(_sa!=NULL)
    delete [] _sa;
  if(_sbb!=NULL)
    delete _sbb;
  if(pos_sample!=NULL)
    delete [] pos_sample;
  if(suff_sample!=NULL)
    delete [] suff_sample;
  if(sampled!=NULL)
    delete sampled;
  delete [] sbuff;
}


bool ssa_words::save(FILE * fp) {
  bool ret = false;
  /*ret &= (1==fwrite(&n,sizeof(uint),1,fp));
  ret &= (1==fwrite(&sigma,sizeof(uint),1,fp));
  ret &= (1==fwrite(&samplepos,sizeof(uint),1,fp));
  ret &= (1==fwrite(&samplesuff,sizeof(uint),1,fp));
  ret &= !bwt->save(fp);
  ret &= !sampled->save(fp);
  ret &= ((2+n/samplepos)==fwrite(pos_sample,sizeof(uint),(2+n/samplepos),fp));
  ret &= ((1+n/samplesuff)==fwrite(suff_sample,sizeof(uint),(1+n/samplesuff),fp));*/
  return ret;
}


uint ssa_words::length() {
  return n;
}


ssa_words::ssa_words(FILE * fp, bool & error) {
  error = true;
  built = true;
  _sa = NULL;
  bwt = NULL;
  _bwt = NULL;
  sampled = NULL;
  pos_sample = NULL;
  suff_sample = NULL;

  _seq=NULL;
  _ssb=NULL;
  _sbb=NULL;

  /*error &= (1==fread(&n,sizeof(uint),1,fp));
  error &= (1==fread(&sigma,sizeof(uint),1,fp));
  error &= (1==fread(&samplepos,sizeof(uint),1,fp));
  sbuff = new uint[samplepos+1];
  spos = (uint)-(samplepos+1);
  error &= (1==fread(&samplesuff,sizeof(uint),1,fp));
  error &= (NULL != (bwt=static_sequence::load(fp)));
  error &= (NULL!=(sampled = static_bitsequence::load(fp)));
  pos_sample = new uint[(2+n/samplepos)];
  error &= ((2+n/samplepos)==fread(pos_sample,sizeof(uint),(2+n/samplepos),fp));
  suff_sample = new uint[(1+n/samplesuff)];
  error &= ((1+n/samplesuff)==fread(suff_sample,sizeof(uint),(1+n/samplesuff),fp));
  error = !error;*/
}


uint ssa_words::size() {
  uint size = bwt->getSize();
  size += sizeof(uint)*(2+n/samplepos);
  size += sizeof(uint)*(1+n/samplesuff);
  size += sizeof(ssa_words);
  size += sizeof(uint)*(1+samplepos);
  return size;
}


void ssa_words::print_stats() {
  cout << "ssa_words stats:" << endl;
  cout << "****************" << endl;
  cout << "Total space  : " << size() << endl;
  cout << endl;
  cout << " bwt         : " << bwt->getSize() << endl;
  cout << " pos sample  : " << sizeof(uint)*(2+n/samplepos) << endl;
  cout << " suff sample : " << sizeof(uint)*(1+n/samplesuff) << endl;
  cout << endl;
}


bool ssa_words::set_static_sequence_builder(SequenceBuilder *ssb) {
  if(built) return false;
  if(_ssb!=NULL) delete _ssb;
  _ssb = ssb;
  return true;
}


bool ssa_words::set_static_bitsequence_builder(BitSequenceBuilder * sbb) {
  if(built) return false;
  if(_sbb!=NULL) delete _sbb;
  _sbb=sbb;
  return true;
}


bool ssa_words::set_samplepos(uint sample) {
  if(built) return false;
  samplepos = sample;
  return true;
}


bool ssa_words::set_samplesuff(uint sample) {
  if(built) return false;
  samplesuff = sample;
  return true;
}


bool ssa_words::build_index() {
  built = true;
  assert(_seq!=NULL);
  assert(_ssb!=NULL);
  if(bwt!=NULL) {
    delete bwt;
    bwt = NULL;
  }
  #ifdef VERBOSE
  cout << "Building OCC" << endl;
  #endif
  #ifdef VERBOSE
  cout << "Done with OCC" << endl;
  #endif
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
  bwt = (_ssb->build(_bwt,n+1));

  uint maxV = 0;
  for(uint i=0;i<n+1;i++)
    maxV = max(_bwt[i],maxV);
  maxV++;

  cout << " Max value: " << maxV << endl;
  occ = new uint[maxV];
  for(uint i=0;i<maxV;i++)
    occ[i]=0;

  cout << " Testing rank and select" << endl;
  for(uint i=0;i<=n;i++) {
    occ[_bwt[i]]++;
  #ifdef TESTING
    assert(bwt->select(_bwt[i],occ[_bwt[i]])==i);
    assert(bwt->rank(_bwt[i],i)==occ[_bwt[i]]);
    uint r = 0; 
    assert(bwt->access(i,r)==_bwt[i]);
    if(r!=occ[_bwt[i]]-1) {
      cout << "r=" << r << " expected:" << occ[_bwt[i]]-1 << endl;
    }
  #endif
  }


  for(uint i=1;i<maxV;i++)
    occ[i] += occ[i-1];

  #ifdef VERBOSE
  cout << "Done with the WT" << endl;
  #endif
  delete [] _bwt;
  _bwt = NULL;
  delete _ssb;
  _ssb = NULL;
  delete _sbb;
  _sbb = NULL;
  sbuff = new uint[samplepos+1];
  spos = (uint)-(samplepos+1);
  return true;
}


void ssa_words::build_bwt() {
  assert(_seq!=NULL);
  assert(_sbb!=NULL);
  if(_bwt!=NULL)
    delete [] _bwt;
  _bwt = new uint[n+2];
  #ifdef VERBOSE
  cout << "Building the SA" << endl;
  #endif
  build_sa();
  #ifdef VERBOSE
  cout << "Done with the SA" << endl;
  #endif
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
  sampled = _sbb->build(sampled_vector,n+1);
  delete [] sampled_vector;
  delete [] _sa;
  _sa = NULL;
}


void ssa_words::build_sa() {
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


uint ssa_words::locate(uint * pattern, uint m, uint ** occs) {
  assert(m>0);
  assert(pattern!=NULL);
  assert(bwt!=NULL);
  uint c = pattern[m-1]+1; ulong i=m-1;
  uint sp = occ[c];
  c++;
  uint ep = occ[c]-1;
  c--;
  while (sp<=ep && i>=1) {
    c = pattern[--i]+1;
    //cout << "sp=" << sp << " ep=" << ep << endl;
    sp = occ[c]+bwt->rank(c,sp-1);
    ep = occ[c]+bwt->rank(c,ep)-1;
  }
  //cout << "* sp=" << sp << " ep=" << ep << endl;
  if(sp<=ep) {
    uint matches = ep-sp+1;
    *occs = new uint[matches];

    //*occs = new uint[matches];
    uint i = sp;
    uint j,dist;
    size_t rank_tmp;
    while(i<=ep) {
      j = i;
      dist = 0;
      while(!sampled->access(j)) {
        c = bwt->access(j,rank_tmp);
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


uint ssa_words::count(uint * pattern, uint m) {
  assert(m>0);
  assert(pattern!=NULL);
  assert(bwt!=NULL);
  uint c = pattern[m-1]+1; ulong i=m-1;
  uint sp = occ[c];
  c++;
  uint ep = occ[c]-1;
  c--;
  while (sp<=ep && i>=1) {
    c = pattern[--i]+1;
    sp = occ[c]+bwt->rank(c,sp-1);
    ep = occ[c]+bwt->rank(c,ep)-1;
  }
  if (sp<=ep) {
    return ep-sp+1;
  }
  else {
    return 0;
  }
}


uchar * ssa_words::rebuild() {
  uchar * text = new uchar[n];
  /*uint p = pos_sample[0];
  uint c =  bwt->access(p);
  p =  bwt->rankLessThan(c,n1)+bwt->rank(c,p)-1;
  for(int i=n-1;i>=0;i--) {
    uint tmp_rank;
    c = bwt->access(p,tmp_rank);
    p =  bwt->rankLessThan(c,n1)+tmp_rank;
    text[i] = c;
  }*/
  for(uint i=0;i<n;i++)
    text[i] = extract_pos(i);
  return text;
}


void ssa_words::fill_buffer(uint i, uint j) {
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


/*uint ssa_words::extract_pos(uint k) {
  uint block = k/samplepos;
  if(block!=spos)
    fill_buffer(samplepos*block,min(samplepos*block+(samplepos-1),n-1));
  spos = block;
  if(block==n/samplepos)
    return sbuff[n%samplepos-1-k];
  return sbuff[samplepos-1-k];
}*/

uchar ssa_words::extract_pos(uint k) {
  uint block = k/samplepos;
  if(block!=spos)
    fill_buffer(samplepos*block,min(samplepos*block+(samplepos-1),n));
  spos = block;
  /*if(block==n/samplepos)
    return sbuff[samplepos-(samplepos*(block+1)-k)-1];*/
  return sbuff[samplepos-(samplepos*(block+1)-k)];
}


uchar * ssa_words::extract(uint i, uint j) {
  assert(j>=i);
  assert(j<n);
  assert(bwt!=NULL);
  uchar *buff = new uchar[j-i+1];
  for(uint k=j;k>=i;k--)
    buff[k-i] = extract_pos(k);
  return buff;
}


void ssa_words::sort_sa(uint ini, uint fin) {
  if(ini>=fin || ini>=(fin+1)) return;
  uint piv = ini;
  piv = pivot(ini,fin,piv);
  sort_sa(ini,piv-1);
  sort_sa(piv+1,fin);
}


int ssa_words::cmp(uint i, uint j) {
  while(i<n && j<n) {
    if(_seq[i]!=_seq[j])
      return (int)_seq[i]-_seq[j];
    i++; j++;
  }
  assert(i!=j);
  if(j<i) return -1;
  return 1;
}


void ssa_words::swap(uint i, uint j) {
  uint tmp = _sa[i];
  _sa[i]=_sa[j];
  _sa[j]=tmp;
}


uint ssa_words::pivot(uint ini, uint fin, uint piv) {
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
