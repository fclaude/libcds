# This Makefile is the base Makefile of the project and is used to
# build the entire project.

ACLOCAL_AMFLAGS = -I m4

include_HEADERS = \
    include/Array.h \
    include/BitmapsSequence.h \
    include/BitSequenceBuilderDArray.h \
    include/BitSequenceBuilder.h \
    include/BitSequenceBuilderRG.h \
    include/BitSequenceBuilderRRR.h \
    include/BitSequenceBuilderSDArray.h \
    include/BitSequenceDArray.h \
    include/BitSequence.h \
    include/BitSequenceRG.h \
    include/BitSequenceRRR.h \
    include/BitSequenceSDArray.h \
    include/Coder.h \
    include/cppUtils.h \
    include/huff.h \
    include/HuffmanCoder.h \
    include/libcdsBasics.h \
    include/libcdsBitString.h \
    include/libcdsSDArray.h \
    include/libcdsTrees.h \
    include/MapperCont.h \
    include/Mapper.h \
    include/MapperNone.h \
    include/MapperRev.h \
    include/perm.h \
    include/PermutationBuilder.h \
    include/PermutationBuilderMRRR.h \
    include/PermutationBuilderWT.h \
    include/Permutation.h \
    include/PermutationMRRR.h \
    include/PermutationWT.h \
    include/sdarraySadakane.h \
    include/SequenceAlphPart.h \
    include/SequenceBuilderAlphPart.h \
    include/SequenceBuilderGMRChunk.h \
    include/SequenceBuilderGMR.h \
    include/SequenceBuilder.h \
    include/SequenceBuilderStr.h \
    include/SequenceBuilderWaveletMatrix.h \
    include/SequenceBuilderWaveletTree.h \
    include/SequenceBuilderWaveletTreeNoptrs.h \
    include/SequenceBuilderWaveletTreeNoptrsS.h \
    include/SequenceGMRChunk.h \
    include/SequenceGMR.h \
    include/Sequence.h \
    include/TableOffsetRRR.h \
    include/timing.h \
    include/WaveletMatrix.h \
    include/WaveletTree.h \
    include/WaveletTreeNoptrs.h \
    include/WaveletTreeNoptrsS.h \
    include/wt_coder_binary.h \
    include/wt_coder.h \
    include/wt_coder_huff.h \
    include/wt_node.h \
    include/wt_node_internal.h \
    include/wt_node_leaf.h

SUBDIRS = src tests tutorial/src tutorial/ssa

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cds.pc
