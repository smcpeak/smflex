/* discard-ml-comments-top-state.lex */
/* Exercise 'yy_top_state'. */

%option main
%option stack
%option yy_top_state
%x comment foo

%%

<*>"(*"                 yy_push_state(yy_lexer, comment);
<comment>{
  [^*(]+                /* eat anything that's not a '*' or '(' */
  "*"+/[^)]             /* eat '*'s not followed by ')' */
  "("/[^*]              /* eat '(' not followed by '*' */
  "*"+")"               yy_pop_state(yy_lexer);
}

  /* A way to get into and out of 'foo'. */
beginfoo                BEGIN(foo); ECHO;
<foo>endfoo             BEGIN(INITIAL); ECHO;

  /* This rule lets me query current start state. */
<INITIAL,foo>state      {
                          printf("[state %d]", YY_START);
                        }

  /* This lets me query 'yy_top_state'.  It requires that the stack not
   * be empty, so is only active inside comments. */
<comment>"(yy_top_state)"   {
                          printf("[top state %d]", yy_top_state(yy_lexer));
                        }

  /* EOF */
