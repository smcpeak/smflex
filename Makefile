# flex/Makefile

# NOTE: This Makefile requires GNU make.

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
CFLAGS += -Wall

# Now optionally pull in local customizations via personal.mk.  Those
# can override settings above or from config.mk.
-include personal.mk

# Default target.
all: $(FLEX)

# Turn off default rules.  (One reason this is important is there is
# a default rule for running bison, but I want to ensure bison only
# runs when specifically requested.)
.SUFFIXES:

# Enable the following rule.
.SUFFIXES: .c .o

# Rule for compiling one C source file.
.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

# Headers to include in the distribution, minus FlexLexer.h for some
# reason.
HEADERS = flexdef.h version.h

# Sources to include in the distribution, and also on which to run tags.
SOURCES = ccl.c dfa.c ecs.c gen.c main.c misc.c nfa.c parse.y \
	scan.l skel.c sym.c tblcmp.c yylex.c

# Object files to compile and link into 'flex'.
OBJECTS = ccl.o dfa.o ecs.o gen.o main.o misc.o nfa.o parse.o \
	scan.o skel.o sym.o tblcmp.o yylex.o

# Complete set of files and directories to be included in the
# distribution tarball, except that 'scan.c' gets renamed to
# 'initscan.c' during the packaging process, and a few things
# in 'test' get removed.
DISTFILES = README.md NEWS COPYING FlexLexer.h \
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


# The bison rules are usually disabled because 'git' will check out
# files with unpredictably-ordered timestamps and I do not want to
# rebuild this needlessly.  Run 'make RUN_BISON=1' to activate the rule
# and rebuild the parser if needed.
ifeq ($(RUN_BISON),1)

# Bison-generated parser for flex's input language.
parse.c: parse.y
	$(YACC) -d parse.y
	mv -f y.tab.c parse.c
	mv -f y.tab.h parse.h

# This rule tells 'make' that in order to create 'parse.h' it must first
# create 'parse.c'.  The latter is what actually makes 'parse.h'.
parse.h: parse.c

endif # RUN_BISON


# scan.c is the output of running flex on 'scan.l'.  It is used by
# flex to read it input file.  Hence, flex is partially written in
# its own language.  See the .bootstrap target.
scan.c: scan.l
	$(FLEX_EXEC) $(FLEX_FLAGS) $(COMPRESSION) -oscan.tmp.c scan.l
	mv -f scan.tmp.c scan.c

# 'skel.c' contains the contents of 'flex.skl' as a C string.
skel.c: flex.skl mkskel.sh
	$(SHELL) mkskel.sh flex.skl skel.c skel

# Compile-time dependencies.
scan.o: scan.c parse.h flexdef.h
yylex.o: yylex.c parse.h flexdef.h
main.o: main.c flexdef.h version.h
ccl.o: ccl.c flexdef.h
dfa.o: dfa.c flexdef.h
ecs.o: ecs.c flexdef.h
gen.o: gen.c flexdef.h
misc.o: misc.c flexdef.h
nfa.o: nfa.c flexdef.h
parse.o: parse.c flexdef.h
sym.o: sym.c flexdef.h
tblcmp.o: tblcmp.c flexdef.h

# Main set of automated tests.
test: check
check: $(FLEX)
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
	$(INSTALL_DATA) FlexLexer.h $(includedir)/FlexLexer.h

installdirs:
	$(SHELL) mkinstalldirs \
	  $(bindir) $(includedir)

uninstall:
	rm -f $(bindir)/$(FLEX)
	rm -f $(includedir)/FlexLexer.h

tags: $(SOURCES)
	ctags $(SOURCES)

TAGS: $(SOURCES)
	etags $(SOURCES)

clean:
	rm -f $(FLEX) *.o lex.yy.c lex.yy.cc \
		config.log config.cache
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

# For an explanation of the following Makefile rules, see node
# `Automatic Remaking' in GNU Autoconf documentation.
config.mk: config.mk.in config.status
	CONFIG_FILES=$@ CONFIG_HEADERS= ./config.status
config.status: configure
	./config.status --recheck

# This rule sometimes causes autoconf to run after a git checkout
# because the timestamps in that case are unpredictable.
#configure: configure.in
#	autoconf

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
