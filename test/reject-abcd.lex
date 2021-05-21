/* reject-abcd.lex */
/* Demonstration of REJECT and continued actions. */

%option main
%option reject

/* BEGIN: example fragment */
%%
a        |
ab       |
abc      |
abcd     YY_ECHO; REJECT;
.|\n     /* eat up any unmatched character */
