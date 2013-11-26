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


# Automake include for the ax_prog_doxygen autoconf macro:

## --------------------------------- ##
## Format-independent Doxygen rules. ##
## --------------------------------- ##

if DX_COND_doc

## ------------------------------- ##
## Rules specific for HTML output. ##
## ------------------------------- ##

if DX_COND_html

DX_CLEAN_HTML = @DX_DOCDIR@/html

endif DX_COND_html

## ------------------------------ ##
## Rules specific for CHM output. ##
## ------------------------------ ##

if DX_COND_chm

DX_CLEAN_CHM = @DX_DOCDIR@/chm

if DX_COND_chi

DX_CLEAN_CHI = @DX_DOCDIR@/@PACKAGE@.chi

endif DX_COND_chi

endif DX_COND_chm

## ------------------------------ ##
## Rules specific for MAN output. ##
## ------------------------------ ##

if DX_COND_man

DX_CLEAN_MAN = @DX_DOCDIR@/man

endif DX_COND_man

## ------------------------------ ##
## Rules specific for RTF output. ##
## ------------------------------ ##

if DX_COND_rtf

DX_CLEAN_RTF = @DX_DOCDIR@/rtf

endif DX_COND_rtf

## ------------------------------ ##
## Rules specific for XML output. ##
## ------------------------------ ##

if DX_COND_xml

DX_CLEAN_XML = @DX_DOCDIR@/xml

endif DX_COND_xml

## ----------------------------- ##
## Rules specific for PS output. ##
## ----------------------------- ##

if DX_COND_ps

DX_CLEAN_PS = @DX_DOCDIR@/@PACKAGE@.ps

DX_PS_GOAL = doxygen-ps

doxygen-ps: @DX_DOCDIR@/@PACKAGE@.ps

@DX_DOCDIR@/@PACKAGE@.ps: @DX_DOCDIR@/@PACKAGE@.tag
	cd @DX_DOCDIR@/latex; \
	rm -f *.aux *.toc *.idx *.ind *.ilg *.log *.out; \
	$(DX_LATEX) refman.tex; \
	$(MAKEINDEX_PATH) refman.idx; \
	$(DX_LATEX) refman.tex; \
	countdown=5; \
	while $(DX_EGREP) 'Rerun (LaTeX|to get cross-references right)' \
	refman.log > /dev/null 2>&1 \
	&& test $$countdown -gt 0; do \
	$(DX_LATEX) refman.tex; \
	countdown=`expr $$countdown - 1`; \
	done; \
	$(DX_DVIPS) -o ../@PACKAGE@.ps refman.dvi

endif DX_COND_ps

## ------------------------------ ##
## Rules specific for PDF output. ##
## ------------------------------ ##

if DX_COND_pdf

DX_CLEAN_PDF = @DX_DOCDIR@/@PACKAGE@.pdf

DX_PDF_GOAL = doxygen-pdf

doxygen-pdf: @DX_DOCDIR@/@PACKAGE@.pdf

@DX_DOCDIR@/@PACKAGE@.pdf: @DX_DOCDIR@/@PACKAGE@.tag
cd @DX_DOCDIR@/latex; \
rm -f *.aux *.toc *.idx *.ind *.ilg *.log *.out; \
$(DX_PDFLATEX) refman.tex; \
$(DX_MAKEINDEX) refman.idx; \
$(DX_PDFLATEX) refman.tex; \
countdown=5; \
while $(DX_EGREP) 'Rerun (LaTeX|to get cross-references right)' \
refman.log > /dev/null 2>&1 \
&& test $$countdown -gt 0; do \
$(DX_PDFLATEX) refman.tex; \
countdown=`expr $$countdown - 1`; \
done; \
mv refman.pdf ../@PACKAGE@.pdf

endif DX_COND_pdf

## ------------------------------------------------- ##
## Rules specific for LaTeX (shared for PS and PDF). ##
## ------------------------------------------------- ##

if DX_COND_latex

DX_CLEAN_LATEX = @DX_DOCDIR@/latex

endif DX_COND_latex

.PHONY: doxygen-run doxygen-doc $(DX_PS_GOAL) $(DX_PDF_GOAL)

.INTERMEDIATE: doxygen-run $(DX_PS_GOAL) $(DX_PDF_GOAL)

doxygen-run: @DX_DOCDIR@/@PACKAGE@.tag

doxygen-doc: doxygen-run $(DX_PS_GOAL) $(DX_PDF_GOAL)

@DX_DOCDIR@/@PACKAGE@.tag: $(DX_CONFIG) $(pkginclude_HEADERS)
	rm -rf @DX_DOCDIR@
	$(DX_ENV) $(DX_DOXYGEN) $(srcdir)/$(DX_CONFIG)

DX_CLEANFILES = \
    @DX_DOCDIR@/@PACKAGE@.tag \
    -r \
    $(DX_CLEAN_HTML) \
    $(DX_CLEAN_CHM) \
    $(DX_CLEAN_CHI) \
    $(DX_CLEAN_MAN) \
    $(DX_CLEAN_RTF) \
    $(DX_CLEAN_XML) \
    $(DX_CLEAN_PS) \
    $(DX_CLEAN_PDF) \
    $(DX_CLEAN_LATEX)

CLEANFILES = $(DX_CLEANFILES)

cleanfile-local:
	rm -rf docs/

endif DX_COND_doc
