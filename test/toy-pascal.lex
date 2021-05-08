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
                        printf("An integer: %s (%d)\n", yytext,
                               atoi(yytext));
                      }

{DIGIT}+"."{DIGIT}*   {
                        printf("A float: %s (%g)\n", yytext,
                               atof(yytext));
                      }

if|then|begin|end|procedure|function|program        {
                        printf("A keyword: %s\n", yytext);
                      }

{ID}                  printf("An identifier: %s\n", yytext);

"'"[^'\n]*"'"         printf("A string: %s\n", yytext);

[+*/()-]              printf("An operator: %s\n", yytext);
[;.,]                 printf("A punctuator: %s\n", yytext);

"{"[^}]*"}"           /* eat up comments, including multi-line */
[ \t\n]+              /* eat up whitespace */

.                     printf("Unrecognized character: %s\n", yytext);

%%

int main(int argc, char **argv)
{
  yyin = (argc >= 2)? fopen(argv[1], "r") : stdin;
  yylex();
  return 0;
}
