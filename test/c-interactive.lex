/* c-interactive.lex */
/* Exercise %option interactive. */

%option interactive
%option main

%{
#include <stdlib.h>        /* exit */
%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
