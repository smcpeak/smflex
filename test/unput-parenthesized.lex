/* unput-parenthesized.lex */
/* Demonstrate use of unput(). */

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
      /* Copy yytext because unput() trashes yytext */
      char *yycopy = strdup(yytext);
      unput(')');
      for (i = yyleng - 1; i >= 0; --i) {
        unput(yycopy[i]);
      }
      unput('(');
      free(yycopy);
    }
    /* END: example fragment */
  }
