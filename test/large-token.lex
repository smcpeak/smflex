/* large-token.lex */
/* Exercise scanning a token so large it requires buffer reallocation. */

%{
#include <string.h>                    /* memset */
%}

%%

"abcdefghij"+       printf("saw %d letters in a row\n", yyleng);
.|\n                /* nothing */

%%

static yy_error_code_t ecode;
static char const *edetail;

static void record_error(yy_lexer_t *yy_lexer,
                         yy_error_code_t code, char const *detail)
{
  ecode = code;
  edetail = detail;
}

/* How many characters to supply. */
int total_length = 0;

/* How many of that we have supplied. */
int supplied_length = 0;

int my_read_input(yy_lexer_t *yy_lexer, void *dest, int size)
{
  char *buf = (char*)dest;
  int i;

  if (supplied_length == total_length) {
    return 0;
  }

  if (size > total_length - supplied_length) {
    size = total_length - supplied_length;
  }

  for (i=0; i < size; i++) {
    /* Data is "abcdefghij" over and over.  This pattern is meant to
     * make it possible to recognize if something in the buffer logic
     * corrupts the data.*/
    buf[i] = (supplied_length % 10) + 'a';
    supplied_length++;
  }

  return size;
}

/* Defined in the generated scanner when
 * YY_ALLOCATOR_FAILURE_INJECTION is defined. */
extern unsigned yy_alloc_failure_countdown;

void scan_length(int len)
{
  yy_lexer_t lexer;

  printf("--------- scan_length(%d) ----------\n", len);

  yy_construct(&lexer);
  lexer.yy_read_input_function = &my_read_input;
  lexer.yy_error_function = &record_error;

  total_length = len;
  supplied_length = 0;
  while (yy_lex(&lexer))
    {}

  if (yy_get_error(&lexer)) {
    printf("lexing failed: %s\n", yy_error_string(yy_get_error(&lexer)));
  }
  yy_destroy(&lexer);
  yy_check_for_memory_leaks();
}

int main()
{
  scan_length(100);
  scan_length(1000);
  scan_length(10000);
  scan_length(100000);

  if (1) {
    /* Failure count 2 is a different recovery issue. */
    /* And a leak that I need to come back to. */
    /*yy_alloc_failure_countdown = 2;*/
    yy_alloc_failure_countdown = 4;
    scan_length(100000);
  }
  return 0;
}
