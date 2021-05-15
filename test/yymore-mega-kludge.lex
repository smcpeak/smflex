/* yymore-mega-kludge.lex */
/* Demonstrate yymore(). */

%option main

/* BEGIN: example fragment */
%%
mega-    YY_ECHO; yymore();
kludge   YY_ECHO;
