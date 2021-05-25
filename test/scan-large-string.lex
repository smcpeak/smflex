/* scan-large-string.lex */
/* Exercise 'yy_scan_string' with a large string and %option reject. */

%smflex 100
%option yy_scan_string
%option reject

%%

.*     printf("saw line with %d characters\n", YY_LENG);

%%

/* TODO: This fails when SZ is 100000. */
#define SZ 10000

int main()
{
  yy_lexer_t lexer;
  char *large_string;

  large_string = malloc(SZ+1);
  memset(large_string, 'x', SZ);
  large_string[SZ] = 0;

  yy_construct(&lexer);
  yy_scan_string(&lexer, large_string);

  while (yy_lex(&lexer))
    {}

  yy_destroy(&lexer);
  return 0;
}
