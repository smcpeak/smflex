/* yylineno.lex */
/* Exercise %option yylineno. */

%option yylineno

%{

#include "perftest.h"                /* runPerftest */

/* Sum of all 'yylineno' when "username" is seen. */
int lineNoSum = 0;

%}

%%
username           lineNoSum += yylineno;
[^ \t\n!]+         /* Nothing. */
[ \t\n!]+          /* Nothing. */

%%

int main(int argc, char **argv)
{
  runPerftest(argc, argv);
  printf("lineNoSum: %d\n", lineNoSum);
  return 0;
}

/* EOF */