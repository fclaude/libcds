
# CPPFLAGS=-g3 -Wall -O0 -fPIC
#-DVERBOSE
CPPFLAGS=-O9 -Wall -DNDEBUG -fPIC

all: libcompact tests

doc:
	@echo " [DOC] Generating documentation"
	@doxygen

libcompact:
	@echo " [MSG] Entering directory src"
	@CPPFLAGS="$(CPPFLAGS)" make --no-print-directory -C src

tests: libcompact
	@echo " [MSG] Entering directory tests"
	@CPPFLAGS="$(CPPFLAGS)" make --no-print-directory -C tests

shared_lib: libcompact
	@echo " [C++] Compiling shared library"
	@g++ -shared $(CPPFLAGS) -w -o lib/libcds.so ./lib/libcds.a
	
shared_lib_install:
	@cp ./lib/libcds.so /usr/lib
	@cp ./lib/libcds.a /usr/lib
	@echo " [MSG] Library copied to /usr/lib"
	@mkdir -p /usr/include/libcds
	@cp -rf ./includes /usr/include/libcds
	@echo " [MSG] Headers copied to /usr/include/libcds"

clean:
	@echo " [MSG] Entering directory src"
	@make --no-print-directory -C src clean
	@echo " [MSG] Entering directory tests"
	@make --no-print-directory -C tests clean
	@echo " [CLN] Cleaning docs folder"
	@rm -rf docs/*
	@touch docs/delete_me
	@echo " [CLN] Cleaning lib folder"
	@rm -f lib/*
	@touch lib/delete_me
	@echo " [CLN] Cleaning includes folder"
	@rm -f includes/*
	@touch includes/delete_me

