/* invalid-sect2-pct-brace.lex */
/* %{ %} code block in section 2 non-prolog. */

%smflex 100
%%

username      printf("USERNAME");

%{
  int x = 5;
%}

x             YY_ECHO;
