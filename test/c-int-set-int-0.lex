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
  yy_lexer_t lexer;
  yy_construct(&lexer);

  lexer.yy_input_stream = stdin;
  yy_set_interactive(&lexer, 0);
  while (yy_lex(&lexer))
    {}

  yy_destroy(&lexer);
  return 0;
}

/* EOF */
