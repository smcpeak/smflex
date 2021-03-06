/* count-actions.lex */
/* Demonstrate using YY_USER_ACTION to count action invocations. */

%smflex 100
%{
#include <assert.h>          /* assert */

int ctr[YY_NUM_RULES+1];     /* Entry 0 not used here. */

#define YY_USER_ACTION \
  assert(1 <= yy_act && yy_act <= YY_NUM_RULES); \
  ++ctr[yy_act];

%}

%%
foo        YY_ECHO;             /* rule 1 */
bar        YY_ECHO;             /* rule 2 */
[ ]+       YY_ECHO;             /* rule 3 */
                             /* rule 4: implicit default */
%%

int main()
{
  int i;
  yy_lexer_t lexer;

  yy_construct(&lexer);
  while (yy_lex(&lexer))
    {}
  for (i=1; i <= YY_NUM_RULES; i++) {
    printf("rule %d executed %d times\n", i, ctr[i]);
  }

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
