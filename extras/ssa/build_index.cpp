
#include "ssa.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char ** argv) {
  if(argc!=3) {
    cout << "usage: " << argv[0] << " <int-file> <index-file>" << endl;
    return 0;
  }

  fstream input(argv[1],ios::in | ios::binary);
  if(!input.is_open()) {
    cout << "Error opening file: " << argv[1] << endl;
    return -1;
  }

  input.seekg(0,ios::end);
  uint n=input.tellg();
  uchar * text = new uchar[n+1];

  input.seekg(0,ios::beg);
  input.read((char*)text,sizeof(uchar)*n);
  input.close();
  //for(uint i=0;i<n;i++) text[i]++;
  text[n]=0;

  ssa * _ssa = new ssa(text,n);
  Mapper * am = new MapperNone();
  wt_coder * wc = new wt_coder_huff(text,n+1,am);
  BitSequenceBuilder * sbb = new BitSequenceBuilderRRR(32);
  _ssa->set_static_bitsequence_builder(sbb);
  SequenceBuilder * ssb = new SequenceBuilderWaveletTree(sbb,am,wc);
  _ssa->set_static_sequence_builder(ssb);
  _ssa->set_samplepos(32);
  _ssa->set_samplesuff(32);
  _ssa->build_index();
  _ssa->print_stats();

  cout << "Index size: " << _ssa->size() << endl;

  ofstream fp(argv[2]);
  _ssa->save(fp);
  fp.close();

  delete sbb;
  delete _ssa;
  delete [] text;

  return 0;
}

