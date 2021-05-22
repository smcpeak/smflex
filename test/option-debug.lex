/* option-debug.lex */
/* Exercise "%option debug". */

%smflex 100
%option main
%option debug

%%

one      printf("ONE");
two      printf("TWO");
three    printf("THREE");
