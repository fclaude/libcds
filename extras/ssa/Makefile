CPP=g++


#CPPFLAGS=-m32 -g -Wall -pg 
CPPFLAGS=-g3 -Wall -DVERBOSE
#CPPFLAGS=-O9 -Wall -DNDEBUG 
CPPFLAGS=-O9 -Wall -DVERBOSE



INCLUDES=-I../../trunk/includes/ -I.
LIB=../../trunk/lib/libcds.a
OBJECTS=ssa.o 
BINS=test_count build_index create_collection index_collection 

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

build_index: 
	@echo " [BLD] Building build_index"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o build_index build_index.cpp $(OBJECTS) $(LIB)

create_collection:
	@echo " [BLD] Building create_collection"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o create_collection create_collection.cpp $(OBJECTS) $(LIB)

index_collection:
	@echo " [BLD] Building index_collection"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o index_collection index_collection.cpp $(OBJECTS) $(LIB)

