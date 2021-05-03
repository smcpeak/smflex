#!/usr/bin/env python3

"""check-line-directives.py

Given a C source file that contains #line directives, check them for
accuracy.  Specifically:

1. When a #line directive refers to another file, check that the first
non-blank line after it contains text that appears on the corresponding
line in the other file.  For example, if we are checking "generated.c",
then if we see:

  #line 10 "some-file.c"
  int x = 3;

then we will check that line 10 of some-file.c contains "int x = 3;"
as a substring.

2. When a #line directiv refers to the containing file, check that the
line number is correct for its location.  For example, again if checking
"generated.c", if we wee:

  #line 34 "generated.c"

then we will check that that #line directive in fact appears on line 33
of generated.c (33 instead of 34 because the *next* line is 34).

If all checks pass, exit with code 0.  Otherwise print what problem
was encountered and exit with code 2.

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
  """Return a list of the lines in 'fname'."""

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
linePattern = re.compile(r'^\s*#line\s+([0-9]+)\s+"(.*)"\s*$')

# Pattern for recognizing blank lines.
blankPattern = re.compile(r'^\s*$')


# Check all lines for #line directives.
for inputLineNumber, inputLine in enumerate(inputFileLines, start=1):
  inputLine = chomp(inputLine)

  m = linePattern.match(inputLine)
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
      sourceFileLines = getFileContents(namedFile)

      # Check that the line number is within range.
      if 1 <= namedLineNumber and namedLineNumber <= len(sourceFileLines):
        # Look for the first non-blank line after the #line directive
        # in the input file.
        n = inputLineNumber + 1    # Line number of non-blank line.
        while (n < len(inputFileLines) and
               blankPattern.match(inputFileLines[n - 1])):
          n += 1

        # Found one?
        if n < len(inputFileLines):
          inputFileLine = chomp(inputFileLines[n - 1])

          # Get the corresponding source file line number.
          sourceLineNumber = namedLineNumber + (n-1 - inputLineNumber)
          if sourceLineNumber > len(sourceFileLines):
            complain(f"""\
              Line {inputLineNumber} of {inputFileName} is:

                {inputLine}

              The first non-blank line after that is line {n}:

                {inputFileLine}

              The corresponding source code line would be line {sourceLineNumber},
              but {namedFile} only has {len(sourceFileLines)} lines.""")

          # Get the corresponding source file line.
          sourceLine = chomp(sourceFileLines[sourceLineNumber - 1])

          # Check that line 'n' in the input file is a substring of
          # 'sourceLine'.
          if inputFileLine not in sourceLine:
            complain(f"""\
              Line {inputLineNumber} of {inputFileName} is:

                {inputLine}

              The first non-blank line after that is line {n}:

                {inputFileLine}

              The corresponding source code line is line {sourceLineNumber}:

                {sourceLine}

              The former is not a subtring of the latter, which means
              these do not match.""")

        else:
          warn(f"""\
            Warning: Line {inputLineNumber} of {inputFileName} is:

              {inputLine}

            It not followed by any non-blank lines.
            """)

      else:
        complain(f"""\
          Line {inputLineNumber} of {inputFileName} is:

            {inputLine}

          It refers to line {namedLineNumber}, but that is out of range for
          {namedFile}, which has {len(sourceFileLines)} lines.""")

  else:
    # Not a #line directive line, keep going.
    pass


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
