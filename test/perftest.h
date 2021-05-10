/* perftest.h */
/* Meant to be included at the top of a lexer meant for performance
 * testing. It provides the routines to run the lexer on the input
 * file with optional iteration, etc. */
/* See ../COPYING for license terms. */

#ifndef PERFTEST_H
#define PERFTEST_H

#if defined(yyFlexLexer_CLASS_DEFINED) || defined(__FLEX_LEXER_H)

/* We are using the C++ interface. */
#  define USING_CPP_INTERFACE
#  include <fstream>
#  include <iostream>
using std::ifstream;
using std::cerr;
using std::cout;
#else

/* We are using the C interface.  'yylex' has not been declared
 * yet, but we want to call it below, so declare it now. */
int yylex();

#endif

#ifdef FLEX_SCANNER
/* Using 'flex', which does not declare 'read' properly. */
#include <unistd.h>                    /* read */
#endif


static void scanFile(char const *fname)
{
#ifdef USING_CPP_INTERFACE
  ifstream in(fname, std::ios::binary);
  if (!in) {
    cerr << "failed to open input file \"" << fname << "\"\n";
    exit(2);
  }

  yyFlexLexer lexer(&in);
  while (lexer.yylex())
    {}

#else
  /* Open a named file, which is how I expect a real scanner would
   * usually operate. */
  FILE *fp = fopen(fname, "rb");
  if (!fp) {
    printf("failed to open file \"%s\"\n", fname);
    exit(2);
  }

  yyrestart(fp);

  while (yylex())
    {}

  fclose(fp);

#endif
}


/* Run the core of a performance test program. */
static int runPerftest(int argc, char **argv)
{
  int iters = 1;

  if (argc >= 2) {
    /* Read a named file. */
    char const *inputFname = argv[1];

    if (argc >= 3) {
      /* Specify number of iterations. */
      iters = atoi(argv[2]);
      printf("iters: %d\n", iters);
    }

    for (int i=0; i < iters; i++) {
      scanFile(inputFname);
    }
  }
  else {
    /* Read from stdin.  By allowing both stdin and named files, I can
     * see if it makes any speed difference. */
#ifdef USING_CPP_INTERFACE
    yyFlexLexer lexer;
    while (lexer.yylex())
      {}

    // Make sure everything is flushed from cout before we go back to
    // using stdio printf.
    cout.flush();
#else
    while (yylex())
      {}

#endif
  }

  return iters;
}


#endif /* PERFTEST_H */
