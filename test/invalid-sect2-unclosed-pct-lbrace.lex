/* invalid-sect2-text-after-pct-rbrace.lex */
/* "%{" without "%}. */

%smflex 100
%%

%{
  int x = 5;
% }

%%
  /* still in code block! */
