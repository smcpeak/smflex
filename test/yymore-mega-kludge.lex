/* yymore-mega-kludge.lex */
/* Demonstrate yymore(). */

%smflex 100
%option main

/* BEGIN: example fragment */
%option yymore
%%
mega-    YY_ECHO; YY_MORE_TEXT();
kludge   YY_ECHO;
