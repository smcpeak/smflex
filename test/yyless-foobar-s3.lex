/* yyless-foobar-s3.lex */
/* Demonstrate yyless() from section 3. */

%option flex-compat

%%

foobar    YY_ECHO; return 1;
goobar    YY_ECHO; yyless(3);  /* also test flex compat alias */
[a-z]+    YY_ECHO;

%%

int main()
{
  int tok;
  yy_lexer_t lexer;

  yy_construct(&lexer);

  while ((tok = yy_lex(&lexer))) {
    if (tok == 1) {
      yy_less_text(&lexer, 3);
    }
  }

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
