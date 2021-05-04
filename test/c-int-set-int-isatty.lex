/* c-int-set-int-isatty.lex */
/* C-interface scanner that calls 'yy_set_interactive()' based on
 * what 'isatty()' says. */

%option main

%{
#include <stdlib.h>        /* exit */
#include <unistd.h>        /* isatty */

#define YY_USER_INIT         \
  yyin = stdin;              \
  yy_set_interactive(isatty(fileno(yyin)) > 0);

%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
