/* yyless-foobar.lex */
/* Demonstrate yyless(). */

%smflex 100
%option main

/* BEGIN: example fragment */
%%
foobar    YY_ECHO; YY_LESS_TEXT(3);
[a-z]+    YY_ECHO;
