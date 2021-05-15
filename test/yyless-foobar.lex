/* yyless-foobar.lex */
/* Demonstrate yyless(). */

%option main

/* BEGIN: example fragment */
%%
foobar    YY_ECHO; yyless(3);
[a-z]+    YY_ECHO;
