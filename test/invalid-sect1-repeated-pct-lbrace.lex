/* invalid-sect1-repeated-pct-lbrace.lex */
/* "%{" then "%{". */

%smflex 100

%{
  int x = 5;
%{
%}

%%
