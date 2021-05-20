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

  yy_restart(&lexer, stdin);
  yy_set_interactive(&lexer, isatty(fileno(lexer.yy_input_stream)) > 0);
  while (yy_lex(&lexer))
    {}

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}

/* EOF */
