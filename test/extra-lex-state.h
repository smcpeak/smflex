/* extra-lex-state.h */
/* Declare a structure for use as an extra 'yy_lex' paramter. */
/* This makes sure we can insert an #include in the right place
 * in the generated scanner. */

#ifndef EXTRA_LEX_STATE_H
#define EXTRA_LEX_STATE_H

typedef struct ExtraLexState_struct {
  int field1;
  int field2;
} ExtraLexState;

#endif /* EXTRA_LEX_STATE_H */
