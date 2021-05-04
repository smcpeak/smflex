/* c-int-batch-never.lex */
/* Exercise %option batch and never-interactive. */

%option batch
%option never-interactive
%option main

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
