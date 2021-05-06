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

# --------------------- Performance Testing ---------------------
# Performance testing corpus.
out/perf/corpus.c:
	@mkdir -p out/perf
	cat ../*.c ../*.h > $@

# Same file, but with CRLF line endings.
out/perf/corpus-crlf.c: out/perf/corpus.c
	perl -p -e 's/\n/\r\n/' < $< > $@


# How many times to run a program being measured.  More runs obviously
# takes more time but produces more reliable measurement.
NUM_RUNS := 5

# Within the test program, how many times should it scan its input?
# More scans takes more time but gives a higher signal to noise ratio
# and discounts startup overhead.
NUM_SCANS := 100

# How to run a program and measure its execution time.
MEASURE_TIME := perl median-of-5.pl -iters $(NUM_RUNS)


# 'readfile' program for baseline.
all: out/perf/readfile-c.exe
out/perf/readfile-c.exe: readfile.c
	$(CC) -o$@ -std=c89 -O2 -Wall -Werror -DHAS_POSIX $<

all: out/perf/readfile-cpp.exe
out/perf/readfile-cpp.exe: readfile.c
	$(CXX) -o$@ -xc++ -std=c++98 -O2 -Wall -Werror -DHAS_POSIX $<

# This collects together the measurements supporting the main
# conclusions from the readfile experiment:
#
#   - fread vs. read vs. iostream is all about the same
#   - size of buffer must be a power of 2 for fread on MinGW
#   - text mode reading is about 2x slower
#   - compiled as C vs. C++ does not make a difference
#
out/perf/readfile-summary.txt: out/perf/readfile-c.exe out/perf/readfile-cpp.exe
	rm -f $@
	$(MEASURE_TIME) -oneline out/perf/readfile-c.exe   fread    0x2000 out/perf/corpus.c 1000 b >>$@
	$(MEASURE_TIME) -oneline out/perf/readfile-cpp.exe fread    0x2000 out/perf/corpus.c 1000 b >>$@
	$(MEASURE_TIME) -oneline out/perf/readfile-c.exe   fread    0x2000 out/perf/corpus.c 1000 t >>$@
	$(MEASURE_TIME) -oneline out/perf/readfile-c.exe   read     0x2000 out/perf/corpus.c 1000 b >>$@
	$(MEASURE_TIME) -oneline out/perf/readfile-cpp.exe iostream 0x2000 out/perf/corpus.c 1000 b >>$@
	$(MEASURE_TIME) -oneline out/perf/readfile-c.exe   fread    0x2010 out/perf/corpus.c 1000 b >>$@
	$(MEASURE_TIME) -oneline out/perf/readfile-c.exe   fread   0x10000 out/perf/corpus.c 1000 b >>$@
	cat $@


# Base name of the lexer to test.
LEXER_NAME := c-lexer1
#LEXER_NAME := elsa-lexer

# Whether to generate a C or C++ interface.
INTERFACE := c
#INTERFACE := cpp


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

# Rule to build a performance-test executable.
#
# At least for now, I'm not making this depend on $(SMFLEX) because
# I won't want to have to redo everything after each change.
.PRECIOUS: out/perf/$(LEXER_NAME)-$(INTERFACE)-%.exe
out/perf/$(LEXER_NAME)-$(INTERFACE)-%.exe: $(LEXER_NAME).lex
	@mkdir -p out/perf
ifeq ($(INTERFACE),c)
	$(SMFLEX) -oout/perf/$(LEXER_NAME)-$(INTERFACE)-$*.yy.c $($*_OPS) $<
	$(CC) -O2 -Wall -o$@ out/perf/$(LEXER_NAME)-$(INTERFACE)-$*.yy.c
else
	$(SMFLEX) -+ -oout/perf/$(LEXER_NAME)-$(INTERFACE)-$*.yy.cc $($*_OPS) $<
	$(CXX) -O2 -Wall -o$@ out/perf/$(LEXER_NAME)-$(INTERFACE)-$*.yy.cc
endif


# Rule to run an executable and measure its size and speed.
.PRECIOUS: out/perf/$(LEXER_NAME)-$(INTERFACE)-%-$(NUM_RUNS)-$(NUM_SCANS).log
out/perf/$(LEXER_NAME)-$(INTERFACE)-%-$(NUM_RUNS)-$(NUM_SCANS).log: out/perf/$(LEXER_NAME)-$(INTERFACE)-%.exe out/perf/corpus.c
	size $< > $@
	$(MEASURE_TIME) $< out/perf/corpus.c $(NUM_SCANS) >> $@


# The summary file depends on each measurement.
$(foreach config,$(PERF_CONFIGS),$(eval out/perf/$(LEXER_NAME)-$(INTERFACE)-$(NUM_RUNS)-$(NUM_SCANS)-summary.txt: out/perf/$(LEXER_NAME)-$(INTERFACE)-$(config)-$(NUM_RUNS)-$(NUM_SCANS).log))

# Single file with all results.
out/perf/$(LEXER_NAME)-$(INTERFACE)-$(NUM_RUNS)-$(NUM_SCANS)-summary.txt:
	echo "configuration size_bytes    time_ms" > $@
	echo "------------- ---------- ----------" >> $@
	for config in $(PERF_CONFIGS); do \
	  printf "%-13s " "$$config" >> $@ || exit; \
	  head -2 out/perf/$(LEXER_NAME)-$(INTERFACE)-$$config-$(NUM_RUNS)-$(NUM_SCANS).log | tail -1 | \
	    awk '{ printf "%10s ", $$4 }' >> $@ || exit; \
	  grep '^median:' out/perf/$(LEXER_NAME)-$(INTERFACE)-$$config-$(NUM_RUNS)-$(NUM_SCANS).log | \
	    awk '{ printf "%10s\n", $$2 }' >> $@ || exit; \
	done
	cat $@

# Convenience alias.
.PHONY: perftest
perftest: out/perf/$(LEXER_NAME)-$(INTERFACE)-$(NUM_RUNS)-$(NUM_SCANS)-summary.txt

.PHONY: perfclean
perfclean:
	rm -rf out/perf


# EOF
