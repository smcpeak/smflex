/* large-read-buffer.lex */
/* Demonstrate using a larger read buffer than normal. */

%smflex 100

/* The buffer constants can be set in section 1. */
%{
#define YY_MAX_READ_SIZE 0x80000 /* 512 KiB */
#define YY_DEFAULT_BUF_ALLOC_SIZE 0x100000 /* 1 MiB */

int num_plain_chars = 0;
int num_newlines = 0;
%}

%%

.*     num_plain_chars += YY_LENG;
\n     num_newlines++;

%%

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
    if (supplied_length % 80 == 0) {
      /* Insert newlines reasonably often so we do not just have one
       * enormous token. */
      buf[i] = '\n';
    }
    else {
      buf[i] = 'x';
    }
    supplied_length++;
  }

  return size;
}


int main()
{
  yy_lexer_t lexer;
  yy_construct(&lexer);
  lexer.yy_read_input_function = &my_read_input;

  total_length = 0x1000000; /* 16 MiB */
  supplied_length = 0;
  while (yy_lex(&lexer))
    {}

  yy_destroy(&lexer);
  yy_check_for_memory_leaks();

  printf("chars=%d newlines=%d\n", num_plain_chars, num_newlines);
  return 0;
}
