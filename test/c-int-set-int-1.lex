/* c-int-set-int-1.lex */
/* C-interface scanner that calls 'yy_set_interactive(1)'. */

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
  yy_set_interactive(1);
  while (yylex())
    {}
  return 0;
}

/* EOF */
