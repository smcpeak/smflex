/* vert-bar-action.lex */
/* Lightly exercise the '|' (vertical bar) action. */

%option main

%%
username           printf("USERNAME");

foo                |
bar                printf("foo_or_bar");

%%
/* EOF */
