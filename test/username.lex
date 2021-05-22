/* username.lex */
/* Very simple example based on the first one in the manual. */

  /* BEGIN: example fragment */
%smflex 100
%%
username    printf("USERNAME");
  /* END: example fragment */

%%
/* EOF */
