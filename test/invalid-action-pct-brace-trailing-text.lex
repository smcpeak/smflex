/* invalid-action-pct-brace-trailing-text.lex */
/* Action %{...%} with trailing text. */

%smflex 100
%option main

%%
username    %{
              printf("USERNAME");
            %}    text cannot go here
there       %{ printf("ereht"); %}
