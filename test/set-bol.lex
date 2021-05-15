/* set-bol.lex */
/* Exercise 'yy_set_bol' and 'YY_AT_BOL'. */

%option main

%%
set_bol      YY_ECHO; yy_set_bol(yy_lexer, 1);
clear_bol\n  YY_ECHO; yy_set_bol(yy_lexer, 0);
^[ ]+        printf("[leading space: \"%s\"]", yytext);
[ ]+         printf("[non-leading space: \"%s\"]", yytext);
get_bol1     YY_ECHO; printf("[AT_BOL1: %d]", YY_AT_BOL());
get_bol2\n   YY_ECHO; printf("[AT_BOL2: %d]", YY_AT_BOL());
