#! /bin/sh
# Create a C source file containing a string literal that contains
# the contents of the given file.

if [ "x$3" = "x" ]; then
  echo "usage: $0 <contents file> <dest file> <array name>"
  echo ""
  echo "Creates <dest file>, a C source code file, which defines an "
  echo "array called <array name> whose contents is <contents file>."
  exit 2
fi

contents_file="$1"
dest_file="$2"
array_name="$3"

# File descriptor 3 is our output.
exec 3>"$dest_file"

cat >&3 <<EOF
/* File created from $contents_file via $0 */

/* NULL-terminated array of NUL-terminated lines, *without* line
 * terminators. */
const char *$array_name[] = {
EOF

# Step 1: Escape backslashes.
# Step 2: Escape double-quotes.
# Step 3: Surround every line with double-quotes.
sed 's/\\/&&/g' "$contents_file" | sed 's/"/\\"/g' | sed 's/.*/  "&",/' >&3

# A single NULL pointer terminates the array.
cat >&3 <<EOF
  0
};
EOF

# Close fd 3.
exec 3>&-

# EOF
