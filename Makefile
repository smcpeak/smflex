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

# Bison, operating in yacc compatibility mode.
YACC = bison -y

# Enable all GCC warnings.
WARNINGS = -Wall

# Flag to enable dependency generation to .d files.
GENDEPS = -MMD

# Now optionally pull in local customizations via personal.mk.  Those
# can override settings above or from config.mk.
-include personal.mk

# Turn off default rules.  (One reason this is important is there is
# a default rule for running bison, but I want to ensure bison only
# runs when specifically requested.)
.SUFFIXES:

# Rule for compiling one C source file.
%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $(GENDEPS) $<

# Pull in generated dependencies.
include $(wildcard *.d)

# Headers to include in the distribution.
HEADERS = ccl.h dfa.h ecs.h flexchar.h flexdef.h FlexLexer.h gen.h \
          main.h misc.h nfa.h parse.h sym.h tblcmp.h version.h yylex.h

# Sources to include in the distribution, and also on which to run tags.
SOURCES = ccl.c dfa.c ecs.c gen.c header.c main.c misc.c nfa.c parse.y \
          scan.l skel.c sym.c tblcmp.c yylex.c

# Object files to compile and link into 'flex'.
OBJECTS = ccl.o dfa.o ecs.o gen.o header.o main.o misc.o nfa.o parse.o \
          scan.o skel.o sym.o tblcmp.o yylex.o

# Complete set of files and directories to be included in the
# distribution tarball, except that 'scan.c' gets renamed to
# 'initscan.c' during the packaging process, and a few things
# in 'test' get removed.
DISTFILES = README.md NEWS COPYING \
            configure.in Makefile config.mk.in mkskel.sh flex.skl \
            $(HEADERS) $(SOURCES) \
            flex.html scan.c install.sh mkinstalldirs configure \
            test \
            parse.c parse.h

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

# Default compression to use when generating scan.c.  This is overridden
# by the 'bigcheck' target.  Empty means none.
COMPRESSION =


# Link the final flex executable.
all: $(FLEX)
$(FLEX): .bootstrap $(OBJECTS)
	$(CC) $(CFLAGS) -o $(FLEX) $(LDFLAGS) $(OBJECTS) $(LIBS)

# This creates the initial scan.c from initscan.c.  The latter is
# checked in to the repo so one does not need to have flex already
# in order to build it.  But once the bootstrap is done, indicated
# by the presence of the .bootstrap file, we let scan.c continue to
# evolve as flex itself changes.
.bootstrap: initscan.c
	rm -f scan.c
	cp initscan.c scan.c
	touch .bootstrap


# Main set of automated tests.
test: check
check: $(FLEX)
	@#
	@# Make sure I do not have "scan.tmp" in scan.c.
	@#
	if grep scan.tmp scan.c; then false; else true; fi
	if grep scan.tmp initscan.c; then false; else true; fi
	@#
	@# Run the tests in test/.
	@#
	$(MAKE) -C test CC=$(CC)
	@#
	@# Check to see if the current flex produces the same output
	@# as it did before.
	@#
	$(FLEX_EXEC) $(FLEX_FLAGS) $(COMPRESSION) -oscan.tmp.c scan.l
	@#
	@# Fix the file names in #line directives so they match.
	@#
	sed -e 's,"scan.tmp.c","scan.c",' < scan.tmp.c > scan.actual.c
	@#
	@# This comparison does not ignore whitespace since the
	@# default is for flex to always use LF line endings, even
	@# on Windows.
	@#
	diff scan.c scan.actual.c
	rm scan.actual.c scan.tmp.c
	@echo "Check successful, using COMPRESSION=\"$(COMPRESSION)\""

# With various compression modes:
#
#   1. Regenerate scan.c with that compression mode.
#   2. Recompile flex with the new scan.c.
#   3. Run the new flex to make scan.c again, with the same compression
#      mode.
#   4. Check that the second output agrees with the first.
#
# Thus, we are checking at each stage that the existing flex (with
# whatever previous compression mode was active) and the new flex have
# the same output.
#
bigcheck:
	rm -f scan.c ; $(MAKE) COMPRESSION="-C" check
	rm -f scan.c ; $(MAKE) COMPRESSION="-Ce" check
	rm -f scan.c ; $(MAKE) COMPRESSION="-Cm" check
	rm -f scan.c ; $(MAKE) COMPRESSION="-f" check
	rm -f scan.c ; $(MAKE) COMPRESSION="-Cfea" check
	rm -f scan.c ; $(MAKE) COMPRESSION="-CFer" check
	rm -f scan.c ; $(MAKE) COMPRESSION="-l" PERF_REPORT="" check
	rm -f scan.c ; $(MAKE)
	@echo "All checks successful"

# Install to the chosen --prefix.
install: $(FLEX) installdirs
	$(INSTALL_PROGRAM) $(FLEX) $(bindir)/$(FLEX)

installdirs:
	$(SHELL) mkinstalldirs \
	  $(bindir) $(includedir)

uninstall:
	rm -f $(bindir)/$(FLEX)

tags: $(SOURCES)
	ctags $(SOURCES)

TAGS: $(SOURCES)
	etags $(SOURCES)

clean:
	rm -f $(FLEX) *.d *.o lex.yy.c lex.yy.cc config.log config.cache
	$(MAKE) -C test clean

distclean: clean
	rm -f .bootstrap scan.c tags TAGS config.mk config.status

# Create a source tarball for distribution.
dist: $(FLEX) $(DISTFILES) parse.c parse.h
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
	mv $(DIST_NAME)/scan.c $(DIST_NAME)/initscan.c
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

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:


# ------------------------ Maintainer rules -------------------------
# The rules in this section are meant for use when making changes to
# flex itself.  They are not normally enabled for two reasons:
#
# 1. The build from distribution source should not require any tools
#    beyond a C compiler, but these rules rely on having other tools
#    installed.
#
# 2. The build from a fresh git clone would otherwise needlessly
#    rebuild some things because the file timestamps after a clone
#    are semi-random.
#
# To enable maintainer mode, set MAINTAINER_MODE=1 in personal.mk.

ifeq ($(MAINTAINER_MODE),1)


# Bison-generated parser for flex's input language.
parse.c: parse.y
	$(YACC) -d parse.y
	mv -f y.tab.c parse.c
	mv -f y.tab.h parse.h

# This rule tells 'make' that in order to create 'parse.h' it must first
# create 'parse.c'.  The latter is what actually makes 'parse.h'.
parse.h: parse.c

# scan.c is the output of running flex on 'scan.l'.  It is used by
# flex to read it input file.  Hence, flex is partially written in
# its own language.  See the .bootstrap target.
scan.c: scan.l
	$(FLEX_EXEC) $(FLEX_FLAGS) $(COMPRESSION) -oscan.tmp1.c scan.l
	sed -e 's,"scan.tmp1.c","scan.c",' < scan.tmp1.c > scan.tmp2.c
	rm scan.tmp1.c
	mv -f scan.tmp2.c scan.c

# 'skel.c' contains the contents of 'flex.skl' as a C string.
skel.c: flex.skl mkskel.sh
	$(SHELL) mkskel.sh flex.skl skel.c skel

# Similarly, 'header.c' contains the contents of 'FlexLexer.h'.
header.c: FlexLexer.h mkskel.sh
	$(SHELL) mkskel.sh FlexLexer.h header.c header_contents

# For an explanation of the following Makefile rules, see node
# `Automatic Remaking' in GNU Autoconf documentation.
config.status: configure
	./config.status --recheck
configure: configure.in
	autoconf


endif # MAINTAINER_MODE
