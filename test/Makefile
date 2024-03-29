# test/Makefile
# Run tests in this directory.

# These tests are safe to run with "make -jN" for N being about the
# number of CPU cores.  However, on Windows, you have to redirect the
# output to a file (or possibly use --output-sync; I have not tried
# that) in that case since there is a bug in conhost.exe that will cause
# it to hang when multiple processes write to the same terminal at the
# same time.  This affects at least cygwin mintty.exe and xterm.exe
# since both use conhost.exe.

# Default target, runs all tests.
all:

# Let "check" be an alias for "all".
check: all

# Remove built-in implicit rules.
.SUFFIXES:

# Delete a target when its recipe fails.
.DELETE_ON_ERROR:

# Do not delete intermediate files.
.SECONDARY:

# Pull in definitions from main build.
include ../config.mk
-include ../personal.mk

# C++ compiler and linker.
#
# TODO: Arrange for config.mk to set this.
CXX := g++

# C compilation options for correctness test executables.
#
# This refers to several variables that can be set in personal.mk:
#
#   CSTD      -std=c89
#   CXXSTD    -std=c++98
#   PEDANTIC  -pedantic
#   WERROR    -Werror
#
CFLAGS := -g -Wall -I. $(CSTD) $(PEDANTIC) $(WERROR)

# C++ compilation options.
#
# CXXWARNINGS can be set in personal.mk to enable warnings that are
# specific to C++, principally -Wsuggest-override.
CXXFLAGS := -g -Wall -I. $(CXXSTD) $(PEDANTIC) $(WERROR) $(CXXWARNINGS)

# smflex executable.
SMFLEX := ../smflex

# Options to pass to smflex for additional diagnostic reporting.
# Specifically, this causes the "performance report" to be printed,
# which consists of one-line commentary on certain configurations.
# I have this enabled both to exercise the option and to check that
# what it is saying is sensible.
SMFLEX_OPTS := -p -p

# Which option combination to test.  See table of XXX_OPS variables,
# below in the performance test section.  For the moment, the only
# way to activate 'full' is by specifying TEST_CONFIG=full on the
# 'make' command line.
TEST_CONFIG := default
#TEST_CONFIG := full

# I use the <(...) syntax in the 'paste' command below, so I need bash
# specifically.
SHELL := /bin/bash

PYTHON3 := python3


# Run smflex on one file.
out/%.yy.c out/%.yy.h: %.lex $(SMFLEX)
	@mkdir -p out
	$(SMFLEX) $(SMFLEX_OPTS) $($(TEST_CONFIG)_OPS) -oout/$*.yy.c $<
	$(PYTHON3) ./check-line-directives.py out/$*.yy.c
	$(PYTHON3) ./check-line-directives.py out/$*.yy.h

# Run smflex on outfile.lex, avoiding use of -o option.
out/outfile-specified.yy.c: outfile.lex $(SMFLEX)
	@mkdir -p out
	$(SMFLEX) $(SMFLEX_OPTS) $<


# Compilation flags to add for memory tests.
MEM_TEST_FLAGS := -DYY_ENABLE_DEBUG_LOG_CALL -DYY_ENABLE_DEBUG_LOG_MEM -DYY_ALLOCATOR_FAILURE_INJECTION

# Flags to get fileno.
FILENO_FLAGS := -D_POSIX_C_SOURCE=200809L

# Extra compilation flags to add for certain tests.
CFLAGS_FOR_c_int_batch                      := -DYY_ENABLE_DEBUG_LOG_CALL
CFLAGS_FOR_c_int_default                    := -DYY_ENABLE_DEBUG_LOG_CALL
CFLAGS_FOR_c_int_interactive                := -DYY_ENABLE_DEBUG_LOG_CALL
CFLAGS_FOR_c_int_interactive_set_int_0      := -DYY_ENABLE_DEBUG_LOG_CALL
CFLAGS_FOR_c_int_interactive_set_int_isatty := -DYY_ENABLE_DEBUG_LOG_CALL $(FILENO_FLAGS)
CFLAGS_FOR_c_int_set_int_0                  := -DYY_ENABLE_DEBUG_LOG_CALL
CFLAGS_FOR_c_int_set_int_1                  := -DYY_ENABLE_DEBUG_LOG_CALL
CFLAGS_FOR_c_int_set_int_isatty             := -DYY_ENABLE_DEBUG_LOG_CALL $(FILENO_FLAGS)
CFLAGS_FOR_large_read_buffer := $(MEM_TEST_FLAGS)
CFLAGS_FOR_large_token       := $(MEM_TEST_FLAGS)
CFLAGS_FOR_scan_string_only  := $(MEM_TEST_FLAGS)
CFLAGS_FOR_two_buffers       := $(MEM_TEST_FLAGS)
CFLAGS_FOR_username_read     := $(FILENO_FLAGS)

