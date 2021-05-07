/* c-int-interactive-set-int-isatty.lex */
/* C-interface scanner that has %option interactive and calls
 * 'yy_set_interactive()' based on what 'isatty()' says. */

/* This is how one would create a scanner that is sometimes used
 * interactively but also should have reasonably good speed when
 * used non-interactively.  The tables are configured for
 * interactive use but the input method is determined at run time. */

%option interactive

%{
#include <stdlib.h>        /* exit */
#include <unistd.h>        /* isatty */
%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%

/* Unlike the other examples, for variety and perhaps better realism,
 * let's call 'yy_set_interactive' from 'main()' instead of in
 * 'YY_USER_INIT'. */
int main()
{
  /* BEGIN: example fragment */
  yyin = stdin;
  yy_set_interactive(isatty(fileno(yyin)) > 0);
  while (yylex())
    {}
  /* BEGIN: example fragment */
  return 0;
}


/* EOF */
