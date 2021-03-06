/* yy-wrap-function.lex */
/* Demonstrate setting 'yy_wrap_function'. */

%smflex 100
%{
#include <stdio.h>                     /* fopen, perror */
#include <stdlib.h>                    /* exit */
%}

%%
username           printf("USERNAME");

%%

/* True once we have read the second input. */
static int readSecondFile = 0;

int my_wrap_func(yy_lexer_t *yy_lexer)
{
  if (!readSecondFile) {
    FILE *fp = fopen("username-yywrap.input2", "r");
    if (!fp) {
      perror("fopen(username-yywrap.input2)");
      exit(2);
    }

    yy_restart(yy_lexer, fp);
    readSecondFile = 1;

    /* Not done, keep going. */
    return 0;
  }
  else {
    fclose(yy_lexer->yy_input_stream);

    /* All done. */
    return 1;
  }
}

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);

  lexer.yy_wrap_function = &my_wrap_func;

  while (yy_lex(&lexer))
    {}
  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}

/* EOF */
