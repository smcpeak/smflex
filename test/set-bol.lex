/* set-bol.lex */
/* Exercise 'yy_set_bol' and 'YY_AT_BOL'. */

%option main

%%
set_bol      ECHO; yy_set_bol(yy_lexer, 1);
clear_bol\n  ECHO; yy_set_bol(yy_lexer, 0);
^[ ]+        printf("[leading space: \"%s\"]", yytext);
[ ]+         printf("[non-leading space: \"%s\"]", yytext);
get_bol1     ECHO; printf("[AT_BOL1: %d]", YY_AT_BOL());
get_bol2\n   ECHO; printf("[AT_BOL2: %d]", YY_AT_BOL());