CXXFLAGS_FOR_cpp_int_batch       := -DYY_ENABLE_DEBUG_LOG_CALL
CXXFLAGS_FOR_cpp_int_default     := -DYY_ENABLE_DEBUG_LOG_CALL
CXXFLAGS_FOR_cpp_int_set_int_1   := -DYY_ENABLE_DEBUG_LOG_CALL
CXXFLAGS_FOR_cpp_int_interactive := -DYY_ENABLE_DEBUG_LOG_CALL
CXXFLAGS_FOR_error_handling_cpp  := $(MEM_TEST_FLAGS)
CXXFLAGS_FOR_large_token_cpp     := $(MEM_TEST_FLAGS)
CXXFLAGS_FOR_two_buffers_cpp     := $(MEM_TEST_FLAGS)

# Given the name of a source file without its extension, get the
# C[XX]FLAGS to use when compiling that file.  This first strips any
# ".yy" extension, then maps hyphens and dots to underscores, then looks
# for a 'C[XX]FLAGS_FOR_XXX' that matches, yielding the empty string if
# there is none.
GET_CFLAGS = $(CFLAGS_FOR_$(subst -,_,$(subst .,_,$(1:.yy=))))
GET_CXXFLAGS = $(CXXFLAGS_FOR_$(subst -,_,$(subst .,_,$(1:.yy=))))

# Compile a C file in out/.
#
# If the file name contains "c-int-", then add -DYY_ENABLE_DEBUG_LOG_CALL
# to the compilation line to enable relevant diagnostic prinouts.
#
out/%.o: out/%.c
	$(CC) -o$@ -c $(CFLAGS) $(CPPFLAGS) $(call GET_CFLAGS,$*) $<

# Compile a C file in out/ as if it were C++.
out/%.c_as_cc.o: out/%.c
	$(CXX) -o$@ -xc++ -c $(CXXFLAGS) $(CPPFLAGS) $<

# Compile a C file in test/.
out/%.o: %.c
	@mkdir -p out
	$(CC) -o$@ -c $(CFLAGS) $<

# Run smflex on one file that generates a C++-interface scanner.
out/%.yy.cc out/%.yy.h: %.lexpp $(SMFLEX)
	@mkdir -p out
	$(SMFLEX) $(SMFLEX_OPTS) $($(TEST_CONFIG)_OPS) -oout/$*.yy.cc $<
	$(PYTHON3) ./check-line-directives.py out/$*.yy.cc
	$(PYTHON3) ./check-line-directives.py out/$*.yy.h

# Compile C++ file in out/.
out/%.oo: out/%.cc
	$(CXX) -o$@ -c $(CXXFLAGS) $(CPPFLAGS) $(call GET_CXXFLAGS,$*) $<

# Link C object files in out/ to make an executable.
out/%.exe: out/%.yy.o
	$(CC) -o$@ $(CFLAGS) $^

# Link C-compiled-as-C++ object file.
out/%.c_as_cc.exe: out/%.yy.c_as_cc.o
	$(CXX) -o$@ $(CXXFLAGS) $^

# Make sure we run smflex before trying to compile this.
out/main-for-username.o: out/username.yy.c

# username is special because it uses main-for-username.
out/username.exe: out/main-for-username.o

# Dependencies on extra headers.
out/yy-lex-params.yy.o: extra-lex-state.h
out/yy-lex-params-cpp.yy.oo: extra-lex-state.h

# Use C++ linking for *.oo files.
out/%.exe: out/%.yy.oo
	$(CXX) -o$@ $(CXXFLAGS) $^

# Prefix test executable.
PREFIX_MODULES :=
PREFIX_MODULES += out/prefix1-c.yy.c
PREFIX_MODULES += out/prefix2-c.yy.c
PREFIX_MODULES += out/prefix3-cpp.yy.cc
PREFIX_MODULES += out/prefix4-cpp.yy.cc
out/prefix-main.exe: prefix-main.cc $(PREFIX_MODULES)
	$(CXX) -o$@ $(CXXFLAGS) $^


