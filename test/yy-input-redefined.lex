/* yy-input-redefined.lex */
/* Demonstrate an alterative definition of YY_INPUT. */

%option main

/* BEGIN: example fragment */
%{
#define YY_INPUT(buf, result, max_size) {          \
  int c = getchar();                               \
  result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
}
%}
/* END: example fragment */

%%
username    printf("USERNAME");
