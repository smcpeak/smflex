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

# C compilation options for correctness test executables.
CFLAGS := -g -Wall -I.

# smflex produces scanners that define 'yyunput' and 'input' even when
# they are not used.
CFLAGS += -Wno-unused-function

# C++ compilation options.
CXXFLAGS := $(CFLAGS)

# smflex executable.
SMFLEX := ../smflex

# Options to pass to smflex for additional diagnostic reporting.
# Specifically, this causes the "performance report" to be printed,
# which consists of one-line commentary on certain configurations.
# I have this enabled both to exercise the option and to check that
# what it is saying is sensible.
SMFLEX_OPTS := -p -p

# Run smflex on one file.
.PRECIOUS: out/%.yy.c
out/%.yy.c: %.lex
	@mkdir -p out
	$(SMFLEX) $(SMFLEX_OPTS) -o$@ $<
	python3 ../check-line-directives.py $@

# Run smflex on outfile.lex, avoiding use of -o option.
out/outfile-specified.yy.c: outfile.lex $(SMFLEX)
	@mkdir -p out
	$(SMFLEX) $(SMFLEX_OPTS) $<

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
	$(SMFLEX) $(SMFLEX_OPTS) -o$@ $<
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
#
# If UPDATE_EXPECT=1, then automatically update the expected output
# to match the actual output.  Be sure to review the changes with
# 'git diff' before checking in!
#
out/%.ok: out/%.exe %.input %.expect
	rm -f $@
	out/$*.exe < $*.input > out/$*.actual
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
all: out/c-int-batch.ok
all: out/c-int-default.ok
all: out/c-int-interactive.ok
all: out/c-int-set-int-0.ok
all: out/c-int-set-int-1.ok
all: out/c-int-set-int-isatty.ok
all: out/c-lexer1.ok
all: out/continued-action2.ok
all: out/cpp-int-batch.ok
all: out/cpp-int-default.ok
all: out/cpp-int-interactive.ok
all: out/cpp-manual-example.lexpp.matches
all: out/cpp-manual-example.ok
all: out/cpp-prefix-yywrap-yyclass.ok
all: out/cpp-prefix-yywrap.ok
all: out/cpp-prefix.ok
all: out/cppscan-full.ok
all: out/cppscan-main.ok
all: out/cppscan-yywrap.ok
all: out/cppscan.ok
all: out/elsa-lexer.ok
all: out/noreject-frob.ok
all: out/notrailing-context.ok
all: out/noyylineno.ok
all: out/outfile-specified.ok
all: out/reject-frob.ok
all: out/trailing-context.ok
all: out/username-main.c_as_cc.ok
all: out/username-main.ok
all: out/username-yywrap.ok
all: out/username.ok
all: out/vert-bar-action.ok
all: out/yylineno.ok

# These tests read a second file.
username-yywrap.ok: username-yywrap.input2
cppscan-yywrap.ok: cppscan-yywrap.input2

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
NUM_SCANS := 100

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
PERF_CFLAGS := -O2 -Wall -Werror -Wno-unused-function -DHAS_POSIX


# 'readfile' program for baseline.
all: $(OPERF)/readfile-c.exe
$(OPERF)/readfile-c.exe: readfile.c
	@mkdir -p $(OPERF)
	$(CC) -o$@ -std=c89 $(PERF_CFLAGS) $<

all: $(OPERF)/readfile-cpp.exe
$(OPERF)/readfile-cpp.exe: readfile.c
	@mkdir -p $(OPERF)
	$(CXX) -o$@ -xc++ -std=c++98 $(PERF_CFLAGS) $<

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
default_OPS       :=
batch_OPS         := -B
align_OPS         := -Ca -B
equiv_meta_OPS    := -Cem -B
meta_OPS          := -Cm -B
equiv_OPS         := -Ce -B
compr_OPS         := -C -B

# All of the modes involving 'f' or 'F' have 'B' implicitly.
full_equiv_OPS    := -Cfe
Fast_equiv_OPS    := -CFe
Cfull_OPS         := -Cf
CFast_OPS         := -CF
full_OPS          := -f
Fast_OPS          := -F
full_align_OPS    := -Cfa
Fast_align_OPS    := -CFa


