
CPP=g++

CPPFLAGS=-g3 -Wall -O0 -I../includes/ -std=c++0x
#CPPFLAGS=-O3 -Wall -DNDEBUG -I../includes/
LIB=../lib/libcds.a

OBJECTS=testArray.o testBitSequence.o testSequence.o testHuffman.o
BIN=testArray testBitSequence testSequence testHuffman

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) -c $< -o $@

all: clean $(OBJECTS) $(BIN)
	@echo " [MSG] Done compiling tests"

testHuffman:
	@echo " [LNK] Building testHuffman"
	@$(CPP) $(CPPFLAGS) -o testHuffman testHuffman.o $(LIB)

testSequence:
	@echo " [LNK] Building testSequence"
	@$(CPP) $(CPPFLAGS) -o testSequence testSequence.o $(LIB)

testBitSequence:
	@echo " [LNK] Building testBitSequence"
	@$(CPP) $(CPPFLAGS) -o testBitSequence testBitSequence.o $(LIB)

testArray:
	@echo " [LNK] Building testArray"
	@$(CPP) $(CPPFLAGS) -o testArray testArray.o $(LIB)

clean:
	@echo " [CLN] Cleaning object and binary files"
	@rm -f $(OBJECTS) $(BIN)
