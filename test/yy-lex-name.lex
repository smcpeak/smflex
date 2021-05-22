/* yy-lex-name.lex */
/* Exercise %option yy_lex_name. */

%smflex 100
%option yy_lex_name="my_custom_lexer_name"

%%

username    printf("USERNAME");

%%

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);
  while (my_custom_lexer_name(&lexer))
    {}
  yy_destroy(&lexer);
  return 0;
}
