/* action-pct-brace.lex */
/* Demonstrate using %{...%} in actions. */

%smflex 100
%option main

%%
  /* BEGIN: example fragment */
username    %{
              printf("USERNAME");
            %}
  /* END: example fragment */

  /* This is currently tolerated, but should not be. */
there       %{ printf("ereht"); %}

blah        %{
              printf("b l a h");
              /* Not a problem: %} */
            %}
