/* outfile.lex */
/* Exercise %option outfile. */

/* Choose output file name that differs not only from the default of
 * lex.yy.c, but also from what the usual Makefile rule would choose. */
%option outfile="out/outfile-specified.yy.c"

%option main

%%
username           printf("USERNAME");

%%
/* EOF */
