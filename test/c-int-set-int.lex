/* c-int-set-int.lex */
/* C-interface scanner that calls 'yy_set_interactive(1)'. */

%option main

%{
#include <stdlib.h>        /* exit */

/* I should not have to set 'yyin' here, but I do.  I do not know
 * how 'yy_set_interactive' is meant to be used; the manual is not
 * much help. */
#define YY_USER_INIT         \
  yyin = stdin;              \
  yy_set_interactive(1);

%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
