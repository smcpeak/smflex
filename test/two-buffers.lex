/* two-buffers.lex */
/* Make two buffers and interleave their input. */

%{
#include <assert.h>                    /* assert */

yy_buffer_state_t *bufs[2];
int curBuf = 0;
%}

%%

.|\n     {
           YY_ECHO;

           /* Alternate between the buffers.  Whichever hits EOF first
            * should cause scanning to stop. */
           curBuf = !curBuf;
           yy_switch_to_buffer(yy_lexer, bufs[curBuf]);
         }

%%

int main()
{
  FILE *fps[2];
  int i;
  yy_lexer_t lexer;

  fps[0] = fopen("two-buffers.input", "rb");
  assert(fps[0]);
  fps[1] = fopen("two-buffers.input2", "rb");
  assert(fps[1]);

  yy_construct(&lexer);

  for (i=0; i < 2; i++) {
    bufs[i] = yy_create_buffer(&lexer, fps[i], 0);
    assert(bufs[i]);
  }

  yy_switch_to_buffer(&lexer, bufs[0]);

  while (yy_lex(&lexer))
    {}

  for (i=0; i < 2; i++) {
    yy_delete_buffer(&lexer, bufs[i]);
    fclose(fps[i]);
  }

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
