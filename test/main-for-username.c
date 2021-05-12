/* testmain.c */
/* Driver main() for testing the 'username' scanner. */

#include "out/username.yy.h"           /* yy_lexer_t, yylex */

int main(int argc, char **argv)
{
  yy_lexer_t lexer;
  yy_construct(&lexer);

  /* Keep retrieving tokens and discarding them, only performing
   * the actions specified in the .lex file, until EOF. */
  while (yylex(&lexer) != 0) {
    /* nothing */
  }

  yy_destroy(&lexer);
  return 0;
}

/* EOF */
