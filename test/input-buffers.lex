/* input-buffers.lex */
/* Demonstrate use of multiple input buffers. */

%{
#include <assert.h>          /* assert */
#include <stdio.h>           /* fprintf, fopen, etc. */
#include <stdlib.h>          /* exit */
%}

/* BEGIN: example fragment */
/* The "incl" state is used for picking up the name of an include file. */
%x incl

%{
/* Stack of in-flight buffers. */
#define MAX_INCLUDE_DEPTH 10
yy_buffer_state_t *include_stack[MAX_INCLUDE_DEPTH];
int include_stack_len = 0;

/* List of files to process at top level. */
char const *file_list[3] =
  { "input-buffers.input2", "input-buffers.input3", "input-buffers.input4" };
int file_list_index = 0;
%}

%%
include[ ]+         YY_SET_START_STATE(incl);

[a-z]+              YY_ECHO;
[^a-z\n]*\n?        YY_ECHO;

<incl>[^\n]+      { /* got the include file name */
          yy_buffer_state_t *newbuf;

          FILE *fp = fopen(YY_TEXT, "r");
          if (!fp) {
            fprintf(stderr, "Cannot open include file: \"%s\".\n", YY_TEXT);
            exit(2);
          }

          if (include_stack_len >= MAX_INCLUDE_DEPTH) {
            fprintf(stderr, "Includes nested too deeply.\n");
            exit(2);
          }
          include_stack[include_stack_len++] = YY_CURRENT_BUFFER;

          newbuf = yy_create_buffer(yy_lexer, fp, 0 /*size*/);
          yy_switch_to_buffer(yy_lexer, newbuf);

          /* Start condition for processing the new file. */
          YY_SET_START_STATE(INITIAL);
        }

<<EOF>> {
          /* Finished with this file; close it. */
          fclose(YY_INPUT_STREAM);

          if (include_stack_len >= 1) {
            /* The current buffer is one we made, so clean it up. */
            yy_delete_buffer(yy_lexer, YY_CURRENT_BUFFER);

            /* Return to the buffer on the top of the stack.  Note
             * that this sets 'YY_INPUT_STREAM'. */
            yy_switch_to_buffer(yy_lexer, include_stack[--include_stack_len]);
          }

          else if (file_list_index < 3) {
            /* Open the next top-level file. */
            char const *fname = file_list[file_list_index++];
            FILE *fp = fopen(fname, "r");
            if (!fp) {
              fprintf(stderr, "Cannot open top-level file: \"%s\".\n", fname);
              exit(2);
            }

            /* Begin scanning it, continuing to use the current buffer. */
            yy_restart(yy_lexer, fp);
          }

          else {
            /* All done. */
            YY_TERMINATE();
          }
        }
  /* END: example fragment */

%%

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);
  while (yy_lex(&lexer))
    {}
  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
