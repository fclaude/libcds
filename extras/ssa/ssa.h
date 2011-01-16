
#ifndef SSA_WORDS_H
#define SSA_WORDS_H

#include <SequenceBuilder.h>
#include <Sequence.h>
#include <BitSequenceBuilder.h>
#include <BitSequence.h>

#include <Mapper.h>
#include <algorithm>

using namespace std;
using namespace cds_static;

#define SSA_HDR 29

class ssa
{
  public:
    ssa(uchar * seq, uint n, bool free_text=false);
    ssa(ifstream & fp);
    ~ssa();

    bool set_static_sequence_builder(SequenceBuilder * ssb);
    bool set_static_bitsequence_builder(BitSequenceBuilder * sbb);
    bool set_samplepos(uint sample);
    bool set_samplesuff(uint sample);

    bool build_index(Array * a=NULL, BitSequence * b=NULL);

    uint size();
    void print_stats();
    uint length();

    uint locate(uchar * pattern, uint m, uint ** occ);
    uint count(uchar * pattern, uint m);
    uchar * extract(uint i, uint j);
    uchar extract_pos(uint i);

    uchar * rebuild();
    void save(ofstream & fp);


  protected:
    uint n,n1;
    uint sigma;
    Sequence * bwt;
    BitSequence * sampled;
    uint samplepos;
    uint samplesuff;
    uint * pos_sample;
    uint * suff_sample;
    uchar * sbuff;
    uint spos;
    uint * occ;
    uint maxV;

    bool built;
    bool free_text;
    uchar * _seq;
    uint * _bwt;
    uint * _sa;
    SequenceBuilder * _ssb;
    BitSequenceBuilder * _sbb;

    void build_bwt();
    void build_sa();
    void sort_sa(uint ini, uint fin);
    int cmp(uint i, uint j);
    void swap(uint i, uint j);
    uint pivot(uint ini, uint fin, uint piv);
    void fill_buffer(uint i, uint j);

};
#endif
