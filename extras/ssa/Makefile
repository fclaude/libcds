CPP=g++


#CPPFLAGS=-m32 -g -Wall -pg 
#CPPFLAGS=-g3 -Wall -DVERBOSE
#CPPFLAGS=-O9 -Wall -DNDEBUG 
CPPFLAGS=-O9 -Wall -DNDEBUG -DVERBOSE



INCLUDES=-I../../trunk/includes/ -I.
LIB=../../trunk/lib/libcds.a
OBJECTS=ssa.o 
BINS=test_count 

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

