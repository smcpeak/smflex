/* c-int-interactive-set-int-0.lex */
/* C-interface scanner with %option interactive that calls
 * 'yy_set_interactive(0)'. */

/* This covers the case of interactive tables but fast reads. */

%option interactive
%option main

%{
#include <stdlib.h>        /* exit */

#define YY_USER_INIT         \
  yyin = stdin;              \
  yy_set_interactive(0);

%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
