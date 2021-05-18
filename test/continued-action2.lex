/* continued-action2.lex
 * Another example of a continued action that breaks #line directives. */

%option main

/* Toss in a test of the 'yyterminate' compatibility macro. */
%option flex-compat

%%

<<EOF>> |
"x" {
  printf("hi\n");
  yyterminate();
}

%%

/* EOF */
