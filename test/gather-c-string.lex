/* gather-c-string.lex */
/* Illustrate use of start conditions to gather and interpret a C
 * string literal. */

%{
#include <assert.h>          /* assert */
#include <stdlib.h>          /* strtol */

#define MAX_STR_LITERAL_LEN 512
#define TOK_STR_LITERAL 1
%}

/* BEGIN: example fragment */
%x str
        char string_buf[MAX_STR_LITERAL_LEN];
        char *string_buf_ptr;
%%
\"      string_buf_ptr = string_buf; YY_SET_START_STATE(str);

<str>\"        { /* saw closing quote - all done */
          YY_SET_START_STATE(INITIAL);
          *string_buf_ptr = '\0';

          /* Inform caller that we saw and interpreted a string literal.
           * The caller can collect it from 'string_buf'. */
          return TOK_STR_LITERAL;
        }

<str>\n        {
          printf("unterminated string literal\n");
          YY_SET_START_STATE(INITIAL);     /* Recover from the error. */
        }

<str>\\[0-7]{1,3} {
          /* Octal escape sequence. */
          int result = (int)strtol(yytext+1, NULL /*endp*/, 8 /*base*/);
          assert(0 <= result && result <= 0xFF);
          *string_buf_ptr++ = result;
        }

<str>\\[0-9]+ {
          /* generate error - bad escape sequence; something
           * like '\48' or '\0777777' */
          printf("bad escape sequence: \"%s\"\n", yytext);
        }

<str>\\n  *string_buf_ptr++ = '\n';
<str>\\t  *string_buf_ptr++ = '\t';
<str>\\r  *string_buf_ptr++ = '\r';
<str>\\b  *string_buf_ptr++ = '\b';
<str>\\f  *string_buf_ptr++ = '\f';

<str>\\(.|\n)  *string_buf_ptr++ = yytext[1];

<str>[^\\\n\"]+        {
          char *p = yytext;
          while (*p) {
            *string_buf_ptr++ = *p++;
          }
        }
  /* END: example fragment */

.|\n      /* Discard non-string-literals. */

%%

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);
  while (yy_lex(&lexer)) {
    printf("string literal: %s\n", string_buf);
  }
  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
  return 0;
}
