/* comments.lex */
/* Experiment with where comments can go. */

/* Comments ok. */ /* Comments ok. */
  /* Comments ok. */ /* Comments ok. */

%smflex 100
%option main

/* Comments ok. */ /* Comments ok. */
  /* Comments ok. */ /* Comments ok. */

%%

  int x = 5;
  int y = 15;     /* Still "user decls". */
/* Comments ok. */ /* Comments ok. */

username    printf("%s", "USERNAME");
x           printf("%d", x++);
y           printf("%d", y++);
z           /* Comment as 'z' action. */ /* More */

/* Comments ok. */ /* Comments ok. */

%%
