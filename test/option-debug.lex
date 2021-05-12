/* option-debug.lex */
/* Exercise "%option debug". */

%option main
%option debug

%%

one      printf("ONE");
two      printf("TWO");
three    printf("THREE");
