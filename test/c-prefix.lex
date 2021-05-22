/* c-prefix.lex */
/* Exercise "%option prefix" for the C interface. */

%smflex 100
%option prefix="my_prefix"
%option main

/* Request all of the optional features so we can check that they
 * get renamed properly too. */
%option yy_scan_string
%option yy_scan_bytes
%option yy_scan_buffer
%option yy_read_character
%option yy_unread_character

%%

one       printf("ONE");
read      YY_READ_CHARACTER();
unread    YY_UNREAD_CHARACTER('x');

%%

/* Make it appear that some functions are used. */
void use_functions(my_prefix_lexer_t *lexer)
{
  char buf[4] = {'h','i',0,0};

  my_prefix_scan_string(lexer, "hi");
  my_prefix_scan_bytes(lexer, "hi", 2);
  my_prefix_scan_buffer(lexer, buf, 4);
}

/* EOF */
