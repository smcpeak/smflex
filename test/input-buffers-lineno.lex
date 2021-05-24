/* input-buffers-lineno.lex */
/* Demonstrate use of multiple input buffers and %option yylineno. */

%smflex 100
%option yylineno

/* The "incl" state is used for picking up the name of an include file. */
%x incl

%{
#include <assert.h>          /* assert */
#include <stdio.h>           /* fprintf, fopen, etc. */
#include <stdlib.h>          /* exit */
#include <string.h>          /* strlen, strcpy */

/* Stack of in-flight buffers. */
#define MAX_INCLUDE_DEPTH 10
yy_buffer_state_t *include_stack[MAX_INCLUDE_DEPTH];
char *fname_stack[MAX_INCLUDE_DEPTH];
int lineno_stack[MAX_INCLUDE_DEPTH];
int include_stack_len = 0;

/* Name of current input file, malloc'd. */
char *current_fname = NULL;

/* List of files to process at top level. */
char const *file_list[3] =
  { "input-buffers.input2", "input-buffers.input3", "input-buffers.input4" };
int file_list_index = 0;

static char *my_strdup(char const *src)
{
  char *ret = (char*)malloc(strlen(src)+1);
  strcpy(ret, src);
  return ret;
}
%}

%%
include[ ]+         YY_SET_START_CONDITION(incl);

getloc              printf("[getloc: %s:%d]", current_fname, YY_LINENO);

[a-z]+              YY_ECHO;
[^a-z\n]*\n?        YY_ECHO;

<incl>[^\n]+      { /* got the include file name */
          yy_buffer_state_t *newbuf;

          FILE *fp = fopen(YY_TEXT, "r");
          if (!fp) {
            fprintf(stderr, "Cannot open include file: \"%s\".\n", YY_TEXT);
            exit(2);
          }

          /* Save current info. */
          if (include_stack_len >= MAX_INCLUDE_DEPTH) {
            fprintf(stderr, "Includes nested too deeply.\n");
            exit(2);
          }
          include_stack[include_stack_len] = YY_CURRENT_BUFFER;
          fname_stack[include_stack_len] = current_fname;
          lineno_stack[include_stack_len] = YY_LINENO;
          include_stack_len++;

          /* Set new 'current' data.  This has to be done before calling
           * 'yy_switch_to_buffer' because that destroys 'YY_TEXT'. */
          current_fname = my_strdup(YY_TEXT);
          YY_LINENO = 1;

          /* Set up new buffer. */
          newbuf = yy_create_buffer(yy_lexer, fp, 0 /*size*/);
          yy_switch_to_buffer(yy_lexer, newbuf);

          /* Start condition for processing the new file. */
          YY_SET_START_CONDITION(INITIAL);
        }

<<EOF>> {
          /* Finished with this file; close it. */
          fclose(YY_INPUT_STREAM);
          free(current_fname);

          if (include_stack_len >= 1) {
            /* The current buffer is one we made, so clean it up. */
            yy_delete_buffer(yy_lexer, YY_CURRENT_BUFFER);

            /* Return to the buffer on the top of the stack.  Note
             * that this sets 'YY_INPUT_STREAM'. */
            include_stack_len--;
            yy_switch_to_buffer(yy_lexer, include_stack[include_stack_len]);
            current_fname = fname_stack[include_stack_len];
            YY_LINENO = lineno_stack[include_stack_len];
          }

          else if (file_list_index < 3) {
            /* Open the next top-level file. */
            char const *fname = file_list[file_list_index++];
            FILE *fp = fopen(fname, "r");
            if (!fp) {
              fprintf(stderr, "Cannot open top-level file: \"%s\".\n", fname);
              exit(2);
            }

            /* Update current file info. */
            current_fname = my_strdup(fname);
            YY_LINENO = 1;

            /* Begin scanning it, continuing to use the current buffer. */
            yy_restart(yy_lexer, fp);
          }

          else {
            /* All done. */
            YY_TERMINATE();
          }
        }

%%

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);

  current_fname = my_strdup("<stdin>");

  while (yy_lex(&lexer))
    {}
  yy_destroy(&lexer);

  /* 'current_fname' was freed in the <<EOF>> action, above. */

  yy_check_for_memory_leaks();
  return 0;
}
