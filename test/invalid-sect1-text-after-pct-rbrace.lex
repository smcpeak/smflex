/* invalid-sect1-text-after-pct-rbrace.lex */
/* Text after "%}". */

%smflex 100

%{
  int x = 5;
%}   text after pct rbrace

x     printf("%d", x++);
%%
