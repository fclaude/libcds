
#include "ssa.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <wt_coder.h>

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
  for(uint i=0;i<n;i++) text[i]++;
  text[n]=0;

  ssa * _ssa = new ssa(text,n);
  Mapper * am = new MapperNone();
  wt_coder * wc = new wt_coder_huff(text,n+1,am);
  SequenceBuilder * ssb = new SequenceBuilderWaveletTree(wc,sbb,am);
  _ssa->set_static_sequence_builder(ssb);
  _ssa->set_samplepos(8);
  _ssa->set_samplesuff(8);
  _ssa->build_index();

  cout << "Index size: " << _ssa->size() << endl;

  FILE * fp = fopen(argv[2],"w");
  cout << "Save: " << _ssa->save(fp) << endl;
  fclose(fp);

  delete sbb;
  delete _ssa;
  delete [] text;

  return 0;
}
