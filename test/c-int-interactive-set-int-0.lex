/* c-int-interactive-set-int-0.lex */
/* C-interface scanner with %option interactive that calls
 * 'yy_set_interactive(0)'. */

/* This covers the case of interactive tables but fast reads. */

%option interactive

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
