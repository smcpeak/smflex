/* username-main.lex */
/* Like 'username', but use '%option main%'. */

/* Have smflex provide a main(). */
%option main

%%
username           printf("USERNAME");

%%
/* EOF */
