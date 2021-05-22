/* read-unread-character.lex */
/* Exercise 'yy_read_character' and 'yy_unread_character'. */

%smflex 100
%option yy_read_character
%option yy_unread_character

%{
#include <ctype.h>                     /* isspace */
%}

%%

one          return 1;
two          return 2;
three        return 3;

%%

int main()
{
  int tok;
  yy_lexer_t lexer;
  yy_construct(&lexer);

  while ((tok = yy_lex(&lexer)) != 0) {
    int c;

    /* Print the digit in place of its english name. */
    printf("%d", tok);

    /* Gobble whitespace. */
    while (isspace(c = yy_read_character(&lexer)))
      {}

    if (c == EOF) {
      printf("yy_read_character returned EOF\n");
      break;
    }

    if (c == '!') {
      /* Replace '!' with "bang", which will then be scanned. */
      yy_unread_character(&lexer, 'g');
      yy_unread_character(&lexer, 'n');
      yy_unread_character(&lexer, 'a');
      yy_unread_character(&lexer, 'b');
    }
    else {
      /* Put the character back. */
      yy_unread_character(&lexer, c);
    }
  }

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
