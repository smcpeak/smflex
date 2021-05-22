/* c-int-interactive.lex */
/* Exercise %option interactive. */

%smflex 100
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
