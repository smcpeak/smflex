/* yyless-foobar.lex */
/* Demonstrate yyless(). */

%option main

/* BEGIN: example fragment */
%%
foobar    ECHO; yyless(3);
[a-z]+    ECHO;
