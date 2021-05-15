/* discard-ml-comments.lex */
/* Discard ML comments, which can nest.  Demonstrates use of the
 * start condition stack. */

%option main
  /* BEGIN: example fragment */
%option stack
%x comment foo
%%
<*>"(*"                 yy_push_state(yy_lexer, comment);
<comment>{
  [^*(]+                /* eat anything that's not a '*' or '(' */
  "*"+/[^)]             /* eat '*'s not followed by ')' */
  "("/[^*]              /* eat '(' not followed by '*' */
  "*"+")"               yy_pop_state(yy_lexer);
}
  /* END: example fragment */

  /* A way to get into and out of 'foo'. */
beginfoo                BEGIN(foo); YY_ECHO;
<foo>endfoo             BEGIN(INITIAL); YY_ECHO;

  /* This rule lets me query current start state. */
<INITIAL,foo>state      {
                          printf("[state %d]", YY_START);
                        }
