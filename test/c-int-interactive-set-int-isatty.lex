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

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);

  /* BEGIN: example fragment */
  lexer.yy_input_stream = stdin;
  yy_set_interactive(&lexer, isatty(fileno(stdin)) > 0);
  while (yy_lex(&lexer))
    {}
  /* END: example fragment */

  yy_destroy(&lexer);
  return 0;
}


/* EOF */
