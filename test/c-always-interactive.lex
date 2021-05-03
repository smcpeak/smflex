/* c-always-interactive.lex */
/* Exercise %option always-interactive. */

%option always-interactive
%option main

%{
#include <stdlib.h>        /* exit */
%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
