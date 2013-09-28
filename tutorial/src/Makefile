
CPP=g++

CPPFLAGS=-g3 -O0 -I../includes/ 
INCS=-I../../includes/
LIB=../../lib/libcds.a

OBJECTS=lz78.o lz78dec.o lz78dec-shell.o
BIN=lz78 lz78dec lz78dec-shell

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCS) -c $< -o $@

all: clean $(OBJECTS) $(BIN)
	@echo " [MSG] Done compiling tests"
	@echo " [FLG] $(CPPFLAGS)"

lz78:
	@echo " [LNK] Building lz78"
	@$(CPP) $(CPPFLAGS) -o lz78 lz78.o $(LIB)

lz78dec:
	@echo " [LNK] Building lz78dec"
	@$(CPP) $(CPPFLAGS) -o lz78dec lz78dec.o $(LIB)

lz78dec-shell:
	@echo " [LNK] Building lz78dec-shell"
	@$(CPP) $(CPPFLAGS) -o lz78dec-shell lz78dec-shell.o $(LIB)

clean:
	@echo " [CLN] Cleaning object and binary files"
	@rm -f $(OBJECTS) $(BIN)
