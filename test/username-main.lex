/* username-main.lex */
/* Like 'username', but use '%option main'. */

/* Have smflex provide a main(). */
%option main
%smflex 100

%%
username           printf("USERNAME");

%%
/* EOF */
