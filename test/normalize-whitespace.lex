/* normalize-whitespace.lex */

%smflex 100
%option main

/* BEGIN: example fragment */
%%
[ \t]+        putchar(' ');
[ \t]+$       /* ignore this token */
