/* count-lines-chars.lex */
/* BEGIN: example fragment */
        int num_lines = 0, num_chars = 0;

%%
\n      ++num_lines; ++num_chars;
.       ++num_chars;

%%
int main() {
  yy_lexer_t lexer;
  yy_construct(&lexer);
  yy_lex(&lexer);
  printf("# of lines = %d, # of chars = %d\n",
         num_lines, num_chars);
  yy_destroy(&lexer);
  return 0;
}
