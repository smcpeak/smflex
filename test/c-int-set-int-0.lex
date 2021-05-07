/* c-int-set-int-0.lex */
/* C-interface scanner that calls 'yy_set_interactive(0)'. */

%{
#include <stdlib.h>        /* exit */
%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%

int main()
{
  yyin = stdin;
  yy_set_interactive(0);
  while (yylex())
    {}
  return 0;
}

/* EOF */
