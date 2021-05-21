/* yy-lex-params.lex */
/* Exercise %option yy_lex_parameters. */

%option yy_lex_parameters="int x, int y"

%%

username    printf("USERNAME [x=%d,y=%d]", x, y);

%%

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);
  while (yy_lex(&lexer, 3, 4))
    {}
  yy_destroy(&lexer);
  return 0;
}
