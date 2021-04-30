#!/usr/bin/env python3
# Modify HTML output of pandoc flex.1.

import re
import subprocess
import sys

# Map from regexp to its compiled pattern and the number of times it
# has matched.
regexpMap = {}

# Count of debug printouts, for easier reference when re-running.
debugPrintCount = 0

def replace(input, regexp, replacement, debug=False):
  def replaceFunc(m):
    orig = m.group(0)
    repl = m.expand(replacement)
    if debug:
      global debugPrintCount
      print(f"{debugPrintCount}: {orig}")
      print(f"{debugPrintCount}: {repl}")
      print()
      debugPrintCount += 1
    return repl

  if regexp not in regexpMap:
    pattern = re.compile(regexp)
    regexpMap[regexp] = {
      "pattern": pattern,
      "replacement": replacement,
      "count": 0,
    }
  entry = regexpMap[regexp]
  (result, n) = re.subn(entry["pattern"], replaceFunc, input)
  entry["count"] += n
  return result

paragraphPattern = re.compile(r'^(\S*)(<p>.*)</p>$')
wrappedParagraphs = 0

def wrapParagraph(line):
  m = paragraphPattern.match(line)
  if m:
    prefix = m.group(1)
    paragraph = m.group(2)
    formatted = subprocess.check_output("fmt", text=True,
                                        input=paragraph)

    # Indentation for lines after the first.
    indent = " " * len(prefix)

    result = ""
    for index, fline in enumerate(formatted.splitlines()):
      if index == 0:
        result += prefix + fline + "\n"
      else:
        result += indent + fline + "\n"

    if False:
      print(f"{line}")
      print(f"{formatted}")
      print(f"{result}")
      sys.exit(0)

    global wrappedParagraphs
    wrappedParagraphs += 1
    return result

  else:
    return line

headingPattern = re.compile(r'^<h1>(.*)</h1>$')
headingsAnchored = 0

def addHeadingAnchor(line):
  m = headingPattern.match(line)
  if m:
    heading = m.group(1)
    anchor = heading.lower()
    anchor = anchor.translate(str.maketrans(" /+", "-np"))

    result = f'<h1 id="{anchor}">{heading}</h1>\n'

    if False:
      #print(f"{line}")
      print(f"{result}", end='')
      #sys.exit(0)

    global headingsAnchored
    headingsAnchored += 1
    return result

  else:
    return line

with open('flex.orig.html', 'r') as input:
  with open('flex.html', 'w') as output:
    for lineNumber, line in enumerate(input):
      #print(f"Line {lineNumber:4}: {line}", end='')

      # ---- <em> ----
      # Fix one case where both close-paren and comma are inside em.
      line = replace(line, r'<em>yyin\),</em>',
                           r'<em>yyin</em>),')

      # Fix both with ").".
      line = replace(line, r'\)[.]</em>',
                           r'</em>).')

      # Move unbalanced close-paren outside <em>
      line = replace(line, r'<em>([^(]+)\)</em>',
                           r'<em>\1</em>)')

      # Move commas, periods, and colons outside em.
      line = replace(line, r'<em>([^<]+)([,.:])</em>',
                           r'<em>\1</em>\2')

      # Move two semicolons outside.
      line = replace(line, r'<em>(branch|yyin)(;)</em>',
                           r'<em>\1</em>\2')

      # Fix apostrophes in <em>.
      line = replace(line, r"'s</em>",
                           r"</em>'s")

      # Fix names that should be treated as code.
      line = replace(line, r'<em>(flex|lex)</em>',
                           r'<code>\1</code>')

      # More names, lower volume.
      line = replace(line, r'<em>(-Cf|-CF|yyleng|n|c|yytext|input)</em>',
                           r'<code>\1</code>')

      # Yet more.
      line = replace(line, r'<em>(lexscan|yyin|yyout|return|getc\(\)|max_size|buf|result)</em>',
                           r'<code>\1</code>')

      # More...
      line = replace(line, r'<em>(bar|enter_special|comment_caller|SCs|&lt;SCs&gt;|FILE)</em>',
                           r'<code>\1</code>')

      # ---- <strong> ----
      # Fix one weird case with <strong>.
      line = replace(line, r'<strong>&lt;s&gt;, /,</strong>',
                           r'<code>&lt;s&gt;</code>, <code>/</code>,')

      # Move puncutation outside <strong>.
      line = replace(line, r'<strong>([^<]+)([,.])</strong>',
                           r'<strong>\1</strong>\2')

      # Move unbalanced close-paren outside <strong>
      line = replace(line, r'<strong>([^(]+)\)</strong>',
                           r'<strong>\1</strong>)')

      # Double close paren: move one outside.
      line = replace(line, r'\)\)</strong>',
                           r')</strong>)')

      # Fix apostrophes in <strong>.  The apostrophe should not be
      # there, and the 's' should move outside.
      line = replace(line, r"'s</strong>",
                           r"</strong>s")

      # Most things in <strong> are code.
      line = replace(line, r'<strong>([^<]+)</strong>',
                           r'<code>\1</code>')

      # Special case.
      line = replace(line, r'<code>yy_scan_string\(\), yy_scan_bytes\(\)</code>',
                           r'<code>yy_scan_string()</code>, <code>yy_scan_bytes()</code>')

      # ---- other ----
      # Fix max-width.
      line = replace(line, r'max-width: 36em;',
                           r'max-width: 60em;')

      # Add anchors to headings.
      line = addHeadingAnchor(line)

      # Specify natural language to pacify HTML validator.
      line = replace(line, r'lang="" xml:lang=""',
                           r'lang="en" xml:lang="en"')

      # Wrap paragraphs, remove </p> tag.  This is slow.
      line = wrapParagraph(line)

      # Blank lines before most elements.
      line = replace(line, r'^<(p|ul|pre|blockquote|li|ul|/ul|h1|dl|dt|dd|/dl)>',
                           "\n<\\1>")

      print(line, file=output, end='')

for regexp in sorted(regexpMap):
  entry = regexpMap[regexp]
  count = entry["count"]
  replacement = entry["replacement"]
  replacement = replacement.replace("\n", "\\n")
  print(f"count={count} regexp={regexp} replacement={replacement}")

print(f"wrappedParagraphs: {wrappedParagraphs}")
print(f"headingsAnchored: {headingsAnchored}")

# EOF
