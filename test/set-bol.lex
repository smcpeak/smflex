/* set-bol.lex */
/* Exercise 'yy_set_bol' and 'YY_AT_BOL'. */

/* Activate YY_AT_BOL, yytext and yyleng. */
%option flex-compat

%option main

%%
set_bol      YY_ECHO; yy_set_bol(yy_lexer, 1);
clear_bol\n  YY_ECHO; yy_set_bol(yy_lexer, 0);
^[ ]+        printf("[leading space: \"%s\"]", YY_TEXT);
[ ]+         printf("[non-leading space: \"%s\", %d chars]", yytext, yyleng);
get_bol1     YY_ECHO; printf("[AT_BOL1: %d]", YY_AT_BOL() && YY_GET_BOL());
get_bol2\n   YY_ECHO; printf("[AT_BOL2: %d]", yy_get_bol(yy_lexer));
