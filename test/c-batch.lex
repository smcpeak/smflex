/* c-batch.lex */
/* Exercise %option batch. */

%option batch
%option main

%%
username           printf("USERNAME");

%%
/* EOF */
