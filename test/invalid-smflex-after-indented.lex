/* invalid-smflex-after-indented.lex */
/* %smflex appears after some indented code. */

  int x = 3;

%smflex 100
%%
