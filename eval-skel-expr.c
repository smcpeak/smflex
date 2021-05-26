/* eval-skel-expr.c */
/* Code for eval-skel-expr.h. */
/* See COPYING for license terms. */

#include "eval-skel-expr.h"            /* this module */

#include <ctype.h>                     /* isdigit */
#include <limits.h>                    /* MAX_INT */
#include <string.h>                    /* strncmp, strlen */


int is_identifier_start(char c)
{
  return c == '_' ||
         ('A' <= c && c <= 'Z') ||
         ('a' <= c && c <= 'z');
}


int is_identifier_continuation(char c)
{
  return c == '_' ||
         ('0' <= c && c <= '9') ||
         ('A' <= c && c <= 'Z') ||
         ('a' <= c && c <= 'z');
}


int str_eq_substr(char const *str, char const *text, int len)
{
  return 0==strncmp(str, text, len) &&
         strlen(str) == len;
}


/* Advance '*expp' past any space characters.  In general, we call
 * this just before checking for specific characters. */
static void skip_spaces(char const **expp)
{
  while (**expp == ' ') {
    (*expp)++;
  }
}


/* Evaluate an integer literal or an identifier. */
static int eval_prim(eval_context_t *ctx, char const **expp)
{
  skip_spaces(expp);

  if (isdigit((unsigned char)**expp)) {
    char const *p = *expp;
    int res = *p - '0';
    p++;
    while (isdigit((unsigned char)*p)) {
      if (res > (INT_MAX - (*p - '0')) / 10) {
        ctx->fatal_error(ctx->extra, "integer too large to represent",
          *expp);
        return 0;
      }
      res = res*10 + (*p - '0');
      p++;
    }

    if (is_identifier_start(*p)) {
      /* Something like "123abc". */
      ctx->fatal_error(ctx->extra, "invalid integer syntax", *expp);
      return 0;
    }

    *expp = p;
    return res;
  }

  if (is_identifier_start(**expp)) {
    char const *p = (*expp) + 1;
    int res;
    while (is_identifier_continuation(*p)) {
      p++;
    }

    res = ctx->eval_identifier(ctx->extra, *expp, p - *expp);
    *expp = p;
    return res;
  }

  ctx->fatal_error(ctx->extra, "invalid expression syntax", *expp);
  return 0;
}


static int eval_or(eval_context_t *ctx, char const **expp);

/* Evaluate a parenthesized expression, or delegate for an expression
 * with higher precedence. */
static int eval_paren(eval_context_t *ctx, char const **expp)
{
  skip_spaces(expp);
  if (**expp == '(') {
    int inner;
    (*expp)++;

    inner = eval_or(ctx, expp);

    skip_spaces(expp);
    if (**expp != ')') {
      ctx->fatal_error(ctx->extra, "'(' missing ')'", *expp);
      return 0;
    }
    (*expp)++;

    return inner;
  }
  else {
    return eval_prim(ctx, expp);
  }
}


/* Evaluate a negation, or delegate for higher precedence. */
static int eval_not(eval_context_t *ctx, char const **expp)
{
  int inner;

  skip_spaces(expp);
  if (**expp == '!') {
    (*expp)++;

    inner = eval_not(ctx, expp);
    return !inner;
  }
  else {
    return eval_paren(ctx, expp);
  }
}


/* Evaluate a relational comparison. */
static int eval_relational(eval_context_t *ctx, char const **expp)
{
  int left = eval_not(ctx, expp);
  int right;

  skip_spaces(expp);
  if (**expp == '>') {
    (*expp)++;

    /* I do not allow relational operators to be used associatively. */
    right = eval_not(ctx, expp);
    return left > right;
  }
  else {
    return left;
  }
}


/* Evaluate a conjunction, or delegate for higher precedence. */
static int eval_and(eval_context_t *ctx, char const **expp)
{
  int left = eval_relational(ctx, expp);
  int right;

  skip_spaces(expp);
  if (**expp == '&') {
    (*expp)++;
    if (**expp != '&') {
      ctx->fatal_error(ctx->extra, "'&' must be followed by '&'", *expp - 1);
      return 0;
    }
    (*expp)++;

    skip_spaces(expp);

    right = eval_and(ctx, expp);
    return left && right;
  }
  else {
    return left;
  }
}


/* Evaluate a disjunction, or delegate for higher precedence. */
static int eval_or(eval_context_t *ctx, char const **expp)
{
  int left = eval_and(ctx, expp);
  int right;

  skip_spaces(expp);
  if (**expp == '|') {
    (*expp)++;
    if (**expp != '|') {
      ctx->fatal_error(ctx->extra, "'|' must be followed by '|'", *expp - 1);
      return 0;
    }
    (*expp)++;

    skip_spaces(expp);

    right = eval_or(ctx, expp);
    return left || right;
  }
  else {
    return left;
  }
}


/* Evaluate the expression 'expr', which is NUL terminated, given that
 * 'eval_id' says how to evaluate identifiers.  The language
 * recognized by this evaluator is:
 *
 *   expr ::= expr || expr             # lowest precedence
 *          | expr && expr
 *          | expr > expr
 *          | ! expr                   # highest precedence
 *          | ( expr )
 *          | number
 *          | identifier
 *
 * where tokens may be separated by spaces (only; no tabs or newlines).
 *
 * There is almost no error recovery.  Upon a syntax error, the
 * 'ctx->fatal_error' function is invoked, which should not return.
 * If it does, the results are unspecified, although the evaluation
 * will terminate.
 */
int evaluate_skel_expr(eval_context_t *ctx, char const *expr)
{
  int res;

  res = eval_or(ctx, &expr);

  skip_spaces(&expr);

  if (expr[0] == '\0') {
    /* Fine; usual case. */
  }
  else if (expr[0] == '\\' && expr[1] == '\0') {
    /* Allow a single trailing backslash so if I use %if within a
     * macro definition I can consistently have backslashes at the
     * end of every line, which makes the syntax highlighting work
     * better in my editor. */
  }
  else {
    ctx->fatal_error(ctx->extra, "extra syntax after expression", expr);
    return 0;
  }

  return res;
}


/* EOF */
