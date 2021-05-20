/* toy-pascal.lex */
/* BEGIN: example fragment */
/* scanner for a toy Pascal-like language */

%{
#include <math.h>            /* atof */
#include <stdlib.h>          /* atoi */
%}

DIGIT    [0-9]
ID       [a-zA-Z][a-zA-Z0-9]*

%%

{DIGIT}+              {
                        printf("An integer: %s (%d)\n", YY_TEXT,
                               atoi(YY_TEXT));
                      }

{DIGIT}+"."{DIGIT}*   {
                        printf("A float: %s (%g)\n", YY_TEXT,
                               atof(YY_TEXT));
                      }

  /* Comments can go between rules, but must be indented. */
if|then|begin|end|procedure|function|program        {
                        printf("A keyword: %s\n", YY_TEXT);
                      }

{ID}                  printf("An identifier: %s\n", YY_TEXT);

"'"[^'\n]*"'"         printf("A string: %s\n", YY_TEXT);

[+*/()-]              printf("An operator: %s\n", YY_TEXT);
[;.,]                 printf("A punctuator: %s\n", YY_TEXT);

"{"[^}]*"}"           /* eat up comments, including multi-line */
[ \t\n]+              /* eat up whitespace */

.                     printf("Unrecognized character: %s\n", YY_TEXT);

%%

int main(int argc, char **argv)
{
  yy_lexer_t lexer;
  yy_construct(&lexer);
  yy_restart(&lexer, (argc >= 2)? fopen(argv[1], "r") : stdin);
  yy_lex(&lexer);
  yy_destroy(&lexer);
  return 0;
}
