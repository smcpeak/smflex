/* gather-c-string2.lex */
/* Illustrate use of start condition scopes. */

%{
#include <assert.h>          /* assert */
#include <stdlib.h>          /* strtol */

#define MAX_STR_LITERAL_LEN 512
#define TOK_STR_LITERAL 1
%}

%x str
        char string_buf[MAX_STR_LITERAL_LEN];
        char *string_buf_ptr;
%%
\"      string_buf_ptr = string_buf; BEGIN(str);

<str>{
  \"    { /* saw closing quote - all done */
          BEGIN(INITIAL);
          *string_buf_ptr = '\0';

          /* Inform caller that we saw and interpreted a string literal.
           * The caller can collect it from 'string_buf'. */
          return TOK_STR_LITERAL;
        }

  \n    {
          printf("unterminated string literal\n");
          BEGIN(INITIAL);     /* Recover from the error. */
        }

  \\[0-7]{1,3} {
          /* Octal escape sequence. */
          int result = (int)strtol(yytext+1, NULL /*endp*/, 8 /*base*/);
          assert(0 <= result && result <= 0xFF);
          *string_buf_ptr++ = result;
        }

  \\[0-9]+ {
          /* generate error - bad escape sequence; something
           * like '\48' or '\0777777' */
          printf("bad escape sequence: \"%s\"\n", yytext);
        }

  \\n   *string_buf_ptr++ = '\n';
  \\t   *string_buf_ptr++ = '\t';
  \\r   *string_buf_ptr++ = '\r';
  \\b   *string_buf_ptr++ = '\b';
  \\f   *string_buf_ptr++ = '\f';

  \\(.|\n)  *string_buf_ptr++ = yytext[1];

  [^\\\n\"]+        {
          char *p = yytext;
          while (*p) {
            *string_buf_ptr++ = *p++;
          }
        }
}

.|\n      /* Discard non-string-literals. */

%%

int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);
  while (yylex(&lexer)) {
    printf("string literal: %s\n", string_buf);
  }
  yy_destroy(&lexer);
  return 0;
}
