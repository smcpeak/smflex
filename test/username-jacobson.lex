/* username-jacobson.lex */
/* Use '%option jacobson'. */

%smflex 100
%option jacobson
%option main

%%
username           printf("USERNAME");

%%
/* EOF */
