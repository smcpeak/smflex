#!/usr/bin/env python3
# See COPYING for license terms.

"""check-line-directives.py

Given a C source file that contains #line directives, check them for
accuracy.  Specifically:

1. When a #line directive refers to another file, check that the lines
line after it, until the next #line directive or EOF, each contain text
that appears on the corresponding line in the other file.  For example,
if we are checking "generated.c", then if we see:

  #line 10 "some-file.c"
  int x = 3;

then we will check that line 10 of some-file.c contains "int x = 3;"
as a substring, and so on for subsequent lines.

2. When a #line directiv refers to the containing file, check that the
line number is correct for its location.  For example, again if checking
"generated.c", if we wee:

  #line 34 "generated.c"

then we will check that that #line directive in fact appears on line 33
of generated.c (33 instead of 34 because the *next* line is 34).

If all checks pass, exit with code 0.  Otherwise print what problems
were encountered and exit with code 2.

"""

import re                              # compile
import sys                             # argv, exit, stderr
import textwrap                        # dedent

programName = sys.argv[0]


# Number of errors and warnings encountered.
numErrors = 0
numWarnings = 0


def die(msg):
  """Print 'msg' and exit with code 2.

  'msg' will have any indentation common to all lines removed first."""

  print(textwrap.dedent(msg), file=sys.stderr)
  sys.exit(2)


def complain(msg):
  """Print 'msg' as an error, but keep going."""

  print("----------------------")
  print(textwrap.dedent(msg))
  print("")

  global numErrors
  numErrors += 1


def warn(msg):
  """Print 'msg' as a warning."""

  print("----------------------")
  print(textwrap.dedent(msg));
  print("")

  global numWarnings
  numWarnings += 1


def chomp(x):
  """Remove any trailing line terminator from 'x' and return that."""

  if x.endswith("\r\n"): return x[:-2]
  if x.endswith("\n") or x.endswith("\r"): return x[:-1]
  return x



# Map from file name to list of lines.
nameToContents = {}


def getFileContents(fname):
  """Return a list of the lines in 'fname'.

  If the file cannot be read, raises IOException."""

  global nameToContents

  # Read the file if we have not done so already.
  if fname not in nameToContents:
    with open(fname, "r") as fp:
      nameToContents[fname] = fp.readlines()

  return nameToContents[fname]


# Interpret the command line.
if len(sys.argv) < 2:
  die(f"""\
    usage: {programName} <file to check>

    Checks the #line directives in the input file for accuracy.""")

inputFileName = sys.argv[1]


# Read the main input file.
inputFileLines = []
with open(inputFileName, "r") as fp:
  inputFileLines = fp.readlines()


# Counts of various kinds of lines.
numInputLines = len(inputFileLines)
directivesWithSameFile = 0
directivesWithDifferentFile = 0


# Pattern for recognizing #line directives.
lineDirectivePattern = re.compile(r'^\s*#line\s+([0-9]+)\s+"(.*)"\s*$')

# Pattern for recognizing blank lines.
blankPattern = re.compile(r'^\s*$')


# Check all lines for #line directives.
for inputLineNumber, inputLine in enumerate(inputFileLines, start=1):
  inputLine = chomp(inputLine)

  m = lineDirectivePattern.match(inputLine)
  if m:
    namedLineNumber = int(m.group(1))
    namedFile = m.group(2)

    if namedFile == inputFileName:
      directivesWithSameFile += 1

      # Check that the line number is right.
      if namedLineNumber != inputLineNumber+1:
        complain(f"""\
          Line {inputLineNumber} of {inputFileName} is:

            {inputLine}

          Since the following line is number {inputLineNumber+1}, the line
          number in the directive should be that value.""")

    else:
      directivesWithDifferentFile += 1

      # Read the named source file.
      try:
        sourceFileLines = getFileContents(namedFile)
      except IOError as e:
        complain(f"""\
          Line {inputLineNumber} of {inputFileName} is:

            {inputLine}

          but attempting to read "{namedFile}" failed:

            {e}""")
        break

      # Check that the line number is within range.
      if 1 <= namedLineNumber and namedLineNumber <= len(sourceFileLines):
        # Check all subsequent lines until the next #line or EOF.
        n = inputLineNumber + 1    # Number of line being checked.
        while n < len(inputFileLines):
          inputFileLine = chomp(inputFileLines[n - 1])

          # If hit #line, bail.
          if lineDirectivePattern.match(inputFileLine):
            break

          # Get the corresponding source file line number.
          sourceLineNumber = namedLineNumber + (n-1 - inputLineNumber)
          if sourceLineNumber > len(sourceFileLines):
            complain(f"""\
              Line {inputLineNumber} of {inputFileName} is:

                {inputLine}

              That section contains line {n}:

                {inputFileLine}

              The corresponding source code line would be line {sourceLineNumber},
              but {namedFile} only has {len(sourceFileLines)} lines.""")
            break

          # Get the corresponding source file line.
          sourceLine = chomp(sourceFileLines[sourceLineNumber - 1])

          # Check that line 'n' in the input file is a substring of
          # 'sourceLine'.
          if inputFileLine not in sourceLine:
            complain(f"""\
              Line {inputLineNumber} of {inputFileName} is:

                {inputLine}

              That section contains line {n}:

                {inputFileLine}

              The corresponding source code line is line {sourceLineNumber}:

                {sourceLine}

              The former is not a substring of the latter, which means
              these do not match.""")
            break

          n += 1

      else:
        complain(f"""\
          Line {inputLineNumber} of {inputFileName} is:

            {inputLine}

          It refers to line {namedLineNumber}, but that is out of range for
          {namedFile}, which has {len(sourceFileLines)} lines.""")

  else:
    # Not a #line directive line, keep going.
    pass


if numErrors > 0:
  # Print a final report of the statistics.
  print(textwrap.dedent(f"""\
    ----------------
    Scanning finished with {numErrors} errors and {numWarnings} warnings.
    Input file: {inputFileName}
    Number of lines: {numInputLines}
    #lines with same file: {directivesWithSameFile}
    #lines with different file: {directivesWithDifferentFile}"""))


if numErrors > 0:
  sys.exit(2)


# EOF
