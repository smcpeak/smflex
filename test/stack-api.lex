/* stack-api.lex */
/* Exercise the "%option stack" API. */

%{
#include <assert.h>                    /* assert */
#include <stdio.h>                     /* printf */
#include <stdlib.h>                    /* getenv, atoi */
#include <string.h>                    /* strcmp */
%}

%option stack
%option yy_top_state

%%

  /* No rules.  We do not actually call 'yy_lex()'. */

%%

static yy_error_code_t ecode;
static char const *edetail;

static void ignore_error(yy_lexer_t *yy_lexer,
                         yy_error_code_t code, char const *detail)
{
  ecode = code;
  edetail = detail;
}

int main()
{
  int i;
  int iters = 100;
  yy_lexer_t lexer;
  char *p;

  /* This is for manual performance testing. */
  p = getenv("ITERS");
  if (p) {
    iters = atoi(p);
    printf("iters: %d\n", iters);
  }

  yy_construct(&lexer);
  assert(yy_get_start_state(&lexer) == 0);

  for (i=1; i < iters; i++) {
    yy_push_state(&lexer, i);
    assert(yy_get_start_state(&lexer) == i);
  }

  for (i = iters-1; i >= 1; i--) {
    assert(yy_get_start_state(&lexer) == i);
    assert(yy_top_state(&lexer) == i-1);
    yy_pop_state(&lexer);
  }

  assert(yy_get_start_state(&lexer) == 0);

  /* Deliberately cause an "API misuse" error. */
  assert(yy_get_error(&lexer) == yy_err_no_error);
  lexer.yy_error_function = &ignore_error;
  yy_pop_state(&lexer);

  /* Validate the error reporting. */
  assert(ecode == yy_err_api_misuse);
  assert(0==strcmp(yy_error_string(ecode), "API misused"));
  assert(0==strcmp(edetail, "yy_pop_state: start stack is empty"));
  assert(yy_get_error(&lexer) == ecode);

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}

/* EOF */
