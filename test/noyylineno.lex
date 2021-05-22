/* noyylineno.lex */
/* Baseline for performance comparison with yylineno.lex. */

%smflex 100
%{

#include "perftest.h"                /* runPerftest */

/* Count of 'username' seen. */
int usernameCount = 0;

%}

%%
username           usernameCount++;
[^ \t\n!]+         /* Nothing. */
[ \t\n!]+          /* Nothing. */

%%

int main(int argc, char **argv)
{
  runPerftest(argc, argv);
  printf("usernameCount: %d\n", usernameCount);
  return 0;
}

/* EOF */
