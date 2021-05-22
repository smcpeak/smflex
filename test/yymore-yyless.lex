/* yymore-yyless.lex */
/* Test 'yymore()' followed by 'YY_LESS_TEXT()'. */

%smflex 100
%option main
%option yymore

%{
int lessArg = 6;
%}

%%

abc      {
           printf("[\"%s\", %d]\n", YY_TEXT, YY_LENG);
           printf("[YY_MORE_TEXT]\n");
           YY_MORE_TEXT();
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
