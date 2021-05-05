/* continued-action2.lex
 * Another example of a continued action that breaks #line directives. */

%option main

%%

<<EOF>> |
"x" {
  printf("hi\n");
  yyterminate();
}

%%

/* EOF */
