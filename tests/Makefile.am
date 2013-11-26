# This Makefile is used to build and execute the tests when "make check"
# is run.

AM_CXXFLAGS = -I../include/
LDADD = ../src/libcds.la

check_PROGRAMS = testHuffman testSequence testQuantile testBitSequence testArray toArray2 toArray timeSequence

testHuffman_SOURCES = testHuffman.cpp
testSequence_SOURCES = testSequence.cpp
testQuantile_SOURCES = testQuantile.cpp
testBitSequence_SOURCES = testBitSequence.cpp
testArray_SOURCES = testArray.cpp
toArray2_SOURCES = toArray2.cpp
toArray_SOURCES = toArray.cpp
timeSequence_SOURCES = timeSequence.cpp

# FIXME:
# The testBitSequence needs arguments to run. A simple shell wrapper is
# needed for this but not sure what are good testing arguments. Perhaps
# this would work:
# #!/bin/sh
# SEED=`date +%s`
# LENGTH=`expr $SEED % 2048`
# DENSITY=`expr $LENGTH / 2`
# echo .lib/lt-testBitSequence $SEED $LENGTH $DENSITY
# .lib/lt-testBitSequence $SEED $LENGTH $DENSITY
# That still needs some cleanup to make it portable and the exec path is
# probably wrong.
# FIXME: Actually most of the test programs have this problem. Might be
#        a better idea to write them to choose default arguments.
TESTS = $(check_PROGRAMS)

CLEANFILES = bitsequence.tmp
