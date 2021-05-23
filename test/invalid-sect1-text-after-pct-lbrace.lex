/* invalid-sect1-text-after-pct-lbrace.lex */
/* Text after "%{". */

%smflex 100

%{   text after pct lbrace
  int x = 5;
%}

%%
x     printf("%d", x++);
