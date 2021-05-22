/* unput-parenthesized.lex */
/* Demonstrate use of unput(). */

%smflex 100
%option main
%option flex-compat
%option yy_unread_character

%{
#include <string.h>          /* strdup */
%}

%%

"(token)"    printf("[saw \"(token)\"]");

"token" {
    {
      int i;
      /* Copy YY_TEXT because unput() trashes YY_TEXT */
      char *yycopy = strdup(YY_TEXT);
      unput(')');
      for (i = YY_LENG - 1; i >= 0; --i) {
        unput(yycopy[i]);
      }
      unput('(');
      free(yycopy);
    }
  }
