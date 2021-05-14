/* stack-api.lex */
/* Exercise the "%option stack" API. */

%{
#include <assert.h>                    /* assert */
#include <stdio.h>                     /* printf */
#include <stdlib.h>                    /* getenv, atoi */
%}

%option stack
%option yy_top_state

%%

  /* No rules.  We do not actually call 'yy_lex()'. */

%%

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

  assert(yy_get_start_state(&lexer) == 0);
  yy_construct(&lexer);

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

  yy_destroy(&lexer);
  return 0;
}

/* EOF */