# Run one test, producing an "ok" file to mark success.
#
# The "actual" output file is the stdout from the scanner program,
# followed by the stderr (if any).  This should be reliable and
# portable, although we lose sensitivity to interleaving.
#
# If UPDATE_EXPECT=1, then automatically update the expected output
# to match the actual output.  Be sure to review the changes with
# 'git diff' before checking in!
#
out/%.ok: out/%.exe %.input %.expect
	rm -f $@
	out/$*.exe < $*.input > out/$*.actual 2>out/$*.stderr
	cat out/$*.stderr >> out/$*.actual
	@echo diff -b -u $*.expect out/$*.actual
ifeq ($(UPDATE_EXPECT),1)
	@if diff -b -u $*.expect out/$*.actual; then true; else \
	  echo ""; \
	  echo "The actual output is different from the expected output."; \
	  echo "Fixing it automatically due to UPDATE_EXPECT=1."; \
	  echo "./update-expect.sh $*"; \
	  ./update-expect.sh $*; \
	fi
else
	@if diff -b -u $*.expect out/$*.actual; then true; else \
	  echo ""; \
	  echo "The actual output is different from the expected output."; \
	  echo "If the actual is correct, update expected by running:"; \
	  echo "  $ ./update-expect.sh $*"; \
	  echo "Or, re-run tests with UPDATE_EXPECT=1 to update them all."; \
	  exit 2; \
	fi
endif
	touch $@

# If 'make' thinks it needs an expect file, just make an empty one so
# the test can proceed and then complain.
%.expect:
	touch $@

# Same for an input file.
%.input:
	touch $@

# Special rule for the test that calls getenv so it does not actually
# depend on the environment.
out/username-getenv.ok: out/username-getenv.exe username-getenv.input username-getenv.expect
	env USER=USERNAME out/username-getenv.exe < username-getenv.input > out/username-getenv.actual
	echo diff -b -u username-getenv.expect out/username-getenv.actual
	touch $@

# Check that what is in the manual matches what is in the tests.
all: out/matching-examples.ok
out/matching-examples.ok: ../smflex.html check-doc-examples.py
	@mkdir -p out
	$(PYTHON3) ./check-doc-examples.py $<
	touch $@

# Test the -L (no #line directive) option.
out/c-lexer1-no-lines.yy.c: c-lexer1.lex $(SMFLEX)
	@mkdir -p $(dir $@)
	$(SMFLEX) $(SMFLEX_OPTS) -L -o$@ $<
	if grep '#line' $@; then \
	  echo "There are not supposed to be any #line directives in that file."; \
	  exit 2; \
	fi

