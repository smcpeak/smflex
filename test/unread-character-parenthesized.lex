/* unread-character-parenthesized.lex */
/* Demonstrate use of 'YY_UNREAD_CHARACTER()'. */

%smflex 100
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
      /* Copy YY_TEXT because YY_UNREAD_CHARACTER() trashes YY_TEXT */
      char *yycopy = strdup(YY_TEXT);
      YY_UNREAD_CHARACTER(')');
      for (i = YY_LENG - 1; i >= 0; --i) {
        YY_UNREAD_CHARACTER(yycopy[i]);
      }
      YY_UNREAD_CHARACTER('(');
      free(yycopy);
    }
    /* END: example fragment */
  }
