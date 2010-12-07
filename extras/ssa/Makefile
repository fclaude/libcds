CPP=g++


#CPPFLAGS=-m32 -g -Wall -pg 
#CPPFLAGS=-g3 -Wall -O2 -DVERBOSE
#CPPFLAGS=-O9 -Wall -DNDEBUG 
#CPPFLAGS=-O9 -m32 -Wall -DNDEBUG -DVERBOSE



INCLUDES=-I../../trunk/includes/ -I.
LIB=../../trunk/lib/libcds.a
OBJECTS=ssa.o 
BINS=test_count build_index

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

all: $(OBJECTS) $(BINS)

stats:
	@echo
	@echo " COMPILING SSA_WORDS"
	@echo " ###################"
	@echo "  * Compiler flags: $(CPPFLAGS)"
	@echo "  * Include dirs: $(INCLUDES)"
	@echo
	@echo

clean:
	@echo " [LCD] Cleaning libcds library"
	@make  --no-print-directory -C libcds clean
	@echo " [LCD] Done cleaning libcds library"
	@echo " [CLN] Removing object files"
	@rm -f $(OBJECTS) index.a $(BINS)

test_count: 
	@echo " [BLD] Building test_count"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o test_count test_count.cpp $(OBJECTS) $(LIB)

build_index: 
	@echo " [BLD] Building build_index"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o build_index build_index.cpp $(OBJECTS) $(LIB)

