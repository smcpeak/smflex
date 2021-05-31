/* username-getenv.lex */
/* First example. */

%smflex 101
%option main
%%
username    printf("%s", getenv("USER"));
