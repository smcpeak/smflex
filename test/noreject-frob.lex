/* noreject-frob.lex */
/* Like reject-frob.lex, but without YY_REJECT. */

%smflex 100
%{

#include "perftest.h"                  /* runPerftest */

static void special(void);

%}

        int word_count = 0;
%%

frob        special();
[^ \t\n]+   ++word_count;

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
