/* input-buffers-with-read.lex */
/* Multiple input buffers but exclusively using 'yy_read_character'. */

%smflex 100
%{
#include <assert.h>          /* assert */
#include <stdio.h>           /* fprintf, fopen, etc. */
#include <stdlib.h>          /* exit */

/* Stack of in-flight buffers. */
#define MAX_INCLUDE_DEPTH 10
yy_buffer_state_t *include_stack[MAX_INCLUDE_DEPTH];
int include_stack_len = 0;

/* List of files to process at top level. */
char const *file_list[3] = {
  "input-buffers-with-read.input2",
  "input-buffers.input3",
  "input-buffers.input4"
};
int file_list_index = 0;
%}

%option yy_read_character

%%

  /* No rules.  This is an API test. */

%%

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);

  if (1) {
    /* TODO: Change how init works so this is not necessary. */
    yy_restart(&lexer, lexer.yy_input_stream);
  }

  while (1) {
    int c = yy_read_character(&lexer);
    if (c == 'I') {
      /* Collect name of include file. */
      char fname[80];
      int i = 0;
      yy_buffer_state_t *newbuf;
      FILE *fp;

      while (1) {
        c = yy_read_character(&lexer);
        if (c == EOF || c == '\n') {
          /* Done with file name. */
          break;
        }
        assert(i < 79);
        fname[i++] = (char)c;
      }
      fname[i] = 0;

      fp = fopen(fname, "r");
      if (!fp) {
        fprintf(stderr, "Cannot open include file: \"%s\".\n", fname);
        exit(2);
      }

      if (include_stack_len >= MAX_INCLUDE_DEPTH) {
        fprintf(stderr, "Includes nested too deeply.\n");
        exit(2);
      }
      include_stack[include_stack_len++] = lexer.yy_current_buffer;

      newbuf = yy_create_buffer(&lexer, fp, 0 /*size*/);
      yy_switch_to_buffer(&lexer, newbuf);
    }

    else if (c == EOF) {
      /* Finished with this file; close it. */
      fclose(lexer.yy_input_stream);

      if (include_stack_len >= 1) {
        /* The current buffer is one we made, so clean it up. */
        yy_delete_buffer(&lexer, lexer.yy_current_buffer);

        /* Return to the buffer on the top of the stack.  Note
         * that this sets 'YY_INPUT_STREAM'. */
        yy_switch_to_buffer(&lexer, include_stack[--include_stack_len]);
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
        yy_restart(&lexer, fp);
      }

      else {
        /* All done. */
        break;
      }
    }

    else {
      printf("%c", c);
    }
  }

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
