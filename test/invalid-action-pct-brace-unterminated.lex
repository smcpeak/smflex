/* invalid-action-pct-brace-unterminated.lex */
/* Action %{...%} except there is no %}. */

%smflex 100
%option main

%%
username    %{
              printf("USERNAME");
