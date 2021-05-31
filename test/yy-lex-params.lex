/* yy-lex-params.lex */
/* Exercise %option yy_lex_parameters. */

%smflex 101
%option yy_lex_parameters="int x, int y, ExtraLexState *extraState"

%header{
#include "extra-lex-state.h"           /* ExtraLexState */
%}

%%

username    {
              printf("USERNAME [x=%d,y=%d,field1=%d,field2=%d]",
                     x, y, extraState->field1, extraState->field2);
            }

%%

int main()
{
  yy_lexer_t lexer;
  ExtraLexState extraState;

  extraState.field1 = 5;
  extraState.field2 = 6;

  yy_construct(&lexer);
  while (yy_lex(&lexer, 3, 4, &extraState))
    {}

  yy_destroy(&lexer);
  return 0;
}
