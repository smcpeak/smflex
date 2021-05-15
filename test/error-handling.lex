/* error-handling.lex */
/* Exercise handling of errors. */

%option nodefault
%option yy_scan_string
%option yy_scan_bytes
%option yy_scan_buffer

%%

username      printf("USERNAME");

%%

static int my_read_input(yy_lexer_t *yy_lexer,
  void *dest, int size)
{
  return -1;
}

static yy_error_code_t ecode;
static char const *edetail;

static void record_error(yy_lexer_t *yy_lexer,
                         yy_error_code_t code, char const *detail)
{
  ecode = code;
  edetail = detail;
}

static void input_error()
{
  int t;

  yy_lexer_t lexer;
  yy_construct(&lexer);
  lexer.yy_error_function = &record_error;
  lexer.yy_read_input_function = &my_read_input;

  assert(yy_get_error(&lexer) == yy_err_no_error);

  /* Will hit an immediate input error. */
  t = yy_lex(&lexer);
  assert(t == 0);

  /* Validate the error reporting. */
  assert(ecode == yy_err_input_error);
  assert(0==strcmp(yy_error_string(ecode), "error reading input source"));
  assert(edetail == NULL);
  assert(yy_get_error(&lexer) == ecode);

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
}

static void no_rule_matches()
{
  int t;
  yy_buffer_state_t *buf;

  yy_lexer_t lexer;
  yy_construct(&lexer);
  lexer.yy_error_function = &record_error;

  buf = yy_scan_string(&lexer, "something");
  assert(buf);

  assert(yy_get_error(&lexer) == yy_err_no_error);

  /* Will hit "no rule matches". */
  t = yy_lex(&lexer);
  assert(t == 0);

  /* Validate the error reporting. */
  assert(ecode == yy_err_no_rule_matches);
  assert(0==strcmp(yy_error_string(ecode), "no rule matches the input text"));
  assert(edetail == NULL);
  assert(yy_get_error(&lexer) == ecode);

  yy_delete_buffer(&lexer, buf);
  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
}


int main()
{
  input_error();
  no_rule_matches();
  return 0;
}
