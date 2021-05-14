/* username-yywrap.lex */
/* Like username.lex, but use 'yywrap()'. */

%{
#include <stdio.h>                     /* fopen */
#include <stdlib.h>                    /* exit, perror */
%}

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


/* BEGIN: example fragment */
int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);

  /* This line is the replacement for the old "%option yywrap". */
  lexer.yy_wrap_function = &yywrap;

  while (yy_lex(&lexer))
    {}
  yy_destroy(&lexer);
  return 0;
}
/* END: example fragment */

/* EOF */
