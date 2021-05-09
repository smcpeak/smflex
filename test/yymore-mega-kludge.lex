/* yymore-mega-kludge.lex */
/* Demonstrate yymore(). */

%option main

/* BEGIN: example fragment */
%%
mega-    ECHO; yymore();
kludge   ECHO;
