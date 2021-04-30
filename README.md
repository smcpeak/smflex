# Introduction

This repository is `smflex`, Scott McPeak's `flex`.

It was created by starting with Vern Paxson's flex-2.5.4,
obtained as
[commit c9213c7ef4162c9df687b06e1df853a0ca533187](https://github.com/westes/flex/commit/c9213c7ef4162c9df687b06e1df853a0ca533187)
at [github.com/westes/flex](https://github.com/westes/flex).

flex-2.5.4 was released in 1996.  A lot has happened to it in the
meantime.  The main issues with the current flex releases are:

* It is a lot more complicated.
* It has broken backward compatibility repeatedly, requiring me to
  frequently adapt.
* My impression is the C++ support is considered low priority.
* The current (2.6.4) version does not compile on MinGW due to
  use of POSIX `wait`.

Moreover, I want to significantly improve the C++ support, particularly
how it handles multiple flex lexers in the same program.  My intent with
this repo is to create something that stays as close to flex-2.5.4 as
possible, except for improved C++ support.

# Build and Install

The usual sequence should work:

```
$ ./configure --prefix=(some directory)
$ make
$ make check
$ make install
```

# Usage

See [flex.html](flex.html).

# Change history

See [NEWS](NEWS).

# License

See [COPYING](COPYING).

All of my contributions are under the same license terms, and I hereby
assign copyright for them to The Regents of the University of
California.
