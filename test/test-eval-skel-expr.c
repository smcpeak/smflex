/* test-eval-skel-expr.c */
/* Unit test for eval-skel-expr.c. */
/* See ../COPYING for license terms. */

#include "eval-skel-expr.h"            /* module under test */

#include <stdio.h>                     /* fprintf */
#include <stdlib.h>                    /* exit */
#include <string.h>                    /* strstr */


/* Whether to print errors. */
static int print_errors = 1;

/* Save the first of a series of error messages. */
static char const *first_error_msg;


static void fatal_test_error(void *extra, char const *msg,
                             char const *syntax)
{
  if (!first_error_msg) {
    first_error_msg = msg;
  }
  if (print_errors) {
    fprintf(stderr,
      "expr error:\n"
      "  msg   : %s\n"
      "  syntax: %s\n",
      msg, syntax);
  }
}


static int eval_test_identifier(void *extra, char const *id, int len)
{
# define CASE(str, val) \
    else if (str_eq_substr(str, id, len)) { return (val); }

  if (0) {}

  CASE("zero", 0)
  CASE("one", 1)
  CASE("two", 2)
  CASE("three", 3)

# undef CASE

  else {
    fatal_test_error(extra, "unknown identifier", id);
    return 0;
  }
}


static int eval_test_expr(char const *expr)
{
  eval_context_t ctx;
  ctx.eval_identifier = &eval_test_identifier;
  ctx.fatal_error = &fatal_test_error;
  ctx.extra = NULL;

  return evaluate_skel_expr(&ctx, expr);
}


static void test_expr(char const *expr, int expect)
{
  print_errors = 1;

  int actual = eval_test_expr(expr);
  if (actual != expect) {
    fprintf(stderr,
      "test_expr failed:\n"
      "  expr  : %s\n"
      "  expect: %d\n"
      "  actual: %d\n",
      expr, expect, actual);
    exit(2);
  }
}


static void test_inval(char const *expr, char const *expect)
{
  first_error_msg = NULL;
  print_errors = 0;
  (void)eval_test_expr(expr);

  if (!first_error_msg) {
    fprintf(stderr,
      "test_valid: did not get an error\n"
      "  expr  : %s\n"
      "  expect: %s\n",
      expr, expect);
    exit(2);
  }

  if (!strstr(first_error_msg, expect)) {
    fprintf(stderr,
      "test_valid: did not get expected error\n"
      "  expr  : %s\n"
      "  actual: %s\n"
      "  expect: %s\n",
      expr, first_error_msg, expect);
    exit(2);
  }
}


int main()
{
  /* Valid expressions. */
  test_expr("5", 5);
  test_expr("one", 1);
  test_expr("0 && 1", 0);
  test_expr("1 && 1", 1);
  test_expr("0 || 1", 1);
  test_expr("0 || 0", 0);
  test_expr("!0", 1);
  test_expr("!1", 0);
  test_expr("!three", 0);
  test_expr("!!three", 1);
  test_expr("!0 && !!1", 1);
  test_expr("!(!0 && !!1)", 0);
  test_expr("1 && 1 || 1 && 0", 1);
  test_expr("(1 && 1) || (1 && 0)", 1);
  test_expr("(1 && 1 || 1) && 0", 0);
  test_expr("((((((7))))))", 7);
  test_expr("one && two && three", 1);
  test_expr("one && two && !three", 0);
  test_expr("one && !two && three", 0);
  test_expr("!one && two && three", 0);
  test_expr("!one && two || three", 1);
  test_expr("zero || zero || zero", 0);
  test_expr("one  || zero || zero", 1);
  test_expr("zero || one  || zero", 1);
  test_expr("zero || zero || one ", 1);
  if (sizeof(int) == 4) {
    test_expr("2147483647", 2147483647);
  }

  /* Cases for "num_backing_up > 0 && !reject_used". */
  test_expr("zero > 0 && !zero", 0);
  test_expr("one > 0  && !zero", 1);
  test_expr("two > 0  && !zero", 1);
  test_expr("zero > 0 && !one",  0);
  test_expr("one > 0  && !one",  0);

  /* Invalid expressions. */
  test_inval("", "invalid expression syntax");
  test_inval("0 0", "extra syntax");
  test_inval("0+0", "extra syntax");
  test_inval("unknown", "unknown identifier");
  test_inval("123abc", "invalid integer");
  test_inval("(1", "missing ')'");
  test_inval("1)", "extra syntax");
  test_inval("1 & 1", "must be followed by '&'");
  test_inval("1 | 1", "must be followed by '|'");
  if (sizeof(int) == 4) {
    test_inval("2147483648", "too large");
    test_inval("999999999999999999999999", "too large");
  }

  fprintf(stderr, "test-eval-skel-expr PASSED\n");
  return 0;
}


/* EOF */
