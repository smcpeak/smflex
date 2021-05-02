# flex/Makefile

# NOTE: This Makefile requires GNU make.

# Default target.
all:

# Pull in the configure-generated settings.
ifeq ($(wildcard config.mk),)
  $(error config.mk does not exist.  Please run ./configure first.)
endif
include config.mk

# Name of the scanner generator executable.
FLEX = flex

# Shell to use when running recipes.
SHELL = /bin/sh

# Bison parser generator.
BISON = bison

# Enable all GCC warnings.
WARNINGS = -Wall

# Flag to enable dependency generation to .d files.
GENDEPS = -MMD

# Directory in which to put .o and .d files.  Note, however, that
# $(FLEX) still gets put into the current directory.
OBJ = obj

# Now optionally pull in local customizations via personal.mk.  Those
# can override settings above or from config.mk.
-include personal.mk

# Turn off default rules.  (One reason this is important is there is
# a default rule for running bison, but I want to ensure bison only
# runs when specifically requested.)
.SUFFIXES:

# Rule for compiling one C source file.
$(OBJ)/%.o: %.c
	@mkdir -p $(OBJ)
	$(CC) -o$@ -c $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $(GENDEPS) $<

# Pull in generated dependencies.
include $(wildcard $(OBJ)/*.d)

# C source code.
SOURCES = $(wildcard *.c)

# Object files to compile and link into 'flex'.
OBJECTS = $(patsubst %.c,$(OBJ)/%.o,$(SOURCES))

# Complete set of files and directories to be included in the
# distribution tarball, except that a few things in 'test' get removed.
DISTFILES = README.md NEWS COPYING flex.html \
            configure config.mk.in Makefile \
            $(wildcard *.h) $(SOURCES) $(wildcard *.skl) \
            input-scan.lex input-parse.y \
            configure.in encode.sh install.sh mkinstalldirs \
            test

# Name of the distribution, meaning what goes before ".tar.gz" in the
# distribution tarball file name, and also the name of the directory
# that tarball expands to.  The value here is not normally used.
# Instead, the 'dist' target sets it when invoking 'dist2'.
DIST_NAME = flex

# How to invoke the 'flex' built here.
FLEX_EXEC = ./$(FLEX)

# Flags to pass to $(FLEX_EXEC).
PERF_REPORT = -p
FLEX_FLAGS = $(PERF_REPORT)

# Default compression to use when generating input-scan.lex.c.  This is overridden
# by the 'bigcheck' target.  Empty means none.
COMPRESSION =


# Link the final flex executable.
all: $(FLEX)
$(FLEX): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(FLEX) $(LDFLAGS) $(OBJECTS) $(LIBS)


# Main set of automated tests.
test: check
check: $(FLEX)
	@#
	@# Make sure I do not have "scan.tmp" in input-scan.lex.c.
	@#
	if grep scan.tmp input-scan.lex.c; then false; else true; fi
	@#
	@# Run the tests in test/.
	@#
	$(MAKE) -C test CC=$(CC)
	@#
	@# Check to see if the current flex produces the same output
	@# as it did before.
	@#
	$(FLEX_EXEC) $(FLEX_FLAGS) $(COMPRESSION) -oscan.tmp input-scan.lex
	@#
	@# Fix the file names in #line directives so they match.
	@#
	sed -e 's,"scan.tmp","input-scan.lex.c",' < scan.tmp > scan.actual
	rm scan.tmp
	@#
	@# This comparison does not ignore whitespace since the
	@# default is for flex to always use LF line endings, even
	@# on Windows.
	@#
	diff input-scan.lex.c scan.actual
	rm scan.actual
	@echo "Check successful, using COMPRESSION=\"$(COMPRESSION)\""

# One step of 'bigcheck'.  COMPRESSION should be set.
#
#   1. Regenerate input-scan.lex.c with that compression mode.
#   2. Recompile flex with the new input-scan.lex.c.
#   3. Run the new flex to make input-scan.lex.c again, with the same compression
#      mode.
#   4. Check that the second output agrees with the first.
#
bigcheck1:
	rm -f input-scan.lex.c
	$(MAKE) input-scan.lex.c
	$(MAKE) $(FLEX)
	$(MAKE) check

# Run 'bigcheck1' with various compression modes.
#
# Thus, we are checking at each stage that the existing flex (with
# whatever previous compression mode was active) and the new flex have
# the same output.
#
bigcheck:
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-C" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-Ce" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-Cm" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-f" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-Cfea" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-CFer" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) COMPRESSION="-l" PERF_REPORT="" bigcheck1
	rm -f input-scan.lex.c ; $(MAKE) bigcheck1
	@echo "All checks successful"

# Install to the chosen --prefix.
install: $(FLEX) installdirs
	$(INSTALL_PROGRAM) $(FLEX) $(bindir)/$(FLEX)

installdirs:
	$(SHELL) mkinstalldirs \
	  $(bindir) $(includedir)

uninstall:
	rm -f $(bindir)/$(FLEX)

clean:
	rm -f $(FLEX) config.log config.cache
	if test -d $(OBJ); then rm -r $(OBJ); fi
	$(MAKE) -C test clean

distclean: clean
	rm -f config.mk config.status

# Create a source tarball for distribution.
dist: $(FLEX) $(DISTFILES) input-parse.y.c input-parse.y.h
	$(MAKE) DIST_NAME=flex-`sed <version.h 's/[^"]*"//' | sed 's/"//'` dist2

# Do the main work of making the tarball, given $(DIST_NAME).  Also
# optionally run tests and remove it afterward.
dist2:
	rm -rf $(DIST_NAME)
	rm -f $(DIST_NAME).tar $(DIST_NAME).tar.gz
	mkdir $(DIST_NAME)
	tar cf - $(DISTFILES) | (cd $(DIST_NAME) && tar xfB -)
	rm -r $(DIST_NAME)/test/out
	rm $(DIST_NAME)/test/.gitignore
	tar chf $(DIST_NAME).tar $(DIST_NAME)
	gzip <$(DIST_NAME).tar >$(DIST_NAME).tar.gz
	rm $(DIST_NAME).tar
ifeq ($(TEST_DIST),1)
	find $(DIST_NAME) -type f > distro-files.txt
	@# Build the distribution the way a user would.
	cd $(DIST_NAME) && \
	  ./configure --prefix=$$(pwd)/opt && \
	  make && \
	  make check && \
	  make install
	@echo "--------- BEGIN: Distribution files ---------"
	@cat distro-files.txt
	@echo "---------- END: Distribution files ----------"
	@echo "--------- BEGIN: Installation files ---------"
	@cd $(DIST_NAME)/opt && find . -type f
	@echo "---------- END: Installation files ----------"
	rm distro-files.txt
endif
ifeq ($(CLEAN_DIST),1)
	@# Remove the distribution after test passes.
	rm -rf $(DIST_NAME)
	rm $(DIST_NAME).tar.gz
endif

# Build and test a distribution, then clean up afterward.
dist-test-clean:
	$(MAKE) TEST_DIST=1 CLEAN_DIST=1 dist

# If 'config.mk.in' has changed, regenerate 'config.mk'.
config.mk: config.mk.in config.status
	CONFIG_FILES=$@ CONFIG_HEADERS= ./config.status


# ------------------------ Maintainer rules -------------------------
# The rules in this section are meant for use when making changes to
# flex itself.  They are not normally enabled for these reasons:
#
# 1. The build from distribution source should not require any tools
#    beyond a C compiler, but these rules rely on having other tools
#    installed.
#
# 2. The build from a fresh git clone would otherwise needlessly
#    rebuild some things because the file timestamps after a clone
#    are semi-random.
#
# 3. The rule for building input-scan.lex.c is effectively circular,
#    with the dependency going through $(FLEX).  I do not declare
#    that dependency so $(MAKE) remains happy, but it is there, and
#    some care is required to deal with that.
#
# To enable maintainer mode, set MAINTAINER_MODE=1 in personal.mk.

ifeq ($(MAINTAINER_MODE),1)


# Bison-generated parser for flex's input language.
#
# For some reason, I need to pass "-y" (yacc mode) to get the
# definitions of the token codes.
#
input-parse.y.c: input-parse.y
	$(BISON) -y --defines=input-parse.y.h --output=input-parse.y.c \
	  input-parse.y

# This rule tells 'make' that in order to create 'input-parse.y.h' it
# must first create 'input-parse.y.c'.  The latter is what actually
# makes 'input-parse.y.h'.
input-parse.y.h: input-parse.y.c

# 'input-scan.lex.c' is the output of running flex on 'input-scan.lex'.
# It is used by flex to read it input file.  Hence, flex is partially
# written in its own language.
input-scan.lex.c: input-scan.lex
	$(FLEX_EXEC) $(FLEX_FLAGS) $(COMPRESSION) input-scan.lex

# 'generated-scanner.skl.c' contains the contents of 'generated-scanner.skl' as a C string.
generated-scanner.skl.c: generated-scanner.skl encode.sh
	$(SHELL) encode.sh generated-scanner.skl generated-scanner.skl.c scanner_skl_contents

# Similarly, 'generated-header.skl.c' contains the contents of 'generated-header.skl'.
generated-header.skl.c: generated-header.skl encode.sh
	$(SHELL) encode.sh generated-header.skl generated-header.skl.c header_skl_contents

# For an explanation of the following Makefile rules, see node
# `Automatic Remaking' in GNU Autoconf documentation.
config.status: configure
	./config.status --recheck
configure: configure.in
	autoconf


endif # MAINTAINER_MODE
