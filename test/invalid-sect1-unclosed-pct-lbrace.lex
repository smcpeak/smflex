/* invalid-sect1-unclosed-pct-lbrace.lex */
/* "%{" without "%}. */

%smflex 100

%{
  int x = 5;
% }

%%
  /* still in code block! */
