/* invalid-sect2p-repeated-pct-lbrace.lex */
/* "%{" then "%{". */

%smflex 100
%%

%{
  int x = 5;
%{
%}

%%
