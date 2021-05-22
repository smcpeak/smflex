/* username-read.lex */
/* Exercise "%option read". */

%smflex 100
%option read
%option main

  /* BEGIN: example fragment */
%%
username    printf("USERNAME");
  /* END: example fragment */

%%
/* EOF */
