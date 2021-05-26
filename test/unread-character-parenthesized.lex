/* unread-character-parenthesized.lex */
/* Demonstrate use of 'YY_UNREAD_CHARACTER()'. */

%smflex 100
%option main

%{
#include <stdlib.h>          /* malloc */
#include <string.h>          /* strlen */

static char *my_strdup(char const *src)
{
  char *ret = (char*)malloc(strlen(src)+1);
  strcpy(ret, src);
  return ret;
}

/* Just for the benefit of the example in the manual, use the name
 * familiar to most people. */
#define strdup(s) my_strdup(s)
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
