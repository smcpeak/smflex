/* c-int-set-int-1.lex */
/* C-interface scanner that calls 'yy_set_interactive(1)'. */

%smflex 100
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

  yy_restart(&lexer, stdin);
  yy_set_interactive(&lexer, 1);
  while (yy_lex(&lexer))
    {}

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}

/* EOF */
