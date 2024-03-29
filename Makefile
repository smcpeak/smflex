# smflex/Makefile

# NOTE: This Makefile requires GNU make.

# Default target.
all:

# --------------------- Configuration ---------------------
# Pull in the configure-generated settings.
ifeq ($(wildcard config.mk),)
  $(error config.mk does not exist.  Please run ./configure first.)
endif
include config.mk

# Name of the scanner generator executable.
SMFLEX = smflex

# Shell to use when running recipes.
SHELL = /bin/sh

# Bison parser generator.
BISON = bison

# Python 3 interpreter.  Only needed if MAINTAINER_MODE=1.
PYTHON3 = python3

# Enable all GCC warnings.
WARNINGS = -Wall

# Flag to enable dependency generation to .d files.
GENDEPS = -MMD

# Directory in which to put .o and .d files.  Note, however, that
# $(SMFLEX) still gets put into the current directory.
OBJ = obj

# Now optionally pull in local customizations via personal.mk.  Those
# can override settings above or from config.mk.
-include personal.mk

# ----------------------- Build rules ---------------------
# Turn off default rules.  (One reason this is important is there is
# a default rule for running bison, but I want to ensure bison only
# runs when specifically requested.)
.SUFFIXES:

# Delete a target when its recipe fails.
.DELETE_ON_ERROR:

# Do not delete intermediate files.
.SECONDARY:

# Rule for compiling one C source file.
$(OBJ)/%.o: %.c
	@mkdir -p $(OBJ)
	$(CC) -o$@ -c $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $(GENDEPS) $<