# List of named configurations, in the order I want them to appear in
# the summary.txt file.
PERF_CONFIGS :=

# The default, interactive scanner is around 10x slower, so I normally
# do not want to spend time measuring it.
ifeq ($(MEASURE_SLOW_CONFIGS),1)
  PERF_CONFIGS += default
endif

PERF_CONFIGS += batch
PERF_CONFIGS += align
PERF_CONFIGS += equiv_meta
PERF_CONFIGS += meta
PERF_CONFIGS += equiv
PERF_CONFIGS += compr
PERF_CONFIGS += full_equiv
PERF_CONFIGS += Fast_equiv
PERF_CONFIGS += Cfull
PERF_CONFIGS += CFast
PERF_CONFIGS += full
PERF_CONFIGS += Fast
PERF_CONFIGS += full_align
PERF_CONFIGS += Fast_align

ifeq ($(INTERFACE),cpp)
  # Remove all the configurations that use Fast, because those are
  # explicitly prohibited by smflex.
  PERF_CONFIGS := $(filter-out Fast% %Fast,$(PERF_CONFIGS))
endif

ifneq ($(filter reject-frob yylineno trailing-context,$(LEXER_NAME)),)
  # REJECT, yylineno, and trailing context cannot be used with -f or -F.
  PERF_CONFIGS := $(filter-out Fast% full% %Fast %full,$(PERF_CONFIGS))
endif

#$(info PERF_CONFIGS: $(PERF_CONFIGS))


# Rule to build a performance-test executable with $(VARIANT).
#
# At least for now, I'm not making this depend on $(SMFLEX) because
# I won't want to have to redo everything after each change.
.PRECIOUS: $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-%.exe
$(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-%.exe: $(LEXER_NAME).lex
	@mkdir -p $(OPERF)
ifeq ($(INTERFACE),c)
	$($(VARIANT)_EXEC) -o$(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$*.yy.c $($*_OPS) $<
	$(CC) $(PERF_CFLAGS) $($(VARIANT)_CFLAGS) -o$@ $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$*.yy.c
else
	$($(VARIANT)_EXEC) -+ -o$(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$*.yy.cc $($*_OPS) $<
	$(CXX) $(PERF_CFLAGS) $($(VARIANT)_CFLAGS) -o$@ $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$*.yy.cc
endif


# Rule to run an executable and measure its size and speed.
.PRECIOUS: $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-%-$(NUM_RUNS)-$(NUM_SCANS).log
$(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-%-$(NUM_RUNS)-$(NUM_SCANS).log: $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-%.exe $(OPERF)/corpus.c
	size $< > $@
	$(MEASURE_TIME) $< $(OPERF)/corpus.c $(NUM_SCANS) >> $@


# The summary file depends on each measurement.
$(foreach config,$(PERF_CONFIGS),$(eval $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$(NUM_RUNS)-$(NUM_SCANS)-summary.txt: $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$(config)-$(NUM_RUNS)-$(NUM_SCANS).log))

# Single file with all results.
$(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$(NUM_RUNS)-$(NUM_SCANS)-summary.txt:
	echo "configuration size_bytes    time_ms +/-" > $@
	echo "------------- ---------- ---------- ---" >> $@
	for config in $(PERF_CONFIGS); do \
	  printf "%-13s " "$$config" >> $@ || exit; \
	  head -2 $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$$config-$(NUM_RUNS)-$(NUM_SCANS).log | tail -1 | \
	    awk '{ printf "%10s ", $$4 }' >> $@ || exit; \
	  grep ' +- ' $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$$config-$(NUM_RUNS)-$(NUM_SCANS).log | \
	    awk '{ printf "%10s %3s\n", $$1, $$3 }' >> $@ || exit; \
	done
	cat $@

# Convenience alias.
.PHONY: perftest
perftest: $(OPERF)/$(VARIANT)-$(LEXER_NAME)-$(INTERFACE)-$(NUM_RUNS)-$(NUM_SCANS)-summary.txt

.PHONY: perfclean
perfclean:
	if -d $(OPERF); then rm -r $(OPERF); fi


# EOF
