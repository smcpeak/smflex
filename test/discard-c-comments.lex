/* discard-c-comments.lex */
/* Another illustration of start conditions. */

%smflex 100
%option main

/* BEGIN: example fragment */
%x comment
                        int line_num = 1;
%%
"/*"                    YY_SET_START_CONDITION(comment);
<comment>[^*\n]*        /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n             ++line_num;
<comment>"*"+"/"        YY_SET_START_CONDITION(INITIAL);
  /* END: example fragment */

  /* Need to maintain 'line_num' outside comments too. */
\n                      ++line_num; YY_ECHO;

  /* This rule lets me confirm that 'line_num' is being maintained. */
line_num                printf("[line_num: %d]", line_num);
