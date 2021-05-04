/* c-int-default.lex */
/* Default interactivity for a C-interface scanner. */

%option main

%{
#include <stdlib.h>        /* exit */
%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
