/* username-read.lex */
/* Exercise "%option read". */

%option read
%option main

  /* BEGIN: example fragment */
%%
username    printf("USERNAME");
  /* END: example fragment */

%%
/* EOF */
