/* c-int-default.lex */
/* Default interactivity for a C-interface scanner. */

%smflex 100
%option main

%{
#include <stdlib.h>        /* exit */
%}

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
