/* vert-bar-action.lex */
/* Lightly exercise the '|' (vertical bar) action. */

%smflex 100
%option main

%%
username           printf("USERNAME");

foo                |
bar                printf("foo_or_bar");

%%
/* EOF */
