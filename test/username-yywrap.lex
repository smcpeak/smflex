/* username-yywrap.lex */
/* Like username.lex, but use 'yywrap()'. */

%{

#include <stdio.h>                     /* fopen */
#include <stdlib.h>                    /* exit, perror */

%}

/* This test uses yywrap, and that feature now has to be explicitly
 * requested. */
%option yywrap

/* Also get a main() so I do not have to write my own. */
%option main


%%
username           printf("USERNAME");

%%

/* True once we have read the second input. */
static int readSecondFile = 0;

int yywrap(yy_lexer_t *yy_lexer)
{
  if (!readSecondFile) {
    readSecondFile = 1;

    yy_lexer->yy_input_stream = fopen("username-yywrap.input2", "r");
    if (!yy_lexer->yy_input_stream) {
      perror("fopen(username-yywrap.input2)");
      exit(2);
    }

    /* Not done, keep going. */
    return 0;
  }
  else {
    fclose(yy_lexer->yy_input_stream);

    /* All done. */
    return 1;
  }
}


/* EOF */
