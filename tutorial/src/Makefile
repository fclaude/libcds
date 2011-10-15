
CPP=g++

CPPFLAGS=-g3 -O0 -I../includes/ 
#CPPFLAGS=-O3 -Wall -DNDEBUG -I../includes/
INCS=-I../../includes/
LIB=../../lib/libcds.a

OBJECTS= ArrayExample.o ArrayExample2.o BitSequenceRGExample.o BitSequenceRRRExample.o BitSequenceSDArrayExample.o SequenceWaveletTreeExample.o SequenceGMRExample.o SequenceAlphPartExample.o
BIN= ArrayExample ArrayExample2 BitSequenceRGExample BitSequenceRRRExample BitSequenceSDArrayExample SequenceWaveletTreeExample SequenceGMRExample SequenceAlphPartExample

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCS) -c $< -o $@

all: clean $(OBJECTS) $(BIN)
	@echo " [MSG] Done compiling tests"
	@echo " [FLG] $(CPPFLAGS)"

ArrayExample:
	@echo " [LNK] Building ArrayExample"
	@$(CPP) $(CPPFLAGS) -o ArrayExample ArrayExample.o $(LIB)

ArrayExample2:
	@echo " [LNK] Building ArrayExample2"
	@$(CPP) $(CPPFLAGS) -o ArrayExample2 ArrayExample2.o $(LIB)

BitSequenceRGExample:
	@echo " [LNK] Building BitSequenceRGExample"
	@$(CPP) $(CPPFLAGS) -o BitSequenceRGExample BitSequenceRGExample.o $(LIB)

BitSequenceRRRExample:
	@echo " [LNK] Building BitSequenceRRRExample"
	@$(CPP) $(CPPFLAGS) -o BitSequenceRRRExample BitSequenceRRRExample.o $(LIB)

BitSequenceSDArrayExample:
	@echo " [LNK] Building BitSequenceSDArrayExample"
	@$(CPP) $(CPPFLAGS) -o BitSequenceSDArrayExample BitSequenceSDArrayExample.o $(LIB)

SequenceWaveletTreeExample:
	@echo " [LNK] Building SequenceWaveletTreeExample"
	@$(CPP) $(CPPFLAGS) -o SequenceWaveletTreeExample SequenceWaveletTreeExample.o $(LIB)

SequenceGMRExample:
	@echo " [LNK] Building SequenceGMRExample"
	@$(CPP) $(CPPFLAGS) -o SequenceGMRExample SequenceGMRExample.o $(LIB)

SequenceAlphPartExample:
	@echo " [LNK] Building SequenceAlphPartExample"
	@$(CPP) $(CPPFLAGS) -o SequenceAlphPartExample SequenceAlphPartExample.o $(LIB)

clean:
	@echo " [CLN] Cleaning object and binary files"
	@rm -f $(OBJECTS) $(BIN)
