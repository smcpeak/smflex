/* read-character-c-comments.lex */
/* Demonstrate use of 'YY_READ_CHARACTER'. */

%option main
%option yy_read_character

%{

#include <stdio.h>           /* printf */
#include <stdlib.h>          /* exit */

static void error(char const *msg)
{
  printf("%s\n", msg);
  exit(2);
}

%}

  /* BEGIN: example fragment */
%%
"/*"      {
            int c;
            for (;;) {
              while ( (c = YY_READ_CHARACTER()) != '*' && c != EOF )
                {}    /* eat up text of comment */

              if (c == '*') {
                while ( (c = YY_READ_CHARACTER()) == '*' )
                  {}
                if (c == '/') {
                  break;    /* found the end */
                }
              }

              if (c == EOF) {
                error("EOF in comment");
                break;
              }
            }
          }