# Tests to run in both "default" and "full" configurations.
all-df: out/begin-special.ok
all-df: out/c-int-batch.ok
all-df: out/c-int-default.ok
all-d : out/c-int-interactive.ok
all-d : out/c-int-interactive-set-int-0.ok
all-d : out/c-int-interactive-set-int-isatty.ok
all-df: out/c-int-set-int-0.ok
all-df: out/c-int-set-int-1.ok
all-df: out/c-int-set-int-isatty.ok
all-df: out/c-lexer1.ok
all-df: out/c-lexer1-no-lines.ok
all-df: out/c-prefix.ok
all-df: out/c-prefix-validated.ok
all-df: out/case-insensitive.ok
all-df: out/continued-action2.ok
all-df: out/count-actions.ok
all-df: out/cpp-int-batch.ok
all-df: out/cpp-int-default.ok
all-df: out/cpp-int-set-int-1.ok
all-d : out/cpp-int-interactive.ok
all-df: out/cpp-manual-example.ok
all-df: out/cpp-prefix-yywrap-yyclass.ok
all-df: out/cpp-prefix-yywrap2-yyclass.ok
all-df: out/cpp-prefix-yywrap.ok
all-df: out/cpp-prefix.ok
all-df: out/cppscan-full.ok
all-df: out/cppscan-jacobson.ok
all-df: out/cppscan-main.ok
all-df: out/cppscan-yywrap.ok
all-df: out/cppscan.ok
all-df: out/comments.ok
all-df: out/count-lines-chars.ok
all-df: out/discard-c-comments.ok
all-df: out/discard-c-comments2.ok
all-df: out/discard-ml-comments.ok
all-df: out/discard-ml-comments-cpp.ok
all-df: out/discard-ml-comments-top-state.ok
all-df: out/elsa-lexer.ok
all-df: out/error-handling.ok
all-df: out/error-handling-cpp.ok
all-df: out/expect-floats.ok
all-df: out/gather-c-string.ok
all-df: out/gather-c-string2.ok
all-df: out/html-link-validator.ok
all-df: out/input-buffers.ok
all-df: out/input-buffers-lineno.ok
all-df: out/input-buffers-lineno-cpp.ok
all-df: out/input-buffers-with-read.ok
all-df: out/input-c-comments.ok
all-df: out/large-read-buffer.ok
all-df: out/large-token.ok
all-df: out/large-token-cpp.ok
all-df: out/noreject-frob.ok
all-df: out/normalize-whitespace.ok
all-df: out/notrailing-context.ok
all-df: out/noyylineno.ok
all-df: out/option-debug.ok
all-df: out/option-debug-cpp.ok
all-df: out/outfile-specified.ok
all-df: out/prefix-main.ok
all-df: out/read-character-c-comments.ok
all-df: out/read-unread-character.ok
all-d : out/reject-abcd.ok
all-d : out/reject-frob.ok
all-df: out/toy-pascal.ok
all-df: out/scan-bytes-only.ok
all-df: out/scan-large-string.ok
all-df: out/scan-string-etc.ok
all-df: out/scan-string-only.ok
all-df: out/set-bol.ok
all-df: out/smflex-after-ws-line.ok
all-df: out/stack-api.ok
all-d : out/trailing-context.ok
all-df: out/two-buffers.ok
all-df: out/two-buffers-cpp.ok
all-df: out/unput-parenthesized.ok
all-d : out/username-full.ok
all-df: out/username-getenv.ok
all-d : out/username-jacobson.ok
all-d : out/username-jacobson-prefix.ok
all-df: out/username-main.c_as_cc.ok
all-df: out/username-main.ok
all-df: out/username-read.ok
all-df: out/username-yywrap.ok
all-df: out/username.ok
all-df: out/vert-bar-action.ok
all-df: out/yy-input-redefined.ok
all-df: out/yy-lex-name.ok
all-df: out/yy-lex-name-prefix.ok
all-df: out/yy-lex-name-prefix-cpp.ok
all-df: out/yy-lex-params.ok
all-df: out/yy-lex-params-cpp.ok
all-df: out/yy-set-start-state-special.ok
all-df: out/yy-wrap-function.ok
all-df: out/yyinput-c-comments.ok
all-d : out/yyless-foobar.ok
all-d : out/yyless-foobar-s3.ok
all-d : out/yylineno.ok
all-df: out/yym-read-character.ok
all-df: out/yym-scan-string-etc.ok
all-df: out/yym-set-bol.ok
all-df: out/yym-set-start-state.ok
all-df: out/yym-wrap.ok
all-d : out/yymore-mega-kludge.ok
all-df: out/yymore-yyless.ok
all-df: out/yymore-yyless-cpp.ok
all-df: out/yymore-yytext.ok
all-df: out/zap-me.ok

# Add the right set of tests to 'all'.
all: all-df
ifeq ($(TEST_CONFIG),default)
  all: all-d
endif

# These tests read additional files.  (I am probably missing some.)
out/cpp-prefix-yywrap.ok: cppscan-yywrap.input2
out/cpp-prefix-yywrap2-yyclass.ok: cppscan-yywrap.input2
out/cppscan-yywrap.ok: cppscan-yywrap.input2
out/input-buffers.ok: input-buffers.input2 input-buffers.input3 input-buffers.input4
out/input-buffers-lineno.ok: input-buffers.input2 input-buffers.input3 input-buffers.input4
out/input-buffers-lineno-cpp.ok: input-buffers.input2 input-buffers.input3 input-buffers.input4
out/input-buffers-with-read.ok: input-buffers-with-read.input2 input-buffers.input3 input-buffers.input4
out/two-buffers.ok: two-buffers.input2
out/two-buffers-cpp.ok: two-buffers-cpp.input2
out/username-yywrap.ok: username-yywrap.input2
out/yy-wrap-function.ok: yy-wrap-function.input2
out/yym-wrap.ok: username-yywrap.input2

# For "c-prefix", check that it does not export any "yy_" symbols.
out/c-prefix-validated.ok: out/c-prefix.ok
	nm --extern-only --defined-only out/c-prefix.yy.o > out/c-prefix.yy.syms
	if grep yy_ out/c-prefix.yy.syms; then \
	  echo "There should not be any exported yy_ symbols!"; \
	  exit 2; \
	fi
	touch $@

# Run $(SMFLEX) on an input that should provoke a syntax error.
out/%.bad: %.lex %.expect $(SMFLEX)
	@mkdir -p out
	if $(SMFLEX) $(SMFLEX_OPTS) $($(TEST_CONFIG)_OPS) -oout/$*.lex.c $< >out/$*.actual 2>&1; then \
	  echo "That should have failed!"; \
	  exit 2; \
	fi
	@echo diff -b -u $*.expect out/$*.actual
