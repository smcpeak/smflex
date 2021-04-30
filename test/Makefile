# test/Makefile
# Run tests in this directory.

# Default target, runs all tests.
all:

# C compiler and linker.
#
# TODO: Get this from ./configure.
CC := gcc
CXX := g++

# C compilation options.
CFLAGS := -g -Wall

# C++ compilation options.  This needs another directory for FlexLexer.h.
CXXFLAGS := $(CFLAGS) -I..

# Flex produces scanners that define 'yyunput' and 'input' even when
# they are not used.
CFLAGS += -Wno-unused-function

# Run flex on one file.
.PRECIOUS: out/%.yy.c
out/%.yy.c: %.lex
	@mkdir -p out
	../flex -o$@ $<

# Compile a C file in out/.
.PRECIOUS: out/%.o
out/%.o: out/%.c
	$(CC) -o$@ -c $(CFLAGS) $<

# Compile a C file in test/.
out/%.o: %.c
	@mkdir -p out
	$(CC) -o$@ -c $(CFLAGS) $<

# cppscan is special because it should write to a .cc file.
out/cppscan.yy.cc: cppscan.lex
	@mkdir -p out
	../flex -o$@ $<

# Compile C++ file in out/.
out/%.o: out/%.cc
	$(CXX) -o$@ -c $(CXXFLAGS) $<

# Link C object files in out/ to make an executable.
.PRECIOUS: out/%.exe
out/%.exe: out/%.yy.o
	$(CC) -o$@ $(CFLAGS) $^

# username is special because uses testmain.
out/username.exe: out/testmain.o

# cppscan is special because it needs C++ linking.
out/cppscan.exe: out/cppscan.yy.o
	$(CXX) -o$@ $(CXXFLAGS) $^

# Run one test, producing an "ok" file to mark success.
out/%.ok: out/%.exe %.input %.expect
	out/$*.exe < $*.input > out/$*.actual
	diff -b -u $*.expect out/$*.actual
	touch $@

# Tests to run.
all: out/username.ok
all: out/username-yywrap.ok
all: out/username-main.ok
all: out/cppscan.ok

# This test reads a second file.
username-yywrap.ok: username-yywrap.input2

# Command to print a message at the end.
all:
	@echo 'All tests in test/ passed.  (Use "make clean" there to reset.)'

# Remove all test outputs.
clean:
	rm -rf out

# EOF
