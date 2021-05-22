/* yymore-yytext.lex */
/* Explore interaction of yymore and YY_TEXT. */

%smflex 100
%option main
%option yy_read_character
%option yymore

/* Exercise the 'yymore' alias. */
%option flex-compat

/* We do not test unreading here.  The updated documentation makes it
 * clear the effects are undefined. */

%%

abc      {
           printf("[matched \"%s\", len=%d]\n", YY_TEXT, YY_LENG);
           yymore();
           printf("[after yymore, text=\"%s\", len=%d]\n", YY_TEXT, YY_LENG);
         }

def      {
           printf("[matched \"%s\", len=%d]\n", YY_TEXT, YY_LENG);
           yymore();
           printf("[after yymore, text=\"%s\", len=%d]\n", YY_TEXT, YY_LENG);
         }

ghi      {
           printf("[matched \"%s\", len=%d]\n", YY_TEXT, YY_LENG);
           yymore();
           printf("[after yymore, text=\"%s\", len=%d]\n", YY_TEXT, YY_LENG);
         }

jkl      printf("[matched \"%s\", len=%d]\n", YY_TEXT, YY_LENG);

mno      {
           printf("[matched \"%s\", len=%d]\n", YY_TEXT, YY_LENG);
           printf("[read: '%c']\n", YY_READ_CHARACTER());
           printf("[after read, text=\"%s\", len=%d]\n", YY_TEXT, YY_LENG);
         }
