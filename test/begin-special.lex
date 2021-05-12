/* begin-special.lex */
/* Illustrate calling BEGIN on entry to yylex(). */

                /* BEGIN: example fragment */
                int enter_special;

%x SPECIAL
%%
                if (enter_special) {
                  BEGIN(SPECIAL);
                }

<SPECIAL>token  {      /* ... etc. ... */
                  /* END: example fragment */
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
  while (yylex(&lexer)) {
    enter_special = !enter_special;

    /* Start each time in INITIAL, so that 'enter_special' can either
     * change it or not. */
    yy_begin(&lexer, INITIAL);
  }

  yy_destroy(&lexer);
  return 0;
}
