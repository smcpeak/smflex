/* scan-string-only.lex */
/* Use 'yy_scan_string' without the others. */

%smflex 100
%option yy_scan_string

%%

username      printf("USERNAME");

%%

static yy_error_code_t ecode;
static char const *edetail;

static void record_error(yy_lexer_t const *yy_lexer,
                         yy_error_code_t code, char const *detail)
{
  ecode = code;
  edetail = detail;
}

/* Defined in the generated scanner when
 * YY_ALLOCATOR_FAILURE_INJECTION is defined. */
extern unsigned yy_alloc_failure_countdown;

static void scan()
{
  yy_buffer_state_t *buf;
  yy_lexer_t lexer;

  printf("-------- scan --------\n");

  yy_construct(&lexer);
  lexer.yy_error_function = &record_error;

  buf = yy_scan_string(&lexer, "something username else\n");

  /* Only try to scan if we do not have errors yet. */
  if (!yy_get_error(&lexer)) {
    while (yy_lex(&lexer))
      {}
  }

  if (yy_get_error(&lexer)) {
    printf("lexing failed: %s\n", yy_error_string(yy_get_error(&lexer)));
  }

  yy_delete_buffer(&lexer, buf);
  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
}

int main()
{
  scan();

  yy_alloc_failure_countdown = 1;
  scan();

  yy_alloc_failure_countdown = 2;
  scan();

  yy_alloc_failure_countdown = 3;
  scan();

  return 0;
}
