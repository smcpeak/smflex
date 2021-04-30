/* username.lex */
/* Very simple example based on the first one in the manual. */

/* Do not use yywrap(). */
/* 2021-04-30: It now defaults to off. */
/*%option noyywrap*/

%%
username           printf("USERNAME");

%%
/* EOF */
