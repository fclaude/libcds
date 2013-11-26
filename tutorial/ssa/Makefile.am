# Build the ssa tutorial programs.

AM_CXXFLAGS = -I../../include/
LDADD = ../../src/libcds.la
noinst_PROGRAMS = test_count build_index dump_bwt

test_count_SOURCES = test_count.cpp ssa.cpp

dump_bwt_SOURCES = dump_bwt.cpp ssa.cpp

build_index_SOURCES = build_index.cpp ssa.cpp

