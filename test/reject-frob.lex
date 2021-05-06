/* reject-frob.lex */
/* First example in REJECT section of the manual. */

        static void special(void);

        /* BEGIN: fragment in manual */
        int word_count = 0;
%%

frob        special(); REJECT;
[^ \t\n]+   ++word_count;
        /* END: fragment in manual */

[ \t\n]     /* Nothing; do not echo. */

%%

static int frob_count = 0;

static void special()
{
  frob_count++;
}

int main()
{
  while (yylex())
    {}

  printf("word_count: %d\n", word_count);
  printf("frob_count: %d\n", frob_count);
  return 0;
}


/* EOF */
