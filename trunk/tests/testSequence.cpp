
#include <cstdlib>

#include <libcdsBasics.h>
#include <BitSequence.h>
#include <Mapper.h>
#include <Sequence.h>

using namespace std;
using namespace cds_static;


void testSequence(Array & a, Sequence & bs) {
    ofstream outfs("sequence.tmp");
    bs.save(outfs);
    outfs.close();
    ifstream infs("sequence.tmp");
    Sequence * seq = Sequence::load(infs);
    infs.close();
    uint maxv = a.getMax();
    size_t count[maxv+1];
    for(size_t i=0;i<=maxv;i++)
        count[i] = 0;
    for(size_t i=0;i<a.getLength();i++) {
        count[a[i]]++;
        for(uint j=a[i];j<=a[i];j++) { 
            if(seq->rank(j,i)!=count[j]) {
                cerr << "ERROR RANK " << endl;
                cerr << " Rank result: " << bs.rank(j,i) << " count=" << count[j] << endl;
                cerr << " symbol=" << j << " position=" << i << endl;
                exit(-1);
            }
        }
        if(seq->select(a[i],count[a[i]])!=i) {
            cerr << "ERROR SELECT " << endl;
            cerr << "a[i]=" << a[i] << " maxv=" << maxv << endl;
            cerr << "bs.select=" << bs.select(a[i],count[a[i]]) << " i=" << i << endl;
            exit(-2);
        }
        if(a[i]!=seq->access(i)) {
            cerr << "ERROR ACCESS" << endl;
            exit(-3);
        }
    }
    delete seq;
}

int main(int argc, char ** argv) {

  if(argc!=4) {
    cout << "Checks the array class generating <length> elements between 0 and <maxv> using <seed> as seed for the numbers generation" << endl << endl;
    cout << "usage: " << argv[0] << " <seed> <length> <maxv>" << endl;
    return 0;
  }

  srand(transform(string(argv[1])));
  uint len = transform(string(argv[2]));
  uint maxv = transform(string(argv[3]));

  //cout << "maxv = " << maxv << endl;
  //cout << "len  = " << len << endl;

  Array a(len,maxv);
  for(uint i=0;i<len;i++) {
    a.setField(i,rand()%maxv);
  }
  
  //BitmapsSequence  bs(a,new MapperNone(),new BitSequenceBuilderRRR(33));
  //testSequence(a, bs);

  Mapper * mapper = new MapperCont(a, BitSequenceBuilderRG(20));
  Mapper * mapper2 = new MapperNone();
  mapper->use();
  mapper2->use();
  cout << "Test 1 : Wavelet tree with pointers" << endl;
  WaveletTree wt1(a,new wt_coder_huff(a, mapper),new BitSequenceBuilderRG(20), mapper);
  cout << "bs.size() = " << wt1.getSize() << endl;
  testSequence(a, wt1);

  cout << "Test 2 : Wavelet tree without pointers" << endl;
  WaveletTreeNoptrs wt3(a, new BitSequenceBuilderRRR(32), mapper);
  cout << "bs.size() = " << wt3.getSize() << endl;
  testSequence(a, wt3);
  mapper->unuse();
  mapper2->unuse();
  return 0;
}

