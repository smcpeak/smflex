/* unread-character-parenthesized.lex */
/* Demonstrate use of 'YY_UNREAD_CHARACTER()'. */

%option main

%{
#include <string.h>          /* strdup */
%}

%%

"(token)"    printf("[saw \"(token)\"]");

"token" {
    /* BEGIN: example fragment */
    {
      int i;
      /* Copy yytext because YY_UNREAD_CHARACTER() trashes yytext */
      char *yycopy = strdup(yytext);
      YY_UNREAD_CHARACTER(')');
      for (i = yyleng - 1; i >= 0; --i) {
        YY_UNREAD_CHARACTER(yycopy[i]);
      }
      YY_UNREAD_CHARACTER('(');
      free(yycopy);
    }
    /* END: example fragment */
  }
