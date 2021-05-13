/* c-prefix.lex */
/* Exercise "%option prefix" for the C interface. */

%option prefix="my_prefix"
%option main
%option yy_read_character
%option yy_unread_character

%%

one       printf("ONE");
read      YY_READ_CHARACTER();
unread    YY_UNREAD_CHARACTER('x');
