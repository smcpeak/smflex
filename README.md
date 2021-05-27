# Introduction

This repository is `smflex`, Scott McPeak's fork of Vern Paxson's
`flex-2.5.4`.

The goals for this project are:

* Retain the basic capabilities of `flex-2.5.4`, namely to generate fast
  lexical analyzers with either a C or C++ interface (API).

* Generate a scanner API that uses regular, consistent,
  forward-compatible names.  (This has required renaming many things.)

* Make the C API re-entrant, meaning its state is encapsulated in an
  object of which there can be multiple instances, each used
  independently of the others.

* Have the C++ API be a thin wrapper around the C API instead of
  something completely separate.

* Support standard C and C++.  C89 and C++98 (the first standardized
  versions) are the minimums.

* Support for today's mainstream operating systems, principally Linux,
  Windows, and Mac OS.

* Depend only on the standard C and C++ libraries.  In particular,
  neither `smflex` nor its generated scanners require POSIX-only
  functions.

* Generated scanners are entirely self-contained.  In particular, they
  do not use `libfl.a` (the `flex` runtime library), nor
  `FlexLexer.h` (the `flex` C++ lexer class interface).

* The process of building from a source distribution is simple and does
  depend on tools beyond a C compiler and linker.

* Stay compatible with `flex-2.5.4` when doing so does not conflict with
  another goal.

Notable **non-goals** include:

* Support for C and C++ implementations that do not conform to the
  standard.  For example, I removed all support for C compilers that
  do not understand function prototypes in order
  to reduce the maintenance effort going forward.

* Support for obsolete operating systems like MS-DOS.

* Compatibility with AT&T or POSIX `lex`.

* 100% compatibility with any version of `flex`.  See the section
  [Incompatibilities with Flex](https://htmlpreview.github.io/?https://github.com/smcpeak/smflex/blob/smflex/smflex.html#incompatibilities-with-flex)
  in the manual for guidance on what will need to be changed.

# Build and Install

The usual sequence is:

```
$ ./configure
$ make                # GNU make!  Might be called 'gmake'.
$ make check          # optional, but recommended
$ make install        # optional
```

The only thing `make install` does is copy the `smflex` executable into
`/usr/local/bin` (by default).  Specify `--prefix` to `./configure`
to put it elsewhere, or just copy it someplace convenient yourself
(or leave it in the build directory).
The executable does not care where it is located nor what it is
called.

The `configure` script requires common unix-like shell tools.  On
Windows, you'll need [Cygwin](https://www.cygwin.com/) or
[MSYS2](https://www.msys2.org/).

The `Makefile` requires [GNU Make](https://www.gnu.org/software/make/).

However, if either of those is a problem, don't worry: the core build
process is literally just:

1. Compile all of the `.c` files in the distribution root directory.
   No extra flags are needed.
2. Link them together to make the `smflex` executable.  No extra
   flags or libraries are needed (beyond the standard C library).

This can all be done in one shell command.  For example, if your
compiler is `gcc`, you can simply run:

```
$ gcc -o smflex *.c
```

# Usage

See the manual at [smflex.html](https://htmlpreview.github.io/?https://github.com/smcpeak/smflex/blob/smflex/smflex.html).  It contains all of the
relevant material from the <code>flex-2.5.4</code> manual, but that
material has been completely reorganized, and augmented by descriptions
of new features in <code>smflex</code>.

# Modifying smflex

If you want to make changes to `smflex`, you will probably want to
create a file called `personal.mk` in the repo root with these
contents:

```
CFLAGS = -g               # Enable debugging, disable optimization.
WARNINGS = -Wall -Werror  # Enable warnings, stop if one reported.
MAINTAINER_MODE = 1       # Activate code generation rules.
```

The last setting, `MAINTAINER_MODE=1`, enables the `Makefile` rules
that generate source code.  All of the generated code is included
in the distribution and checked in to the repository, but if you
change one of the sources (e.g., `input-parse.y`), then you will need
to have the corresponding generator tool (`bison` in this case).

Also, be aware that when `git` clones a repo, the file timestamps
are semi-randomly ordered, so in maintainer mode, `make` will likely
think it needs to rebuild some things even when it does not.

# Relationship to westes/flex

The current official `flex` releases are maintaned by Will Estes at
[github.com/westes/flex](https://github.com/westes/flex).  Maintenance
of a software tool is a difficult and thankless job.  From what I have
observed, Will and the other contributors there have done a competent
job at keeping `flex` alive and running on modern systems, but I want
to take things in a different direction.

`smflex` was created by starting with Vern Paxson's `flex-2.5.4`,
obtained as
[commit c9213c7ef4162c9df687b06e1df853a0ca533187](https://github.com/westes/flex/commit/c9213c7ef4162c9df687b06e1df853a0ca533187).
From there, the histories diverge; no attempt has been made at
merging in either direction.  Furthermore, merging would require a
lot of manual effort because, among other things, I have reformatted
all of the source code and made significant changes to its organization,
even while preserving the logic mostly unchanged.

`flex-2.5.4` was released in 1996.  A lot has happened to `flex` in the
meantime.  The main concerns I have with the current `flex` releases are:

* It has grown significantly more complicated.
* The C++-interface scanner interface has changed multiple times.
  Not only has this required adaptations, but it has been difficult
  to support multiple version of `flex` simultaneously.
* At the same time, all of the awkwardness of the C++ interface that
  `flex-2.5.4` had is still present.
* My impression is the C++ support is considered low priority, but
  for me it is at least as important as C support.
* The current (2.6.4) version uses POSIX functions such as `wait` that
  are not readily available on Windows.

Moreover, I want to significantly improve the C++ support, particularly
how it handles multiple scanners (generated from different `smflex` input
files) in the same program.  That too will
break backward compatibility.  By making my own fork and giving it a
new name, that can be clearly communicated.

# Change history

The best summary of what has changed is in the
[Incompatibilities with Flex](https://htmlpreview.github.io/?https://github.com/smcpeak/smflex/blob/smflex/smflex.html#incompatibilities-with-flex)
section of the manual.

There is also the old [NEWS](NEWS) file, but I have not made any changes
to that file so it only covers work leading up to
`flex-2.5.4`.

# License

See [COPYING](COPYING), the license that accompanied `flex-2.5.4`.

All of my contributions are under the terms of the
[2-Clause BSD License](https://opensource.org/licenses/BSD-2-Clause).
