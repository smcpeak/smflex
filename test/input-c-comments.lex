/* input-c-comments.lex */
/* Demonstrate use of yyinput(). */

%option main

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
              while ( (c = yyinput()) != '*' && c != EOF )
                {}    /* eat up text of comment */

              if (c == '*') {
                while ( (c = yyinput()) == '*' )
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
