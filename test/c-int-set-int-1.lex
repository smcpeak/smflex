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
  yy_lexer_t lexer;
  yy_construct(&lexer);

  lexer.yy_input_stream = stdin;
  yy_set_interactive(&lexer, 1);
  while (yylex(&lexer))
    {}

  yy_destroy(&lexer);
  return 0;
}

/* EOF */
