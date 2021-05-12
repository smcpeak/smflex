/* eval-skel-expr.h */
/* Evaluate expressions appearing in skeleton %if directives. */
/* See COPYING for license terms. */

#ifndef EVAL_SKEL_EXPR_H
#define EVAL_SKEL_EXPR_H

/* Type of a function that can evaluate identifiers.  'extra' is what
 * the client supplied in the context.  The identifier is at 'id' and
 * has length 'len'. */
typedef int (*eval_identifier_t)(void *extra, char const *id, int len);

/* Type of a function for reporting a expression syntax error.
 * This function must not return.  'extra' is data supplied by
 * the client in the evaluation context.  'syntax' is the start
 * of the offending syntax; it has a NUL terminator, but only
 * at the end of the entire expression. */
typedef void (*eval_fatal_error_t)(void *extra, char const *msg,
  char const *syntax);


/* Context data for expression evaluation. */
typedef struct eval_context_struct {
  /* Evaluate an identifier. */
  eval_identifier_t eval_identifier;

  /* Report a fatal error. */
  eval_fatal_error_t fatal_error;

  /* Additional parameter for both function pointers. */
  void *extra;
} eval_context_t;


/* Return true if 'c' is a character that can be the start of an
 * identifier in C/C++. */
int is_identifier_start(char c);

/* Return true if 'c' can be an identifier continuation. */
int is_identifier_continuation(char c);

/* Return true if 'text[0,len-1]' matches the entire NUL-terminated
 * 'str' string. */
int str_eq_substr(char const *str, char const *text, int len);

/* Evaluate an expression.  See comments on implementation for details. */
int evaluate_skel_expr(eval_context_t *ctx, char const *expr);


#endif /* EVAL_SKEL_EXPR_H */
