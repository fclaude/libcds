
#include <cstdlib>

#include <libcdsBasics.h>
#include <BitSequence.h>
#include <Mapper.h>
#include <Sequence.h>

using namespace std;
using namespace cds_static;


int uintcmp(const void* a,const void* b)
{
    return (*(const uint32_t*)a - *(const uint32_t*)b);
}

void testQuantileWT(Array& a,WaveletTree& wt)
{
    size_t N = 500;
    /* select 500 rand ranges and perform quantile queries */
    while(N) {
        size_t start = rand() % a.getLength();
        size_t len = (rand() % (a.getLength() - start)) % 10;

        /* copy and sort the array */
        uint32_t* A = new uint32_t[len+1];
        for(size_t i=0;i<=len;i++) {
            A[i] = a[start+i];
        };
        qsort(A,len+1,sizeof(uint32_t),uintcmp);

        /* test */
        size_t accum = 0;
        size_t quantile = 1;
        while(accum <= len) {
            pair<uint,size_t> qf = wt.quantile_freq(start,start+len,quantile);
            for(size_t i=0;i<qf.second;i++) {
                if(A[accum+i] != qf.first) {
                    fprintf(stderr,"ERROR!\n");
                    exit(EXIT_FAILURE);
                }
            }
            accum += qf.second;
            quantile += qf.second;
        }

        /* cleanup */
        N--;
        delete [] A;
    }

}

void testQuantileWTNPTR(Array& a,WaveletTreeNoptrs& wt)
{
    size_t N = 500;
    /* select 500 rand ranges and perform quantile queries */
    while(N) {
        size_t start = rand() % a.getLength();
        size_t len = rand() % (a.getLength() - start);

        /* copy and sort the array */
        uint32_t* A = new uint32_t[len+1];
        for(size_t i=0;i<=len;i++) {
            A[i] = a[start+i];
        };
        qsort(A,len+1,sizeof(uint32_t),uintcmp);


        /* test */
        size_t accum = 0;
        size_t quantile = 1;
        while(accum <= len) {
            pair<uint,size_t> qf = wt.quantile_freq(start,start+len,quantile);
            for(size_t i=0;i<qf.second;i++) {
                if(A[accum+i] != qf.first) {
                    fprintf(stderr,"ERROR!\n");
                    exit(EXIT_FAILURE);
                }
            }
            accum += qf.second;
            quantile += qf.second;
        }

        /* cleanup */
        N--;
        delete [] A;
    }
}

int main(int argc, char ** argv) {

  size_t len = rand() % 200; 
  uint maxv = 50 + rand() % 50;

  Array a(len,maxv);
  for(uint i=0;i<len;i++) {
    a.setField(i,rand()%maxv);
  }
  
  Mapper * mapper = new MapperCont(a, BitSequenceBuilderRG(20));
  Mapper * mapper2 = new MapperNone();
  mapper->use();
  mapper2->use();
  cout << "Test 1 : Wavelet tree with pointers" << endl;
  /*WaveletTree wt1(a,new wt_coder_huff(a, mapper),new BitSequenceBuilderDArray(), mapper);*/
  WaveletTree wt1(a,new wt_coder_binary(a, mapper),new BitSequenceBuilderRG(20), mapper);
  cout << "bs.size() = " << wt1.getSize() << endl;

  testQuantileWT(a, wt1);

  cout << "Test 2 : Wavelet tree without pointers" << endl;
  WaveletTreeNoptrs wt3(a, new BitSequenceBuilderRRR(32), mapper);
  cout << "bs.size() = " << wt3.getSize() << endl;
  testQuantileWTNPTR(a, wt3);
  mapper->unuse();
  mapper2->unuse();

  fprintf(stdout,"ALL OK\n");

  return 0;
}

