
#include <cstdlib>

#include <libcdsBasics.h>
#include <BitString.h>
#include <BitSequence.h>
#include <BitSequenceRG.h>
#include <BitSequenceRRR.h>
#include <BitSequenceSDArray.h>

using namespace std;
using namespace cds_utils;
using namespace cds_static;

BitSequence * saveLoad(BitSequence * bs) {
    ofstream ofs("bitsequence.tmp");
    bs->save(ofs);
    ofs.close();
    ifstream ifs("bitsequence.tmp");
    BitSequence * ret = BitSequence::load(ifs);
    ifs.close();
    return ret;
}

bool testBitSequence(BitString & a, BitSequence * bs) {
  size_t rank0SoFar = 0;
  size_t rank1SoFar = 0;
  for(size_t i=0; i<a.getLength(); i++) {
    if(a[i]) {
        rank1SoFar++;
        if(bs->select1(rank1SoFar)!=i) {
            cerr << "SELECT1 ERROR " << i << endl;
            return false;
        }
        if(i>0 && bs->selectNext1(i)!=i) {
            cout << "i=" << i << "sn=" << bs->selectNext1(i) << endl;
           cerr << "SELECTNEXT1 ERROR" << endl;
           return false;
        }
    } else {
        rank0SoFar++;
        if(bs->select0(rank0SoFar)!=i) {
           cerr << "SELECT0 ERROR" << endl;
           return false;
        }
        if(i>0 && bs->selectNext0(i)!=i) {
           cerr << "SELECTNEXT0 ERROR" << endl;
           return false;
        }
    }
    if(bs->rank1(i)!=rank1SoFar) 
        return false;
    if(bs->rank0(i)!=rank0SoFar)
        return false;
    if(bs->access(i)!=a[i])
        return false;
  }
  return true;
}

int main(int argc, char ** argv) {

  if(argc!=4) {
    cout << "Checks the bitsequence classes generating <length> bitmaps with density <dens> using <seed> as seed for the numbers generation" << endl << endl;
    cout << "usage: " << argv[0] << " <seed> <length> <dens>" << endl;
    return 0;
  }

  srand(transform(string(argv[1])));
  uint len = transform(string(argv[2]));
  uint dens = transform(string(argv[3]));

  uint act_dens = 0;
  BitString a(len);
  while(act_dens < dens) {
      size_t pos = rand()%len;
      if(!a[pos]) {
          act_dens++;
          a.setBit(pos,true);
      }
  }

  BitSequenceRG bsRG(a,20);
  BitSequence * s = saveLoad(&bsRG);
  if(!testBitSequence(a,s)) {
      cerr << "ERROR TESTING BitSequenceRG" << endl;
      return -1;
  }
	cout << "RG OK\n" << endl; 
  delete (BitSequenceRG *)s;

	BitSequenceDArray bsDArray(a);
	s = saveLoad(&bsDArray);
	if(!testBitSequence(a,s)) {
		cerr << "ERROR TESTING BitSequenceDArray" << endl;
		return -1;
	}
	cout << "DArray OK\n" << endl; 
	delete (BitSequenceDArray *)s;

	BitSequenceSDArray bsSDArray(a);
  s = saveLoad(&bsSDArray);
  if(!testBitSequence(a,s)) {
      cerr << "ERROR TESTING BitSequenceSDArray" << endl;
      return -1;
  }
	cout << "SDArray OK\n" << endl;
  delete (BitSequenceSDArray *)s;

	BitSequenceRRR bsRRR(a,33);
  s = saveLoad(&bsRRR);
  if(!testBitSequence(a,s)) {
      cerr << "ERROR TESTING BitSequenceRRR" << endl;
      return -1;
  }
	cout << "RRR OK\n" << endl;
  delete (BitSequenceRRR *)s;
  return 0;
}

