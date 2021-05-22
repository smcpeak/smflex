/* stack-api.lex */
/* Exercise the "%option stack" API. */

%smflex 100
%{
#include <assert.h>                    /* assert */
#include <stdio.h>                     /* printf */
#include <stdlib.h>                    /* getenv, atoi */
#include <string.h>                    /* strcmp */
%}

%option stack

%%

  /* No rules.  We do not actually call 'yy_lex()'. */

%%

static yy_error_code_t ecode;
static char const *edetail;

static void record_error(yy_lexer_t const *yy_lexer,
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
  assert(yy_get_start_condition(&lexer) == 0);
  assert(yy_condition_stack_is_empty(&lexer));

  for (i=1; i < iters; i++) {
    yy_push_start_condition(&lexer, i);
    assert(yy_get_start_condition(&lexer) == i);
    assert(!yy_condition_stack_is_empty(&lexer));
  }

  for (i = iters-1; i >= 1; i--) {
    assert(!yy_condition_stack_is_empty(&lexer));
    assert(yy_get_start_condition(&lexer) == i);
    assert(yy_top_start_condition(&lexer) == i-1);
    yy_pop_start_condition(&lexer);
  }

  assert(yy_condition_stack_is_empty(&lexer));
  assert(yy_get_start_condition(&lexer) == 0);

  /* Deliberately cause an "API misuse" error. */
  assert(yy_get_error(&lexer) == yy_err_no_error);
  lexer.yy_error_function = &record_error;
  yy_pop_start_condition(&lexer);

  /* Validate the error reporting. */
  assert(ecode == yy_err_api_misuse);
  assert(0==strcmp(yy_error_string(ecode), "API misused"));
  assert(0==strcmp(edetail, "yy_pop_start_condition: start stack is empty"));
  assert(yy_get_error(&lexer) == ecode);

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}

/* EOF */
