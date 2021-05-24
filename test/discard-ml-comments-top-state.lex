/* discard-ml-comments-top-state.lex */
/* Exercise 'yy_top_start_condition'. */
/* Also exercise flex-compat YY_START. */

%smflex 100
%option main
%option stack
%option flex-compat
%x comment foo

%{
#include <assert.h>                    /* assert */
%}

%%

<*>"(*"                 yy_push_start_condition(yy_lexer, comment);
<comment>{
  [^*(]+                /* eat anything that's not a '*' or '(' */
  "*"+/[^)]             /* eat '*'s not followed by ')' */
  "("/[^*]              /* eat '(' not followed by '*' */
  "*"+")"               yy_pop_start_condition(yy_lexer);
}

  /* A way to get into and out of 'foo'. */
beginfoo                YY_SET_START_CONDITION(foo); YY_ECHO;
<foo>endfoo             YY_SET_START_CONDITION(INITIAL); YY_ECHO;

  /* This rule lets me query current start condition. */
<INITIAL,foo>state      {
                          printf("[cond %d]", YY_START);
                        }

  /* This lets me query 'yy_top_start_condition'.  It requires that the stack not
   * be empty, so is only active inside comments. */
<comment>"(yy_top_start_condition)"   {
                          assert(!YY_CONDITION_STACK_IS_EMPTY());
                          printf("[top cond %d]", YY_TOP_START_CONDITION());
                        }

  /* EOF */
