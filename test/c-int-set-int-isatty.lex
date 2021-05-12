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
  yy_lexer_t lexer;
  yy_construct(&lexer);

  lexer.yy_input_stream = stdin;
  yy_set_interactive(&lexer, isatty(fileno(lexer.yy_input_stream)) > 0);
  while (yylex(&lexer))
    {}

  yy_destroy(&lexer);
  return 0;
}

/* EOF */
