/* reject-abcd.lex */
/* Demonstration of REJECT and continued actions. */

%option main

/* BEGIN: example fragment */
%%
a        |
ab       |
abc      |
abcd     ECHO; REJECT;
.|\n     /* eat up any unmatched character */
