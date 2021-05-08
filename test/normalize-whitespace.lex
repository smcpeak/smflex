/* normalize-whitespace.lex */

%option main

/* BEGIN: example fragment */
%%
[ \t]+        putchar(' ');
[ \t]+$       /* ignore this token */
