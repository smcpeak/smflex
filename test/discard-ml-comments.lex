/* discard-ml-comments.lex */
/* Discard ML comments, which can nest.  Demonstrates use of the
 * start condition stack. */

%smflex 100
%option main
  /* BEGIN: example fragment */
%option stack
%x comment foo
%%
<*>"(*"                 YY_PUSH_START_CONDITION(comment);
<comment>{
  [^*(]+                /* eat anything that's not a '*' or '(' */
  "*"+/[^)]             /* eat '*'s not followed by ')' */
  "("/[^*]              /* eat '(' not followed by '*' */
  "*"+")"               YY_POP_START_CONDITION();
}
  /* END: example fragment */

  /* A way to get into and out of 'foo'. */
beginfoo                YY_SET_START_CONDITION(foo); YY_ECHO;
<foo>endfoo             YY_SET_START_CONDITION(INITIAL); YY_ECHO;

  /* This rule lets me query current start condition. */
<INITIAL,foo>state      {
                          printf("[state %d]", YY_GET_START_CONDITION());
                        }
