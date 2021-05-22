/* c-int-batch.lex */
/* Exercise %option batch. */

%smflex 100
%option batch
%option main

%%
username           printf("USERNAME");
exit               printf("exiting...\n"); exit(0);

%%
/* EOF */