ifeq ($(UPDATE_EXPECT),1)
	@if diff -b -u $*.expect out/$*.actual; then true; else \
	  echo ""; \
	  echo "The actual output is different from the expected output."; \
	  echo "Fixing it automatically due to UPDATE_EXPECT=1."; \
	  echo "./update-expect.sh $*"; \
	  ./update-expect.sh $*; \
	fi
else
	@if diff -b -u $*.expect out/$*.actual; then true; else \
	  echo ""; \
	  echo "The actual output is different from the expected output."; \
	  echo "If the actual is correct, update expected by running:"; \
	  echo "  $ ./update-expect.sh $*"; \
	  echo "Or, re-run tests with UPDATE_EXPECT=1 to update them all."; \
	  exit 2; \
	fi
endif
	touch $@

# List of tests of syntax errors.
all-invalid: out/invalid-action-pct-brace.bad
all-invalid: out/invalid-missing-smflex.bad
all-invalid: out/invalid-option-name.bad
all-invalid: out/invalid-pct-a.bad
all-invalid: out/invalid-sect1-repeated-pct-lbrace.bad
all-invalid: out/invalid-sect1-text-after-pct-lbrace.bad
all-invalid: out/invalid-sect1-text-after-pct-rbrace.bad
all-invalid: out/invalid-sect1-unclosed-pct-lbrace.bad
all-invalid: out/invalid-sect1-unmatched-pct-rbrace.bad
all-invalid: out/invalid-sect2-pct-brace.bad
all-invalid: out/invalid-sect2p-indented-code.bad
all-invalid: out/invalid-sect2p-repeated-pct-lbrace.bad
all-invalid: out/invalid-sect2p-text-after-pct-lbrace.bad
all-invalid: out/invalid-sect2p-text-after-pct-rbrace.bad
all-invalid: out/invalid-sect2p-unclosed-pct-lbrace.bad
all-invalid: out/invalid-sect2p-unmatched-pct-rbrace.bad
all-invalid: out/invalid-smflex-after-indented.bad
all-invalid: out/invalid-smflex-repeated.bad
all-invalid: out/invalid-smflex-too-large.bad
all-invalid: out/invalid-smflex-too-small.bad

.PHONY: all-invalid
all: all-invalid

# Command line error.
all-cmderr: out/cmderr-outfile-ext.bad
out/cmderr-outfile-ext.bad: cmderr-outfile-ext.expect $(SMFLEX)
	if $(SMFLEX) -oout/cmderr-outfile-ext.h username.lex >out/cmderr-outfile-ext.actual 2>&1; then \
	  echo "should have failed!"; \
	  exit 2; \
	fi
	diff -b -u cmderr-outfile-ext.expect out/cmderr-outfile-ext.actual
	touch $@

.PHONY: all-cmderr
all: all-cmderr


# Unit test program.
out/test-%.exe: test-%.c ../%.h ../%.c
	$(CC) -o$@ $(CFLAGS) -I.. test-$*.c ../$*.c

out/test-%.unit.ok: out/test-%.exe
	out/test-$*.exe
	touch $@

all: out/test-eval-skel-expr.unit.ok
all: out/test-growstring.unit.ok


# v.Nu does not validate links, so I wrote a little smflex program
# to do it.  Use it to check the manual.
#
# This rule appears before the v.Nu rule because then 'make' will
# normally run it first, and it is very fast.
all: out/smflex-check-links.ok
out/smflex-check-links.ok: ../smflex.html out/html-link-validator.exe
	cd .. && FAIL_IF_ERRORS=1 test/out/html-link-validator.exe < smflex.html
	touch $@

# Check smflex.html for syntax errors if HTML_CHECKER is defined.
# I use the Nu Html Checker (v.Nu) at:
# https://github.com/validator/validator
ifneq ($(HTML_CHECKER),)
all: out/smflex-check-html.ok
out/smflex-check-html.ok: ../smflex.html
	@mkdir -p $(dir $@)
	$(HTML_CHECKER) $^
	touch $@
endif

# Command to print a message at the end.
all:
	@echo 'All tests in test/ passed.  (Use "make clean" there to reset.)'

# Remove all test outputs.
clean:
	rm -rf out


# --------------------- Performance Testing ---------------------
# Directory in which to store performance test outputs.
OPERF := operf

