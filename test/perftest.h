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

#include <stdio.h>                     /* FIlE */

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
  while (lexer.yym_lex())
    {}

#else
  yy_lexer_t lexer;
  yy_construct(&lexer);

  /* Open a named file, which is how I expect a real scanner would
   * usually operate. */
  FILE *fp = fopen(fname, "rb");
  if (!fp) {
    printf("failed to open file \"%s\"\n", fname);
    exit(2);
  }

  yyrestart(&lexer, fp);
  while (yylex(&lexer))
    {}

  fclose(fp);
  yy_destroy(&lexer);
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
    while (lexer.yym_lex())
      {}

    // Make sure everything is flushed from cout before we go back to
    // using stdio printf.
    cout.flush();
#else
    yy_lexer_t lexer;
    yy_construct(&lexer);
    while (yylex(&lexer))
      {}
    yy_destroy(&lexer);
#endif
  }

  return iters;
}


/* What 'strncpy' should have been: write up to 'len-1' characters
 * of 'src' into 'dest', stopping after writing a NUL, and always
 * writing a NUL.  Do nothing if 'len' is 0. */
static void my_strncpy(char *dest, char const *src, size_t len)
{
  if (len > 0) {
    /* Last character in 'dest' we can write to. */
    char *destLast = dest+len-1;

    while (*src != '\0' && dest < destLast) {
      *(dest++) = *(src++);
    }

    /* Either '*src==0' or 'dest==destLast' (or both).  Write the
     * final NUL. */
    *dest = '\0';
  }
}


#endif /* PERFTEST_H */
