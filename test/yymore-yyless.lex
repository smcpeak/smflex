/* yymore-yyless.lex */
/* Test 'yymore()' followed by 'YY_LESS_TEXT()'. */

%option main

%{
int lessArg = 6;
%}

%%

abc      {
           printf("[\"%s\", %d]\n", YY_TEXT, YY_LENG);
           printf("[yymore]\n");
           yymore();
         }

def      {
           printf("[\"%s\", %d]\n", YY_TEXT, YY_LENG);
           if (lessArg >= 0) {
             printf("[YY_LESS_TEXT(%d)]\n", lessArg);
             YY_LESS_TEXT(lessArg);
             lessArg--;
             printf("[\"%s\", %d]\n", YY_TEXT, YY_LENG);
           }
         }