/* yyless-foobar.lex */
/* Demonstrate yyless(). */

%option main

/* BEGIN: example fragment */
%%
foobar    YY_ECHO; YY_LESS_TEXT(3);
[a-z]+    YY_ECHO;
