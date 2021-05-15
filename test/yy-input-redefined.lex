/* yy-input-redefined.lex */
/* Demonstrate an alterative definition of yy_read_input_function. */

%{
#include <ctype.h>                     /* tolower */
%}

%%
username    printf("USERNAME");

%%

/* BEGIN: example fragment */
int my_read_input(yy_lexer_t *yy_lexer, void *dest, int size)
{
  int c = getchar();
  c = tolower(c);    /* Just for demonstration purposes. */
  if (c == EOF) {
    return 0;
  }
  else {
    ((char*)dest)[0] = c;
    return 1;
  }
}

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);

  /* Alter how the scanner gets its input. */
  lexer.yy_read_input_function = &my_read_input;

  while (yy_lex(&lexer))
    {}
  yy_destroy(&lexer);
  return 0;
}
