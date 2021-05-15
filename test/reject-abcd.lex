/* reject-abcd.lex */
/* Demonstration of REJECT and continued actions. */

%option main

/* BEGIN: example fragment */
%%
a        |
ab       |
abc      |
abcd     YY_ECHO; REJECT;
.|\n     /* eat up any unmatched character */
