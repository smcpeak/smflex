/* case-insensitive.lex */
/* Exercise %option case-insensitive. */

%smflex 100
%option case-insensitive
%option main

%%

fooBAR    printf("[fooBAR]");
