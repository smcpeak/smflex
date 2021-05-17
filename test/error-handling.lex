/* error-handling.lex */
/* Exercise handling of errors. */

%{
#include <stdio.h>                     /* printf */
#include <string.h>                    /* strcmp, strstr */
%}

%option nodefault
%option yy_scan_string
%option yy_unread_character

%%

username      printf("USERNAME");
reject        REJECT;  /* Use REJECT to make buffer unexpandable. */
x+            printf("saw %d xs\n", yyleng);

unread        {
                /* Unread a lot of characters so we hit the limit. */
                int i;
                printf("saw 'unread'\n");
                for (i=0; i<17000; i++) {
                  /* Once we hit the limit, this will keep trying and
                   * failing to unread.  Should be fine. */
                  YY_UNREAD_CHARACTER('x');
                }
              }

%%

static int read_input_fail(yy_lexer_t *yy_lexer,
  void *dest, int size)
{
  return -1;
}


static yy_error_code_t ecode;
static char const *edetail;

static void record_error(yy_lexer_t const *yy_lexer,
                         yy_error_code_t code, char const *detail)
{
  ecode = code;
  edetail = detail;
}


static int remaining_xs = 0;

/* Generate 'remaining_xs' bytes of "x". */
static int read_input_xs(yy_lexer_t *yy_lexer,
  void *dest, int size)
{
  if (remaining_xs <= 0) {
    return 0;
  }

  if (size > remaining_xs) {
    size = remaining_xs;
  }

  memset(dest, 'x', size);
  remaining_xs -= size;
  return size;
}


static char const *fixed_string;
static int fixed_string_index;

/* Generate 'fixed_string'. */
static int read_input_fixed_string(yy_lexer_t *yy_lexer,
  void *dest, int size)
{
  int len = strlen(fixed_string);
  int remain = len - fixed_string_index;

  if (remain <= 0) {
    return 0;
  }

  if (size > remain) {
    size = remain;
  }

  memcpy(dest, fixed_string+fixed_string_index, size);
  fixed_string_index += size;
  return size;
}


static void provoke_error(yy_error_code_t expect_code)
{
  int t;
  yy_lexer_t lexer;

  printf("---- code %d ----\n", (int)expect_code);

  yy_construct(&lexer);
  lexer.yy_error_function = &record_error;
  assert(yy_get_error(&lexer) == yy_err_no_error);

  ecode = yy_err_no_error;
  edetail = NULL;

  switch (expect_code) {
    case yy_err_input_error: {
      lexer.yy_read_input_function = &read_input_fail;

      /* Will hit an immediate input error. */
      t = yy_lex(&lexer);
      printf("error: %s\n", yy_error_string(yy_get_error(&lexer)));
      assert(t == 0);

      assert(0==strcmp(yy_error_string(ecode),
        "error reading input source"));
      assert(edetail == NULL);
      break;
    }

    case yy_err_no_rule_matches: {
      yy_buffer_state_t *buf = yy_scan_string(&lexer, "something");
      assert(buf);

      /* Will hit "no rule matches". */
      t = yy_lex(&lexer);
      printf("error: %s\n", yy_error_string(yy_get_error(&lexer)));
      assert(t == 0);

      assert(0==strcmp(yy_error_string(ecode),
        "no rule matches the input text"));
      assert(edetail == NULL);
      break;
    }

    case yy_err_input_buffer_cannot_expand: {
      lexer.yy_read_input_function = &read_input_xs;
      remaining_xs = 100000;

      t = yy_lex(&lexer);
      printf("error: %s\n", yy_error_string(yy_get_error(&lexer)));
      assert(t == 0);

      assert(0==strcmp(yy_error_string(ecode),
        "token is too large and input buffer cannot be expanded"));
      assert(strstr(edetail, "REJECT"));
      break;
    }

    case yy_err_unread_overflow: {
      lexer.yy_read_input_function = &read_input_fixed_string;
      fixed_string = "unread";
      fixed_string_index = 0;

      t = yy_lex(&lexer);
      printf("error: %s\n", yy_error_string(yy_get_error(&lexer)));
      assert(t == 0);

      assert(0==strcmp(yy_error_string(ecode),
        "no space in buffer to unread a character"));
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
  provoke_error(yy_err_input_buffer_cannot_expand);
  provoke_error(yy_err_unread_overflow);
  return 0;
}
