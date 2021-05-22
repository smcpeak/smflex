/* reject-abcd.lex */
/* Demonstration of YY_REJECT and continued actions. */

%smflex 100
%option main
%option reject

/* BEGIN: example fragment */
%%
a        |
ab       |
abc      |
abcd     YY_ECHO; YY_REJECT;
.|\n     /* eat up any unmatched character */
