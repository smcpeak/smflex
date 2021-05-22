/* comments.lex */
/* Experiment with where comments can go. */

/* Comments ok. */ /* Comments ok. */
  /* Comments ok. */ /* Comments ok. */

%smflex 100
%option main

/* Comments ok. */ /* Comments ok. */
  /* Comments ok. */ /* Comments ok. */

%%
username    printf("%s", "USERNAME");

%%
