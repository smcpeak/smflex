/* scan-string-only.lex */
/* Use 'yy_scan_string' without the others. */

%option yy_scan_string

%%

username      printf("USERNAME");

%%

int main()
{
  yy_buffer_state_t *buf;

  yy_lexer_t lexer;
  yy_construct(&lexer);

  buf = yy_scan_string(&lexer, "something username else\n");

  while (yy_lex(&lexer))
    {}

  yy_delete_buffer(&lexer, buf);
  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
