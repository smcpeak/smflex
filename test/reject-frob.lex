/* reject-frob.lex */
/* First example in REJECT section of the manual. */

%{

#include "perftest.h"                  /* runPerftest */

static void special(void);

%}

        /* BEGIN: example fragment */
        int word_count = 0;
%%

frob        special(); REJECT;
[^ \t\n]+   ++word_count;
        /* END: example fragment */

[ \t\n]     /* Nothing; do not echo. */

%%

static int frob_count = 0;

static void special()
{
  frob_count++;
}

int main(int argc, char **argv)
{
  runPerftest(argc, argv);

  printf("word_count: %d\n", word_count);
  printf("frob_count: %d\n", frob_count);
  return 0;
}


/* EOF */
