/* input-buffers.lex */
/* Demonstrate use of multiple input buffers. */

%{
#include <assert.h>          /* assert */
#include <stdio.h>           /* fprintf, fopen, etc. */
#include <stdlib.h>          /* exit */
%}

%option main

/* BEGIN: example fragment */
/* The "incl" state is used for picking up the name of an include file. */
%x incl

%{
#define MAX_INCLUDE_DEPTH 10
yy_buffer_state_t *include_stack[MAX_INCLUDE_DEPTH];
int include_stack_len = 0;
%}

%%
include[ ]+         YY_SET_START_STATE(incl);

[a-z]+              YY_ECHO;
[^a-z\n]*\n?        YY_ECHO;

<incl>[^\n]+      { /* got the include file name */
          yy_buffer_state_t *newbuf;

          FILE *fp = fopen(yytext, "r");
          if (!fp) {
            fprintf(stderr, "Cannot open include file: \"%s\".\n", yytext);
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
          /* We are done with the current buffer. */
          if (include_stack_len >= 1) {
            /* The current buffer is one we made, so clean it up. */
            yy_delete_buffer(yy_lexer, YY_CURRENT_BUFFER);
            fclose(yyin);

            /* Return to the buffer on the top of the stack.  Note
             * that this sets 'yyin'. */
            yy_switch_to_buffer(yy_lexer, include_stack[--include_stack_len]);
          }

          else {
            /* All done. */
            YY_TERMINATE();
          }
        }
