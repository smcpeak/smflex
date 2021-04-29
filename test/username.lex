/* username.lex */
/* Very simple example based on the first one in the manual. */

/* Do not use yywrap(). */
%option noyywrap

%%
username           printf("USERNAME");

%%
/* EOF */
