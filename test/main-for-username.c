/* testmain.c */
/* Driver main() for testing. */

/* Defined in the generated scanner. */
extern int yylex();

int main(int argc, char **argv)
{
  /* Keep retrieving tokens and discarding them, only performing
   * the actions specified in the .lex file, until EOF. */
  while (yylex() != 0) {
    /* nothing */
  }
  return 0;
}

/* EOF */
