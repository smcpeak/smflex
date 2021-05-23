/* invalid-sect1-unmatched-pct-rbrace.lex */
/* "%}" without "%{. */

%smflex 100

  int x = 5;
%}

%%
