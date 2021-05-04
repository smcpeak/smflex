#!/bin/sh
# Make a copy of a test.

if [ "x$2" = "x" ]; then
  echo "usage: $0 srcBaseName destBaseName"
  exit 2
fi

src="$1"
dest="$2"

# Copy $1 to $2 unless the former does not exist or the latter
# already exists.  Also substitute $dest for $src in the contents.
copy_if() {
  if [ -e "$1" ]; then
    if [ -e "$2" ]; then
      echo "$2 already exists, not copying."
    else
      echo sed -s "s/$src/$dest/g" "$1" '>' "$2"
      sed -s "s/$src/$dest/g" "$1" > "$2"

      # Turn off execute permission on cygwin.
      if [ -x "$2" ]; then
        chmod a-x "$2"
      fi
    fi
  fi
}

copy_if "$src.lex" "$dest.lex"
copy_if "$src.lexpp" "$dest.lexpp"
copy_if "$src.input" "$dest.input"
copy_if "$src.expect" "$dest.expect"

# EOF
