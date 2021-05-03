/* c-batch.lex */
/* Exercise %option batch. */

%option batch
%option main

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
