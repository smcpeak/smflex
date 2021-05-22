/* yy-lex-name-prefix.lex */
/* Exercise %option yy_lex_name and %option prefix. */

%smflex 100
%option yy_lex_name="my_custom_lexer_name"
%option prefix="my_custom_prefix"

%%

username    printf("USERNAME");

%%

int main()
{
  my_custom_prefix_lexer_t lexer;
  my_custom_prefix_construct(&lexer);
  while (my_custom_lexer_name(&lexer))
    {}
  my_custom_prefix_destroy(&lexer);
  return 0;
}