# Performance testing corpus.
$(OPERF)/corpus.c:
	@mkdir -p $(OPERF)
	cat ../*.c ../*.h > $@

# Same file, but with CRLF line endings.
$(OPERF)/corpus-crlf.c: $(OPERF)/corpus.c
	perl -p -e 's/\n/\r\n/' < $< > $@


# How many times to run a program being measured.  More runs obviously
# takes more time but produces more reliable measurement.
NUM_RUNS := 5

# Within the test program, how many times should it scan its input?
# More scans takes more time but gives a higher signal to noise ratio
# and discounts startup overhead.
NUM_SCANS := 1000

# How to run a program and measure its execution time.
#
# The "-oneline" is pretty important; on MinGW, redirecting all
# output to /dev/null speeds up the program substantially!
MEASURE_TIME := perl median-of-5.pl -iters $(NUM_RUNS) -oneline

# flex-2.5.4 executable, for comparison.
FLEX254 := ../../flex-2.5.4/flex

# flex-2.6.4, also for comparison.
FLEX264 := /usr/bin/flex

# Compilation flags for use when making performance test programs.
PERF_CPPFLAGS := -DHAS_POSIX -DNDEBUG
PERF_CFLAGS := -O2 -Wall -Werror -Wno-unused-function $(PERF_CPPFLAGS) -std=c89
PERF_CXXFLAGS := -O2 -Wall -Werror -Wno-unused-function $(PERF_CPPFLAGS) -std=c++98


# 'readfile' program for baseline.
all: $(OPERF)/readfile-c.exe
$(OPERF)/readfile-c.exe: readfile.c
	@mkdir -p $(OPERF)
	$(CC) -o$@ -std=c89 $(PERF_CFLAGS) $<

all: $(OPERF)/readfile-cpp.exe
$(OPERF)/readfile-cpp.exe: readfile.c
	@mkdir -p $(OPERF)
	$(CXX) -o$@ -xc++ -std=c++98 $(PERF_CXXFLAGS) $<

# This collects together the measurements supporting the main
# conclusions from the readfile experiment:
#
#   - fread vs. read vs. iostream is all about the same
#   - size of buffer must be a power of 2 for fread on MinGW
#   - text mode reading is about 2x slower
#   - compiled as C vs. C++ does not make a difference
#
$(OPERF)/readfile-summary.txt: $(OPERF)/readfile-c.exe $(OPERF)/readfile-cpp.exe
	rm -f $@
	$(MEASURE_TIME) $(OPERF)/readfile-c.exe   fread    0x2000 $(OPERF)/corpus.c 1000 b >>$@
	$(MEASURE_TIME) $(OPERF)/readfile-cpp.exe fread    0x2000 $(OPERF)/corpus.c 1000 b >>$@
	$(MEASURE_TIME) $(OPERF)/readfile-c.exe   fread    0x2000 $(OPERF)/corpus.c 1000 t >>$@
	$(MEASURE_TIME) $(OPERF)/readfile-c.exe   read     0x2000 $(OPERF)/corpus.c 1000 b >>$@
	$(MEASURE_TIME) $(OPERF)/readfile-cpp.exe iostream 0x2000 $(OPERF)/corpus.c 1000 b >>$@
	$(MEASURE_TIME) $(OPERF)/readfile-c.exe   fread    0x2010 $(OPERF)/corpus.c 1000 b >>$@
	$(MEASURE_TIME) $(OPERF)/readfile-c.exe   fread   0x10000 $(OPERF)/corpus.c 1000 b >>$@
	cat $@


# Base name of the lexer to test.  I usually specify this on the
# command line, rather than modifying Makefile, but the list here
# is useful as reference for the allowable values.
LEXER_NAME := c-lexer1
#LEXER_NAME := elsa-lexer
#LEXER_NAME := reject-frob
#LEXER_NAME := noreject-frob
#LEXER_NAME := yylineno
#LEXER_NAME := noyylineno
#LEXER_NAME := trailing-context
#LEXER_NAME := notrailing-context

# Whether to generate a C or C++ interface.
INTERFACE := c
#INTERFACE := cpp

# Which variant of flex to use.
VARIANT := smflex
#VARIANT := flex254
#VARIANT := flex264

# Map VARIANT to an executable.
smflex_EXEC       := $(SMFLEX) $(SMFLEX_OPTS)
flex254_EXEC      := $(FLEX254)
flex264_EXEC      := $(FLEX264)

# Map VARIANT to additional compilation flags.  The -I. is to allow
# perftest.h to be included.  The others are to get the respective
# FlexLexer.h files.
smflex_CFLAGS     := -I.
flex254_CFLAGS    := -I. -Iinclude-flex-2.5.4
flex264_CFLAGS    := -I. -Iinclude-flex-2.6.4


# Names for various flex/smflex option combinations.
#
# I think this is an exhaustive list, except for non-default -7, given
# the restrictions:
#
#   - Set of all flags: a e m f F 8.
#   - Cannot combine f and F.
#   - Cannot combine m either either f or F.
#   - 8 is an option only with (f or F) and not e
#     (since it is already active in other configurations)
#
# There is also 'r' but that just changes the input method, which does
# not make a significant difference for performance.
#
# Note that -Cem is the default, not -C.
#
default_OPS             :=
compr_OPS               := -C
align_OPS               := -Ca
equiv_OPS               := -Ce
meta_OPS                := -Cm
align_equiv_OPS         := -Cae
equiv_meta_OPS          := -Cem
align_meta_OPS          := -Cam
align_equiv_meta_OPS    := -Caem
full_OPS                := -Cf
full_8_OPS              := -Cf -8
full_align_OPS          := -Cfa
full_align_8_OPS        := -Cfa -8
full_equiv_OPS          := -Cfe
full_align_equiv_OPS    := -Cfae
Jaco_OPS                := -CJ
Jaco_8_OPS              := -CJ -8
Jaco_align_OPS          := -CJa
Jaco_align_8_OPS        := -CJa -8
Jaco_equiv_OPS          := -CJe
Jaco_align_equiv_OPS    := -CJae

# List of named configurations, in the same order as above.
PERF_CONFIGS :=
PERF_CONFIGS += compr
PERF_CONFIGS += align
PERF_CONFIGS += equiv
PERF_CONFIGS += meta
PERF_CONFIGS += align_equiv
PERF_CONFIGS += equiv_meta
PERF_CONFIGS += align_meta
PERF_CONFIGS += align_equiv_meta
PERF_CONFIGS += full
PERF_CONFIGS += full_8
PERF_CONFIGS += full_align
PERF_CONFIGS += full_align_8
PERF_CONFIGS += full_equiv
PERF_CONFIGS += full_align_equiv
PERF_CONFIGS += Jaco
PERF_CONFIGS += Jaco_8
PERF_CONFIGS += Jaco_align
PERF_CONFIGS += Jaco_align_8
PERF_CONFIGS += Jaco_equiv
PERF_CONFIGS += Jaco_align_equiv

# Utility to filter out a substring.  (Ordinary 'filter-out' cannot
# do that.)
#
# usage: $(call FILTER_OUT_SUBSTR,substring,list of things to filter))
FILTER_OUT_SUBSTR = $(foreach element,$(2),$(if $(findstring $(1),$(element)),,$(element)))

ifneq ($(filter reject-frob yylineno trailing-context,$(LEXER_NAME)),)
  # YY_REJECT, yylineno, and trailing context cannot be used with -Cf or -CJ.
  PERF_CONFIGS := $(call FILTER_OUT_SUBSTR,Jaco,$(PERF_CONFIGS))
  PERF_CONFIGS := $(call FILTER_OUT_SUBSTR,full,$(PERF_CONFIGS))
endif

#$(info PERF_CONFIGS: $(PERF_CONFIGS))


# Rule to build a performance-test executable with $(VARIANT).
#
# At least for now, I'm not making this depend on $(SMFLEX) because
# I won't want to have to redo everything after each change.
$(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-%.exe: $(LEXER_NAME).lex
	@mkdir -p $(OPERF)
ifeq ($(INTERFACE),c)
	$($(VARIANT)_EXEC) -o$(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$*.yy.c $($*_OPS) $<
	$(CC) $(PERF_CFLAGS) $($(VARIANT)_CFLAGS) -o$@ $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$*.yy.c
else
	$($(VARIANT)_EXEC) -+ -o$(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$*.yy.cc $($*_OPS) $<
	$(CXX) $(PERF_CXXFLAGS) $($(VARIANT)_CFLAGS) -o$@ $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$*.yy.cc
endif


# Rule to run an executable and measure its size and speed.
$(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-%-$(NUM_RUNS)-$(NUM_SCANS).log: $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-%.exe $(OPERF)/corpus.c
	size $< > $@
	$(MEASURE_TIME) $< $(OPERF)/corpus.c $(NUM_SCANS) >> $@


# The TSV table file depends on each measurement.
$(foreach config,$(PERF_CONFIGS),$(eval $(OPERF)/table-$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$(NUM_RUNS)-$(NUM_SCANS).tsv: $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$(config)-$(NUM_RUNS)-$(NUM_SCANS).log))

# TSV table of results for all configurations.
$(OPERF)/table-$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$(NUM_RUNS)-$(NUM_SCANS).tsv:
	echo "configuration	size_bytes	time_ms	+/-" > $@
	for config in $(PERF_CONFIGS); do \
	  printf "%s	" $$config >> $@ || exit; \
	  head -2 $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$$config-$(NUM_RUNS)-$(NUM_SCANS).log | tail -1 | \
	    awk '{ printf "%s	", $$4 }' >> $@ || exit; \
	  grep ' +/- ' $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$$config-$(NUM_RUNS)-$(NUM_SCANS).log | \
	    awk '{ printf "%s	%s\n", $$1, $$3 }' >> $@ || exit; \
	done

# ASCII table of a TSV table.
$(OPERF)/table-%.txt: $(OPERF)/table-%.tsv
	$(PYTHON3) tsv-to-ascii-table.py < $^ > $@
	cat $@

# Make an HTML table; requires TSV_TO_HTML_TABLE, which is a script of
# mine that I have not (yet?) checked in here.
$(OPERF)/table-%.html: $(OPERF)/table-%.tsv
	$(TSV_TO_HTML_TABLE) --coltag:switches:code --appendcol:Notes \
	  "--prefixcol:+/-:&plusmn; " < $^ > $@


# Construct a single shell command that, when executed, will print
# out all of the configurations and their switches as TSV.
PRINT_CONFIGS_AND_OPS := $(foreach config,$(PERF_CONFIGS),printf "%s	%s\n" $(config) "$($(config)_OPS)";)

# Use that to make a table of configuration and switches.  This file is
# meant to be combined with the others using 'cut' and 'paste'.
$(OPERF)/table-switches.tsv:
	@mkdir -p $(dir $@)
	printf "%s	%s\n" configuration switches > $@
	( $(PRINT_CONFIGS_AND_OPS) ) >> $@
	$(PYTHON3) tsv-to-ascii-table.py < $@


# Run the active test set.
.PHONY: perftest
perftest: $(OPERF)/table-$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$(NUM_RUNS)-$(NUM_SCANS).txt

# Also make the switch table.
perftest: $(OPERF)/table-switches.txt


# Run $(LEXER_NAME) in both C and C++ mode and create a combined table.
$(OPERF)/table-$(VARIANT)-$(LEXER_NAME)-both-$(NUM_RUNS)-$(NUM_SCANS).tsv: $(OPERF)/table-switches.tsv
	$(MAKE) perftest INTERFACE=c
	$(MAKE) perftest INTERFACE=cpp
	paste \
	  $(OPERF)/table-switches.tsv \
	  <( cut -f2- $(OPERF)/table-$(VARIANT)-$(LEXER_NAME)-c-$(NUM_RUNS)-$(NUM_SCANS).tsv ) \
	  <( cut -f2- $(OPERF)/table-$(VARIANT)-$(LEXER_NAME)-cpp-$(NUM_RUNS)-$(NUM_SCANS).tsv ) \
	  > $@

# Run the above and make an ASCII table.
.PHONY: perftest-both-interfaces
perftest-both-interfaces: $(OPERF)/table-$(VARIANT)-$(LEXER_NAME)-both-$(NUM_RUNS)-$(NUM_SCANS).txt

# Also make the HTML table if I have the script.
ifneq ($(TSV_TO_HTML_TABLE),)
perftest-both-interfaces: $(OPERF)/table-$(VARIANT)-$(LEXER_NAME)-both-$(NUM_RUNS)-$(NUM_SCANS).html
endif

# Run both c-lexer1 and elsa-lexer in both C and C++ mode.
.PHONY: perftest-both-lexers
perftest-both-lexers:
	$(MAKE) perftest-both-interfaces LEXER_NAME=c-lexer1
	$(MAKE) perftest-both-interfaces LEXER_NAME=elsa-lexer


# Create plots.
../img-c-lexer1-both-1000.png: c-lexer1.gp operf/table-smflex-c-lexer1-both-5-1000.tsv
	$(GNUPLOT) $<

../img-elsa-lexer-both-1000.png: elsa-lexer.gp operf/table-smflex-elsa-lexer-both-5-1000.tsv
	$(GNUPLOT) $<

.PHONY: plots
plots: ../img-c-lexer1-both-1000.png ../img-elsa-lexer-both-1000.png


.PHONY: perfclean
perfclean:
	if test -d $(OPERF); then rm -r $(OPERF); fi

# Do-nothing target, useful if I just want to exercise the Makefile
# logic without running any recipes.
nothing:


# EOF
