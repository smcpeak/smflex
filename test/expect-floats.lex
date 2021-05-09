/* expect-floats.lex */
/* Illustrate use of start conditions. */

%option main

/* BEGIN: example fragment */
%{
#include <math.h>            /* atof */
#include <stdlib.h>          /* atoi */
%}
%s expect

%%
expect-floats       BEGIN(expect);

<expect>[0-9]+"."[0-9]+  {
                      printf("found a float: %f\n", atof(yytext));
                    }
<expect>\n          {
                      /* That's the end of the line, so we need another
                       * "expect-floats" before we'll recognize any more
                       * floats. */
                      BEGIN(INITIAL);
                    }

[0-9]+              {
                      printf("found an integer: %d\n", atoi(yytext));
                    }

"."                 printf("found a dot\n");
  /* END: example fragment */

.|\n                /* discard */
