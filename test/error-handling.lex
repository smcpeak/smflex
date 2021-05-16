/* error-handling.lex */
/* Exercise handling of errors. */

%option nodefault
%option yy_scan_string

%%

username      printf("USERNAME");

%%

static int read_input_fail(yy_lexer_t *yy_lexer,
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


static void provoke_error(yy_error_code_t expect_code)
{
  int t;
  yy_lexer_t lexer;

  yy_construct(&lexer);
  lexer.yy_error_function = &record_error;
  assert(yy_get_error(&lexer) == yy_err_no_error);

  switch (expect_code) {
    case yy_err_input_error: {
      lexer.yy_read_input_function = &read_input_fail;

      /* Will hit an immediate input error. */
      t = yy_lex(&lexer);
      assert(t == 0);

      assert(0==strcmp(yy_error_string(ecode), "error reading input source"));
      assert(edetail == NULL);
      break;
    }

    case yy_err_no_rule_matches: {
      yy_buffer_state_t *buf = yy_scan_string(&lexer, "something");
      assert(buf);

      /* Will hit "no rule matches". */
      t = yy_lex(&lexer);
      assert(t == 0);

      assert(0==strcmp(yy_error_string(ecode), "no rule matches the input text"));
      assert(edetail == NULL);
      break;
    }

    default:
      assert(!"unhandled code");
  }

  /* Validate the error reporting. */
  assert(ecode == expect_code);
  assert(yy_get_error(&lexer) == expect_code);

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
}


int main()
{
  provoke_error(yy_err_input_error);
  provoke_error(yy_err_no_rule_matches);
  return 0;
}
