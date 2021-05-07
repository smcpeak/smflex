/* c-int-set-int-isatty.lex */
/* C-interface scanner that calls 'yy_set_interactive()' based on
 * what 'isatty()' says. */

%{
#include <stdlib.h>        /* exit */
#include <unistd.h>        /* isatty */
%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%

int main()
{
  yyin = stdin;
  yy_set_interactive(isatty(fileno(yyin)) > 0);
  while (yylex())
    {}
  return 0;
}

/* EOF */
