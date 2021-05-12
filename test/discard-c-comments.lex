/* discard-c-comments.lex */
/* Another illustration of start conditions. */

%option main

/* BEGIN: example fragment */
%x comment
                        int line_num = 1;
%%
"/*"                    BEGIN(comment);
<comment>[^*\n]*        /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n             ++line_num;
<comment>"*"+"/"        BEGIN(INITIAL);
  /* END: example fragment */

  /* Need to maintain 'line_num' outside comments too. */
\n                      ++line_num; ECHO;

  /* This rule lets me confirm that 'line_num' is being maintained. */
line_num                printf("[line_num: %d]", line_num);