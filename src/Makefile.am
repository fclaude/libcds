# This Makefile builds the libcds library.

AM_CPPFLAGS = -I../include/

lib_LTLIBRARIES = libcds.la
libcds_la_LDFLAGS = -version-info $(CDS_SO_VERSION)
libcds_la_SOURCES = \
    static/bitsequence/BitSequenceBuilderDArray.cpp \
    static/bitsequence/BitSequenceBuilderRG.cpp \
    static/bitsequence/BitSequenceBuilderRRR.cpp \
    static/bitsequence/BitSequenceBuilderSDArray.cpp \
    static/bitsequence/BitSequence.cpp \
    static/bitsequence/BitSequenceDArray.cpp \
    static/bitsequence/BitSequenceRG.cpp \
    static/bitsequence/BitSequenceRRR.cpp \
    static/bitsequence/BitSequenceSDArray.cpp \
    static/bitsequence/sdarraySadakane.cpp \
    static/bitsequence/TableOffsetRRR.cpp \
    static/coders/huff.cpp \
    static/coders/HuffmanCoder.cpp \
    static/mapper/MapperCont.cpp \
    static/mapper/Mapper.cpp \
    static/mapper/MapperNone.cpp \
    static/mapper/MapperRev.cpp \
    static/permutation/perm.cpp \
    static/permutation/PermutationBuilderMRRR.cpp \
    static/permutation/PermutationBuilderWT.cpp \
    static/permutation/Permutation.cpp \
    static/permutation/PermutationMRRR.cpp \
    static/permutation/PermutationWT.cpp \
    static/sequence/BitmapsSequence.cpp \
    static/sequence/SequenceAlphPart.cpp \
    static/sequence/SequenceBuilderAlphPart.cpp \
    static/sequence/SequenceBuilderGMRChunk.cpp \
    static/sequence/SequenceBuilderGMR.cpp \
    static/sequence/SequenceBuilderStr.cpp \
    static/sequence/SequenceBuilderWaveletMatrix.cpp \
    static/sequence/SequenceBuilderWaveletTree.cpp \
    static/sequence/SequenceBuilderWaveletTreeNoptrs.cpp \
    static/sequence/SequenceBuilderWaveletTreeNoptrsS.cpp \
    static/sequence/Sequence.cpp \
    static/sequence/SequenceGMRChunk.cpp \
    static/sequence/SequenceGMR.cpp \
    static/sequence/WaveletMatrix.cpp \
    static/sequence/WaveletTree.cpp \
    static/sequence/WaveletTreeNoptrs.cpp \
    static/sequence/WaveletTreeNoptrsS.cpp \
    static/sequence/wt_coder_binary.cpp \
    static/sequence/wt_coder.cpp \
    static/sequence/wt_coder_huff.cpp \
    static/sequence/wt_node.cpp \
    static/sequence/wt_node_internal.cpp \
    static/sequence/wt_node_leaf.cpp \
    utils/Array.cpp \
    utils/cppUtils.cpp \
    utils/libcdsBitString.cpp \
    utils/timing.cpp
