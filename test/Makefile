# test/Makefile
# Run tests in this directory.

# Default target, runs all tests.
all:

# Remove built-in implicit rules.
.SUFFIXES:

# C compiler and linker.
#
# TODO: Get this from ./configure.
CC := gcc
CXX := g++

# C compilation options.
CFLAGS := -g -Wall

# smflex produces scanners that define 'yyunput' and 'input' even when
# they are not used.
CFLAGS += -Wno-unused-function

# C++ compilation options.
CXXFLAGS := $(CFLAGS)

# smflex executable.
SMFLEX := ../smflex

# Run smflex on one file.
.PRECIOUS: out/%.yy.c
out/%.yy.c: %.lex
	@mkdir -p out
	$(SMFLEX) -o$@ $<
	python3 ../check-line-directives.py $@

# Run smflex on outfile.lex, avoiding use of -o option.
out/outfile-specified.yy.c: outfile.lex $(SMFLEX)
	@mkdir -p out
	$(SMFLEX) $<

# Compile a C file in out/.
#
# If the file name contains "c-int-", then add -DYY_ENABLE_DEBUG_LOG_CALL
# to the compilation line to enable relevant diagnostic prinouts.
#
.PRECIOUS: out/%.o
out/%.o: out/%.c
	@if echo "$*" | grep --quiet "c-int-"; then \
	  echo $(CC) -o$@ -c $(CFLAGS) -DYY_ENABLE_DEBUG_LOG_CALL $<; \
	  $(CC) -o$@ -c $(CFLAGS) -DYY_ENABLE_DEBUG_LOG_CALL $<; \
	else \
	  echo $(CC) -o$@ -c $(CFLAGS) $<; \
	  $(CC) -o$@ -c $(CFLAGS) $<; \
	fi

# Compile a C file in out/ as if it were C++.
.PRECIOUS: out/%.c_as_cc.o
out/%.c_as_cc.o: out/%.c
	$(CXX) -o$@ -xc++ -c $(CXXFLAGS) $<

# Compile a C file in test/.
out/%.o: %.c
	@mkdir -p out
	$(CC) -o$@ -c $(CFLAGS) $<

# Run smflex on one file that generates a C++-interface scanner.
.PRECIOUS: out/%.yy.cc
out/%.yy.cc: %.lexpp $(SMFLEX)
	@mkdir -p out
	$(SMFLEX) -o$@ $<
	python3 ../check-line-directives.py $@

# Compile C++ file in out/.
.PRECIOUS: out/%.oo
out/%.oo: out/%.cc
	@if echo "$*" | grep --quiet "cpp-int-"; then \
	  echo $(CXX) -o$@ -c $(CXXFLAGS) -DYY_ENABLE_DEBUG_LOG_CALL $<; \
	  $(CXX) -o$@ -c $(CXXFLAGS) -DYY_ENABLE_DEBUG_LOG_CALL $<; \
	else \
	  echo $(CXX) -o$@ -c $(CXXFLAGS) $<; \
	  $(CXX) -o$@ -c $(CXXFLAGS) $<; \
	fi

# Link C object files in out/ to make an executable.
.PRECIOUS: out/%.exe
out/%.exe: out/%.yy.o
	$(CC) -o$@ $(CFLAGS) $^

# Link C-compiled-as-C++ object file.
.PRECIOUS: out/%.c_as_cc.exe
out/%.c_as_cc.exe: out/%.yy.c_as_cc.o
	$(CXX) -o$@ $(CXXFLAGS) $^

# username is special because it uses testmain.
out/username.exe: out/testmain.o

# Use C++ linking for *.oo files.
out/%.exe: out/%.yy.oo
	$(CXX) -o$@ $(CXXFLAGS) $^

# Run one test, producing an "ok" file to mark success.
out/%.ok: out/%.exe %.input %.expect
	rm -f $@
	out/$*.exe < $*.input > out/$*.actual
	@echo diff -b -u $*.expect out/$*.actual
	@if diff -b -u $*.expect out/$*.actual; then true; else \
	  echo ""; \
	  echo "The actual output is different from the expected output."; \
	  echo "If the actual is correct, update expected by running:"; \
	  echo "  $ ./update-expect.sh $*"; \
	  exit 2; \
	fi
	touch $@

# Check that what is in the manual matches what is in the tests.
out/%.matches: % ../smflex.html
	@mkdir -p out
	cat ../smflex.html | \
	  sed -n -e '/-- BEGIN example: $* --/,/-- END example --/p' | \
	  grep -v -- '-- BEGIN example' | \
	  grep -v -- '-- END example' | \
	  sed -e 's/^    //' \
	      -e 's/&lt;/</g' \
	      -e 's/&gt;/>/g' \
	      -e 's/&amp;/&/g' > out/$*.manual
	diff -u $* out/$*.manual
	touch $@

# Tests to run, in sorted ("env LANG=C sort") order.
all: out/c-int-always.ok
all: out/c-int-batch.ok
all: out/c-int-batch-never.ok
all: out/c-int-default.ok
all: out/c-int-interactive.ok
all: out/c-int-never.ok
all: out/c-int-set-int.ok
all: out/cpp-int-always.ok
all: out/cpp-int-batch.ok
all: out/cpp-int-default.ok
all: out/cpp-int-interactive.ok
all: out/cpp-int-never.ok
all: out/cpp-manual-example.lexpp.matches
all: out/cpp-manual-example.ok
all: out/cpp-prefix-yywrap-yyclass.ok
all: out/cpp-prefix-yywrap.ok
all: out/cpp-prefix.ok
all: out/cppscan-main.ok
all: out/cppscan-yywrap.ok
all: out/cppscan.ok
all: out/outfile-specified.ok
all: out/username-main.c_as_cc.ok
all: out/username-main.ok
all: out/username-yywrap.ok
all: out/username.ok
all: out/vert-bar-action.ok

# These tests read a second file.
username-yywrap.ok: username-yywrap.input2
cppscan-yywrap.ok: cppscan-yywrap.input2

# Command to print a message at the end.
all:
	@echo 'All tests in test/ passed.  (Use "make clean" there to reset.)'

# Remove all test outputs.
clean:
	rm -rf out

# EOF
