/* discard-c-comments2.lex */
/* Keep track of state before 'comment'. */

%smflex 100
%option main

/* BEGIN: example fragment */
%x comment foo
                        int line_num = 1;
                        int comment_caller = INITIAL;
%%
"/*"                    {
                          comment_caller = INITIAL;
                          YY_SET_START_CONDITION(comment);
                        }

<foo>"/*"               {
                          comment_caller = foo;
                          YY_SET_START_CONDITION(comment);
                        }

<comment>[^*\n]*        /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n             ++line_num;
<comment>"*"+"/"        YY_SET_START_CONDITION(comment_caller);
  /* END: example fragment */

  /* A way to get into and out of 'foo'. */
beginfoo                YY_SET_START_CONDITION(foo); YY_ECHO;
<foo>endfoo             YY_SET_START_CONDITION(INITIAL); YY_ECHO;

  /* Need to maintain 'line_num' outside comments too. */
<INITIAL,foo>\n         ++line_num; YY_ECHO;

  /* This rule lets me confirm that 'line_num' is being maintained.
   * and also check the current start condition. */
<INITIAL,foo>line_num   {
                          printf("[line_num %d in state %d]",
                                 line_num, YY_GET_START_CONDITION());
                        }