# Pull in generated dependencies.
include $(wildcard $(OBJ)/*.d)

# C source code.
SOURCES :=
SOURCES += ccl.c
SOURCES += dfa.c
SOURCES += ecs.c
SOURCES += eval-skel-expr.c
SOURCES += gen.c
SOURCES += generated-header.skl.c
SOURCES += generated-scanner.skl.c
SOURCES += growstring.c
SOURCES += input-parse.y.c
SOURCES += input-scan.lex.c
SOURCES += main.c
SOURCES += misc.c
SOURCES += nfa.c
SOURCES += sym.c
SOURCES += tblcmp.c
SOURCES += yylex.c

# Object files to compile and link into 'smflex'.
OBJECTS = $(patsubst %.c,$(OBJ)/%.o,$(SOURCES))

# Name of the distribution, meaning what goes before ".tar.gz" in the
# distribution tarball file name, and also the name of the directory
# that tarball expands to.  The value here is not normally used.
# Instead, the 'dist' target sets it when invoking 'dist2'.
DIST_NAME = smflex

# How to invoke the 'smflex' built here.
SMFLEX_EXEC = ./$(SMFLEX)

# Flags to pass to $(SMFLEX_EXEC).
PERF_REPORT = -p
SMFLEX_FLAGS = $(PERF_REPORT)

# Default compression to use when generating input-scan.lex.c.  This is overridden
# by the 'bigcheck' target.  Empty means none.
COMPRESSION =


# Link the final smflex executable.
all: $(SMFLEX)
$(SMFLEX): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(SMFLEX) $(LDFLAGS) $(OBJECTS) $(LIBS)


# Quick test.
#
# This does not invoke the tests in test/ by itself because I want
# people to be able to put smflex into their own project by just
# copying the files in the root directory.  I also like that what
# is run by default lets people quickly get some confidence that the
# basic system is working before investing time into the long tests.
test: check
check: $(SMFLEX)
	@#
	@# Check to see if the current smflex produces the same output
	@# as it did before.
	@#
	$(SMFLEX_EXEC) $(SMFLEX_FLAGS) $(COMPRESSION) -oscan.tmp.c input-scan.lex
	@#
	@# Fix the file names in #line directives and comments, as well
	@# as the include guard name, so they match.
	@#
	sed -e 's,scan\.tmp,input-scan.lex,' \
	    -e 's,SCAN_TMP,INPUT_SCAN_LEX,'  < scan.tmp.h > scan.actual.h
	sed -e 's,scan\.tmp,input-scan.lex,' < scan.tmp.c > scan.actual.c
	rm scan.tmp.h scan.tmp.c
	@#
	@# This comparison does not ignore whitespace since the
	@# default is for smflex to always use LF line endings, even
	@# on Windows.
	@#
	diff input-scan.lex.h scan.actual.h
	diff input-scan.lex.c scan.actual.c
	rm scan.actual.h scan.actual.c
ifeq ($(MAINTAINER_MODE),1)
	@#
	@# Make sure I do not have "scan.tmp" in input-scan.lex.c.
	@#
	if grep scan.tmp input-scan.lex.c; then false; else true; fi
	@#
	@# Check the #line directives.
	@#
	$(PYTHON3) ./test/check-line-directives.py input-scan.lex.c
endif
	@echo "Check successful, using COMPRESSION=\"$(COMPRESSION)\"".
	@echo "Run \"make -C test check\" and \"make bigcheck\" for more thorough checking."
	@echo "The former takes around 5 minutes and the latter 1.5 minutes."

# One step of 'bigcheck'.  COMPRESSION should be set.
#
#   1. Regenerate input-scan.lex.c with that compression mode.
#   2. Recompile smflex with the new input-scan.lex.c.
#   3. Run the new smflex to make input-scan.lex.c again, with the same compression
#      mode.
#   4. Check that the second output agrees with the first.
#
bigcheck1:
	rm -f input-scan.lex.c
	$(MAKE) input-scan.lex.c ENABLE_INPUT_SCAN_RULE=1
	$(MAKE) $(SMFLEX)
	$(MAKE) check

# Run 'bigcheck1' with various compression modes.
#
# Thus, we are checking at each stage that the existing smflex (with
# whatever previous compression mode was active) and the new smflex have
# the same output.
#
bigcheck:
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-C" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-Ce" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-Cm" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-Cf" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-Cfea" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-CJe --use-read" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) bigcheck1
	@echo "All checks successful"

# Exercise the ability to build in a single line, as advertised in
# README.md.
.PHONY: one-line-compile
one-line-compile:
	$(CC) -o smflex.oneline.exe *.c
	./smflex.oneline.exe --version
	rm smflex.oneline.exe

# Install to the chosen --prefix.
install: $(SMFLEX) installdirs
	$(INSTALL_PROGRAM) $(SMFLEX) $(bindir)/$(SMFLEX)

installdirs:
	$(SHELL) mkinstalldirs $(bindir)

uninstall:
	rm -f $(bindir)/$(SMFLEX)

# Remove compilation output, but keep 'configure' output.
clean:
	rm -f $(SMFLEX) config.log config.cache
	if test -d $(OBJ); then rm -r $(OBJ); fi
	$(MAKE) -C test clean

# Remove everything not in the source distribution tarball.
distclean: clean
	rm -f config.mk config.status

# Complete set of files and directories to be included in the
# distribution tarball, except that a few things in 'test' get removed.
DISTFILES :=
DISTFILES += $(SOURCES)
DISTFILES += COPYING
DISTFILES += Makefile
DISTFILES += NEWS
DISTFILES += README.md
DISTFILES += ccl.h
DISTFILES += config.mk.in
DISTFILES += configure
DISTFILES += configure.in
DISTFILES += dfa.h
DISTFILES += dist-build-test.sh
DISTFILES += ecs.h
DISTFILES += eval-skel-expr.h
DISTFILES += encode.sh
DISTFILES += flexchar.h
DISTFILES += flexdef.h
DISTFILES += gen.h
DISTFILES += generated-header.skl
DISTFILES += generated-scanner.skl
DISTFILES += growstring.h
DISTFILES += historical
DISTFILES += input-parse.h
DISTFILES += input-parse.y
DISTFILES += input-parse.y.h
DISTFILES += input-scan.h
DISTFILES += input-scan.lex
DISTFILES += input-scan.lex.h
DISTFILES += install.sh
DISTFILES += main.h
DISTFILES += misc.h
DISTFILES += mkinstalldirs
DISTFILES += nfa.h
DISTFILES += smflex.html
DISTFILES += sym.h
DISTFILES += tblcmp.h
DISTFILES += test/*.c
DISTFILES += test/*.cc
DISTFILES += test/*.expect
DISTFILES += test/*.gp
DISTFILES += test/*.h
DISTFILES += test/*.input*
DISTFILES += test/*.lex
DISTFILES += test/*.lexpp
DISTFILES += test/*.pl
DISTFILES += test/*.py
DISTFILES += test/*.sh
DISTFILES += test/Makefile
DISTFILES += version.h
DISTFILES += yylex.h

# Extract the version number from version.h.
VERSION_NUMBER = $(shell grep '#define SMFLEX_VERSION' version.h | \
                         sed -e 's/[^"]*"//' -e 's/"//')

# Create a source tarball for distribution.
dist: all
	$(MAKE) DIST_NAME=smflex-$(VERSION_NUMBER) dist2

# Do the main work of making the tarball, given $(DIST_NAME).  Also
# optionally run tests and remove it afterward.
dist2:
	rm -rf $(DIST_NAME)
	rm -f $(DIST_NAME).tar $(DIST_NAME).tar.gz
	mkdir $(DIST_NAME)
	cp --parents --recursive --target-directory=$(DIST_NAME) $(DISTFILES)
	tar cfz $(DIST_NAME).tar.gz $(DIST_NAME)
ifeq ($(TEST_DIST),1)
	find $(DIST_NAME) -type f > distro-files.txt
	find $(DIST_NAME) -type f -executable > executable-files.txt
	@# Build the distribution the way a user would.
	cd $(DIST_NAME) && \
	  ./configure --prefix=$$(pwd)/opt && \
	  make && \
	  make check && \
	  make install
ifeq ($(THOROUGH_TEST_DIST),1)
	cd $(DIST_NAME) && \
	  make -C test && \
	  make bigcheck
endif
	@echo "--------- Distribution files ---------"
	@cat distro-files.txt
	@echo "--------- Executable files ---------"
	@cat executable-files.txt
	@echo "--------- Installation files ---------"
	@cd $(DIST_NAME)/opt && find . -type f
	rm distro-files.txt
	rm executable-files.txt
endif
	rm -rf $(DIST_NAME)
ifeq ($(CLEAN_DIST),1)
	@# Remove the distribution after test passes.
	rm $(DIST_NAME).tar.gz
endif

# Build and test a distribution, then clean up afterward.
dist-test-clean:
	$(MAKE) TEST_DIST=1 CLEAN_DIST=1 dist

# Like above, but running the "thorough" tests too.
dist-thorough-test-clean:
	$(MAKE) TEST_DIST=1 THOROUGH_TEST_DIST=1 CLEAN_DIST=1 dist

# If 'config.mk.in' has changed, regenerate 'config.mk'.
config.mk: config.mk.in config.status
	CONFIG_FILES=$@ CONFIG_HEADERS= ./config.status


# ------------------------ Maintainer rules -------------------------
# The rules in this section are meant for use when making changes to
# smflex itself.  They are not normally enabled for these reasons:
#
# 1. These rules rely on having other tools installed.  The normal
#    build from a source distribution should only need a C compiler.
#
# 2. The build from a fresh git clone would otherwise needlessly
#    rebuild some things because the file timestamps after a clone
#    are semi-random.
#
# 3. The rule for building input-scan.lex.c is effectively circular,
#    with the dependency going through $(SMFLEX).  I do not declare
#    that dependency so $(MAKE) remains happy, but it is there, and
#    some care is required to deal with that.
#
# To enable maintainer mode, set MAINTAINER_MODE=1 in personal.mk.

ifeq ($(MAINTAINER_MODE),1)


# Bison-generated parser for smflex's input language.
input-parse.y.c: input-parse.y
	$(BISON) --defines=input-parse.y.h --output=input-parse.y.c \
	  input-parse.y

# This rule tells 'make' that in order to create 'input-parse.y.h' it
# must first create 'input-parse.y.c'.  The latter is what actually
# makes 'input-parse.y.h'.
input-parse.y.h: input-parse.y.c

# 'generated-scanner.skl.c' contains the contents of
# 'generated-scanner.skl' as a C string.
generated-scanner.skl.c: generated-scanner.skl encode.sh
	$(SHELL) encode.sh generated-scanner.skl generated-scanner.skl.c scanner_skl_contents

# Similarly, 'generated-header.skl.c' contains the contents of
# 'generated-header.skl'.
generated-header.skl.c: generated-header.skl encode.sh
	$(SHELL) encode.sh generated-header.skl generated-header.skl.c header_skl_contents

# If 'configure' has changed, but its output script is older, run the
# latter.
config.status: configure
	./config.status --recheck

# If 'configuire.in' has changed, run autoconf.
configure: configure.in
	autoconf

ENABLE_INPUT_SCAN_RULE := 1


endif # MAINTAINER_MODE


# This section is separate from MAINTAINER_MODE because it is needed for
# the 'bigcheck' target but I do not want to activate all of the
# maintainer rules for that.
ifeq ($(ENABLE_INPUT_SCAN_RULE),1)


# 'input-scan.lex.c' is the output of running smflex on 'input-scan.lex'.
# It is used by smflex to read its input file.  Hence, smflex is partially
# written in its own language.
input-scan.lex.c: input-scan.lex
	$(SMFLEX_EXEC) $(SMFLEX_FLAGS) $(COMPRESSION) input-scan.lex

# Inform 'make' that it has to create the .c file to get the .h file.
input-scan.lex.h: input-scan.lex.c


endif # ENABLE_INPUT_SCAN_RULE


# EOF
