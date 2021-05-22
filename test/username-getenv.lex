/* username-getenv.lex */
/* First example. */

%smflex 100
%option main
%%
username    printf("%s", getenv("USER"));
