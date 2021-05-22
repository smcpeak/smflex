/* scan-bytes-only.lex */
/* Use 'yy_scan_bytes' without the others. */

%smflex 100
%option yy_scan_bytes

%{
#include <string.h>                    /* string */
%}

%%

username      printf("USERNAME");

%%

char const *str = "something username else\n";

int main()
{
  yy_buffer_state_t *buf;

  yy_lexer_t lexer;
  yy_construct(&lexer);

  buf = yy_scan_bytes(&lexer, str, strlen(str));

  while (yy_lex(&lexer))
    {}

  yy_delete_buffer(&lexer, buf);
  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
