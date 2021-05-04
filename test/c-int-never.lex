/* c-int-never.lex */
/* Exercise %option never-interactive for a C-interface scanner. */

%option never-interactive
%option main

%{
#include <stdlib.h>        /* exit */
%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
