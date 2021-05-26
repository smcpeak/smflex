/* unput-parenthesized.lex */
/* Demonstrate use of unput(). */

%smflex 100
%option main
%option flex-compat
%option yy_unread_character

%{
#include <stdlib.h>          /* malloc */
#include <string.h>          /* strlen */

static char *my_strdup(char const *src)
{
  char *ret = (char*)malloc(strlen(src)+1);
  strcpy(ret, src);
  return ret;
}
%}

%%

"(token)"    printf("[saw \"(token)\"]");

"token" {
    {
      int i;
      /* Copy YY_TEXT because unput() trashes YY_TEXT */
      char *yycopy = my_strdup(YY_TEXT);
      unput(')');
      for (i = YY_LENG - 1; i >= 0; --i) {
        unput(yycopy[i]);
      }
      unput('(');
      free(yycopy);
    }
  }
