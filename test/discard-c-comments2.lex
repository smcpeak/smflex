/* discard-c-comments2.lex */
/* Keep track of state before 'comment'. */

%option main

/* BEGIN: example fragment */
%x comment foo
                        int line_num = 1;
                        int comment_caller = INITIAL;
%%
"/*"                    {
                          comment_caller = INITIAL;
                          BEGIN(comment);
                        }

<foo>"/*"               {
                          comment_caller = foo;
                          BEGIN(comment);
                        }

<comment>[^*\n]*        /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n             ++line_num;
<comment>"*"+"/"        BEGIN(comment_caller);
  /* END: example fragment */

  /* A way to get into and out of 'foo'. */
beginfoo                BEGIN(foo); ECHO;
<foo>endfoo             BEGIN(INITIAL); ECHO;

  /* Need to maintain 'line_num' outside comments too. */
<INITIAL,foo>\n         ++line_num; ECHO;

  /* This rule lets me confirm that 'line_num' is being maintained.
   * and also check the current start state. */
<INITIAL,foo>line_num   {
                          printf("[line_num %d in state %d]",
                                 line_num, YY_START);
                        }
