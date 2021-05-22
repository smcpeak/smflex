/* c-int-interactive-set-int-0.lex */
/* C-interface scanner with %option interactive that calls
 * 'yy_set_interactive(0)'. */

/* This covers the case of interactive tables but fast reads. */

%smflex 100
%option interactive

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
  yy_set_interactive(&lexer, 0);
  while (yy_lex(&lexer))
    {}
  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}

/* EOF */
