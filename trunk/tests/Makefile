
CPP=g++

CPPFLAGS=-g3 -Wall -O0 -I../includes/ 
#CPPFLAGS=-O3 -Wall -DNDEBUG -I../includes/
LIB=../lib/libcds.a

OBJECTS= testArray.o testBitSequence.o testSequence.o testHuffman.o testTextIndex.o testLCP.o testNPR.o testCST.o
BIN= testArray testBitSequence testSequence testHuffman testTextIndex testLCP testNPR testCST

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

testTextIndex:
	 @echo " [LNK] Building testTextIndex"
	 @$(CPP) $(CPPFLAGS) -o testTextIndex testTextIndex.o $(LIB)

testLCP:
	@echo " [LNK] Building testLCP"
	@$(CPP) $(CPPFLAGS) -o testLCP testLCP.o $(LIB)

testNPR:
	@echo " [LNK] Building testNPR"
	@$(CPP) $(CPPFLAGS) -o testNPR testNPR.o $(LIB)

testCST:
	@echo " [LNK] Building testCST"
	@$(CPP) $(CPPFLAGS) -o testCST testCST.o $(LIB)



clean:
	@echo " [CLN] Cleaning object and binary files"
	@rm -f $(OBJECTS) $(BIN)
