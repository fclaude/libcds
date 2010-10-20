
#include <cstdlib>

#include <libcdsBasics.h>
#include <Coder.h>

using namespace std;
using namespace cds_utils;
using namespace cds_static;


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
    cout << "s[" << i << "]=" << a[i] << endl;
  }
  
  HuffmanCoder * hc = new HuffmanCoder(a);
  cout << "Max length: " << hc->maxLength() << endl;

  for(size_t i=0;i<len;i++) {
      size_t ptr = 0;
      uint stream[10];
      cout << "coding " << a[i] << ": ";
      ptr = hc->encode(a[i],stream,ptr);
      for(size_t k=0;k<ptr;k++)
          cout << bitget(stream,k);
      cout << endl;
  }
  return 0;
}

