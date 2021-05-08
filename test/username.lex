/* username.lex */
/* Very simple example based on the first one in the manual. */

  /* The example in the manual is meant to be calling POSIX
   * 'getlogin()', but for the sake of portability I am
   * substituting this instead. */
  static char const *getlogin()
  {
    return "USERNAME";
  }

  /* BEGIN: example fragment */
%%
username    printf("%s", getlogin());
  /* END: example fragment */

%%
/* EOF */
