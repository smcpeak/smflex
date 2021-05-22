/* username-main.lex */
/* Like 'username', but use '%option main'. */

%smflex 100

/* Have smflex provide a main(). */
%option main

%%
username           printf("USERNAME");

%%
/* EOF */
