CPP=g++


#CPPFLAGS=-g3 -Wall -DVERBOSE
#CPPFLAGS=-O9 -Wall -DNDEBUG 
CPPFLAGS=-O9 -Wall -DVERBOSE

INCLUDES=-I../../includes/ -I.
LIB=../../lib/libcds.a
OBJECTS=ssa.o 
BINS=test_count build_index dump_bwt

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

all: clean stats $(OBJECTS) $(BINS)

stats:
	@echo
	@echo " COMPILING SSA"
	@echo " ###################"
	@echo "  * Compiler flags: $(CPPFLAGS)"
	@echo "  * Include dirs: $(INCLUDES)"
	@echo
	@echo

clean:
	@echo " [CLN] Removing object files"
	@rm -f $(OBJECTS) $(BINS)

test_count: 
	@echo " [BLD] Building test_count"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o test_count test_count.cpp $(OBJECTS) $(LIB)

dump_bwt: 
	@echo " [BLD] Building dump_bwt"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o dump_bwt dump_bwt.cpp $(OBJECTS) $(LIB)

build_index: 
	@echo " [BLD] Building build_index"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o build_index build_index.cpp $(OBJECTS) $(LIB)

