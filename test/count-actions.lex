/* count-actions.lex */
/* Demonstrate using YY_USER_ACTION to count action invocations. */

%{
#include <assert.h>          /* assert */

int ctr[YY_NUM_RULES+1];     /* Entry 0 not used here. */

#define YY_USER_ACTION \
  assert(1 <= yy_act && yy_act <= YY_NUM_RULES); \
  ++ctr[yy_act];

%}

%%
foo        ECHO;             /* rule 1 */
bar        ECHO;             /* rule 2 */
[ ]+       ECHO;             /* rule 3 */
                             /* rule 4: implicit default */
%%

int main()
{
  int i;
  yy_lexer_t lexer;

  yy_construct(&lexer);
  while (yylex(&lexer))
    {}
  for (i=1; i <= YY_NUM_RULES; i++) {
    printf("rule %d executed %d times\n", i, ctr[i]);
  }

  yy_destroy(&lexer);
  return 0;
}
