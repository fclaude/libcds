
#include <cstdlib>

#include <libcdsBasics.h>
#include <Array.h>
#include <cppUtils.h>

using namespace std;
using namespace cds_utils;

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
  uint * array = new uint[len];
  uint acc=0;
  for(uint i=0;i<len;i++) {
    array[i] = rand()%(maxv+1);
    a.setField(i,array[i]);
    if(array[i]!=a[i]) {
      cerr << "ERROR FOR ARRAY AT POSITION " << i << endl;
      return -1;
    }
    acc += a[i];
    //cout << tmp << a[i];
  }
  //cout << "Test done." << endl;
  //cout << "acc = " << acc <<  endl;

  delete [] array;
  return 0;
}

