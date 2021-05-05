#!/bin/sh
# Performance test.

set -e

runecho() {
  echo "$@"
  "$@"
}

compile() {
  runecho gcc -O2 "-oout/$1.exe" "out/$1.yy.c"
}

timerun() {
  echo -n "$1: "
  median-of-5 -hide -iters 1 "$1" out/corpus.c 50
}

flex_compile() {
  runecho ../smflex -oout/c-lexer1-$1.yy.c $2 c-lexer1.lex
  compile c-lexer1-$1
}

flex_compile_all() {
  flex_compile default ""
  flex_compile align -Ca
  flex_compile equiv-meta -Cem
  flex_compile meta -Cm
  flex_compile equiv -Ce
  flex_compile compr -C
  flex_compile full-equiv -Cfe
  flex_compile Fast-equiv -CFe
  flex_compile Cfull -Cf
  flex_compile CFast -CF
  flex_compile full -f
  flex_compile Fast -F
  flex_compile full-align -Cfa
  flex_compile Fast-align -CFa

  size out/c-lexer1-*.exe
}

flex_compile_all


timerun out/c-lexer1-default.exe
timerun out/c-lexer1-align.exe
timerun out/c-lexer1-equiv-meta.exe
timerun out/c-lexer1-meta.exe
timerun out/c-lexer1-equiv.exe
timerun out/c-lexer1-compr.exe
timerun out/c-lexer1-full-equiv.exe
timerun out/c-lexer1-Fast-equiv.exe
timerun out/c-lexer1-Cfull.exe
timerun out/c-lexer1-CFast.exe
timerun out/c-lexer1-full.exe
timerun out/c-lexer1-Fast.exe
timerun out/c-lexer1-full-align.exe
timerun out/c-lexer1-Fast-align.exe

# EOF
