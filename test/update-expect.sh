#!/bin/sh
# Copy the actual output of a test to its expected output.

if [ "x$1" = "x" ]; then
  echo "usage: $0 <test>"
  echo "Copies out/<test>.actual to <test>.expect"
  exit 2
fi

# When running on Windows, get rid of CR characters.
echo tr -d "'\\015'" "<" "out/$1.actual" ">" "$1.expect"
exec tr -d '\015' < "out/$1.actual" > "$1.expect"

# EOF
