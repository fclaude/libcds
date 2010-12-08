
CPP=g++

INCL=-I../includes/

## OBJECTS FROM cds_utils PACKAGE
UTILS_OBJ=utils/Array.o utils/BitString.o utils/cppUtils.o utils/timing.o

# OBJECTS FROM cds_static PACKAGE
STATIC_MAPPER_OBJ=static/mapper/Mapper.o static/mapper/MapperNone.o static/mapper/MapperCont.o
STATIC_BITSEQ_OBJ= static/bitsequence/BitSequence.o static/bitsequence/BitSequenceRG.o static/bitsequence/BitSequenceSDArray.o static/bitsequence/sdarraySadakane.o static/bitsequence/BitSequenceDArray.o static/bitsequence/BitSequenceRRR.o static/bitsequence/BitSequenceBuilderRG.o static/bitsequence/BitSequenceBuilderRRR.o static/bitsequence/BitSequenceBuilderSDArray.o static/bitsequence/BitSequenceBuilderDArray.o static/bitsequence/TableOffsetRRR.o
STATIC_CODER_OBJ=static/coders/huff.o static/coders/HuffmanCoder.o
STATIC_SEQ_OBJ=static/sequence/Sequence.o static/sequence/BitmapsSequence.o static/sequence/wt_coder.o static/sequence/wt_coder_binary.o static/sequence/wt_coder_huff.o static/sequence/wt_node.o static/sequence/wt_node_internal.o static/sequence/wt_node_leaf.o static/sequence/WaveletTree.o static/sequence/WaveletTreeNoptrs.o static/sequence/SequenceGMR.o static/sequence/SequenceGMRChunk.o static/sequence/SequenceBuilderWaveletTreeNoptrs.o static/sequence/SequenceBuilderWaveletTree.o static/sequence/SequenceBuilderGMR.o static/sequence/SequenceBuilderGMRChunk.o static/sequence/SequenceAlphPart.o static/sequence/SequenceBuilderStr.o static/sequence/SequenceBuilderAlphPart.o

STATIC_PERM_OBJ=static/permutation/Permutation.o static/permutation/PermutationMRRR.o static/permutation/PermutationBuilderMRRR.o static/permutation/perm.o
STATIC_TEXTIN_OBJ= static/textindex/mmap.o static/textindex/comparray4.o static/textindex/qsufsort.o static/textindex/suftest3.test.o  static/textindex/TextIndex.o static/textindex/TextIndexCSA.o 
STATIC_TREE_OBJ= static/suffixtree/LCP.o static/suffixtree/LCP_naive.o static/suffixtree/LCP_Sad.o static/suffixtree/LCP_FMN.o static/suffixtree/RMQ_succinct.o static/suffixtree/LCP_PT.o static/suffixtree/LCP_PhiSpare.o static/suffixtree/factorization.o  static/suffixtree/LCP_DAC.o static/suffixtree/factorization_var.o static/suffixtree/LCP_DAC_VAR.o static/suffixtree/NPR.o static/suffixtree/NSV.o  static/suffixtree/PSV.o static/suffixtree/RMQ_succinct_lcp.o  static/suffixtree/NPR_FMN.o static/suffixtree/NPR_CN.o static/suffixtree/SuffixTree.o static/suffixtree/SuffixTreeY.o


STATIC_OBJ=$(STATIC_MAPPER_OBJ) $(STATIC_BITSEQ_OBJ) $(STATIC_CODER_OBJ) $(STATIC_SEQ_OBJ) $(STATIC_PERM_OBJ) $(STATIC_TEXTIN_OBJ) $(STATIC_TREE_OBJ)

OBJECTS=$(UTILS_OBJ) $(STATIC_OBJ)

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCL) -c $< -o $@

all: pre lib 

clean:
	@echo " [CLN] Removing object files"
	@rm -f $(OBJECTS)

lib: pre $(OBJECTS)
	@echo " [LIB] Packing the object files"
	@ar rcs ../lib/libcds.a $(OBJECTS) 

pre:
	@echo " [HDR] Populating the includes folder"
	@echo " [CFG] CPPFLAGS = $(CPPFLAGS)"
	@make --no-print-directory -C utils
	@make --no-print-directory -C static/mapper
	@make --no-print-directory -C static/bitsequence
	@make --no-print-directory -C static/coders
	@make --no-print-directory -C static/sequence
	@make --no-print-directory -C static/permutation
	@make --no-print-directory -C static/textindex
	@make --no-print-directory -C static/suffixtree

