
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


class ssa_words
{
  public:
    ssa_words(uint * seq, uint n, bool free_text=false);
    ssa_words(FILE *fp, bool & error);
    ~ssa_words();

    bool set_static_sequence_builder(SequenceBuilder * ssb);
    bool set_static_bitsequence_builder(BitSequenceBuilder * sbb);
    bool set_samplepos(uint sample);
    bool set_samplesuff(uint sample);

    bool build_index();

    uint size();
    void print_stats();
    uint length();

    uint locate(uint * pattern, uint m, uint ** occ);
    uint count(uint * pattern, uint m);
    uchar * extract(uint i, uint j);
    uchar extract_pos(uint i);

    uchar * rebuild();
    bool save(FILE * fp);

  protected:
    uint n,n1;
    uint sigma;
    Sequence * bwt;
    BitSequence * sampled;
    uint samplepos;
    uint samplesuff;
    uint * pos_sample;
    uint * suff_sample;
    uint * sbuff;
    uint spos;
    uint * occ;

    bool built;
    bool free_text;
    uint * _seq;
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
