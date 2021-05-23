/* begin-special.lex */
/* Illustrate calling BEGIN on entry to yy_lex(). */

%smflex 100
%option flex-compat

%{
#include <assert.h>                    /* assert */
%}

                int enter_special;

%x SPECIAL
%%

%{
                /* Code inserted at the top of yy_lex(). */
                if (enter_special) {
                  BEGIN(SPECIAL);

                  /* Check the yyin and yyout aliases. */
                  assert(YY_INPUT_STREAM == yyin);
                  assert(YY_OUTPUT_STREAM == yyout);
                }
%}

<SPECIAL>token  {      /* ... etc. ... */
                  printf("saw 'token' in SPECIAL state\n");
                  return 1;
                }
token           {
                  printf("saw 'token' in INITIAL state\n");
                  return 2;
                }
<*>.|\n         /* discard */

%%

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);

  /* Repeatedly toggle 'enter_special' until EOF. */
  enter_special = 0;
  while (yy_lex(&lexer)) {
    enter_special = !enter_special;

    /* Start each time in INITIAL, so that 'enter_special' can either
     * change it or not. */
    yy_set_start_condition(&lexer, INITIAL);
  }

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
