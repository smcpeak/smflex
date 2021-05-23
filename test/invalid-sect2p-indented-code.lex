/* invalid-sect2p-indented-code.lex */
/* Indented code in section 2 prolog. */

%smflex 100
%%

  int x = 5;

x      printf("%d", x++);
