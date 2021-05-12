/* scan-string-etc.lex */
/* Exercise 'scan_string' and friends. */

%{
#include <assert.h>          /* assert */
#include <stdio.h>           /* fread */
%}

%%
username    printf("USERNAME");

%%

int main()
{
  char data[4096];
  yy_lexer_t lexer;

  /* Read stdin into memory. */
  size_t size = fread(data, 1, sizeof(data)-2, stdin);

  /* Place the required sentinel for 'yy_scan_buffer'. */
  data[size] = 0;
  data[size+1] = 0;

  yy_construct(&lexer);

  printf("Scanning with 'yy_scan_string'...\n");
  YY_BUFFER_STATE buf = yy_scan_string(&lexer, data);
  while (yylex(&lexer))
    {}
  yy_delete_buffer(&lexer, buf);

  printf("Scanning with 'yy_scan_bytes'...\n");
  buf = yy_scan_bytes(&lexer, data, size);
  while (yylex(&lexer))
    {}
  yy_delete_buffer(&lexer, buf);

  /* This must be done last because it will destroy the contents of
   * 'data'. */
  printf("Scanning with 'yy_scan_buffer'...\n");
  buf = yy_scan_buffer(&lexer, data, size+2);
  assert(buf);
  while (yylex(&lexer))
    {}
  yy_delete_buffer(&lexer, buf);

  return 0;
}
