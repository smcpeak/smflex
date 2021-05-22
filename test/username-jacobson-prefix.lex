/* username-jacobson-prefix.lex */
/* Use '%option jacobson' and '%option prefix'. */

%smflex 100
%option jacobson
%option prefix="MyPrefix"
%option main

%%
username           printf("USERNAME");

%%
/* EOF */
