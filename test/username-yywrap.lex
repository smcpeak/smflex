/* username-yywrap.lex */
/* Like username.lex, but use 'yywrap()'. */

%{

#include <stdio.h>                     /* fopen */
#include <stdlib.h>                    /* exit, perror */

%}

/* This test uses yywrap, and that feature now has to be explicitly
 * requested. */
%option yywrap


%%
username           printf("USERNAME");

%%

/* True once we have read the second input. */
static int readSecondFile = 0;

int yywrap()
{
  if (!readSecondFile) {
    readSecondFile = 1;

    yyin = fopen("username-yywrap.input2", "r");
    if (!yyin) {
      perror("fopen(username-yywrap.input2)");
      exit(2);
    }

    /* Not done, keep going. */
    return 0;
  }
  else {
    fclose(yyin);

    /* All done. */
    return 1;
  }
}


/* EOF */
