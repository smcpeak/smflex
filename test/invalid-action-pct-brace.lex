/* invalid-action-pct-brace.lex */
/* %{...%} in actions is no longer allowed. */

%smflex 100
%option main

%%
username    %{
              printf("USERNAME");
            %}
