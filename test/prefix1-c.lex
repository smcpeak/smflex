/* prefix1-c.lex */
/* One of several lexers combined with "%option prefix". */

%smflex 100
%option prefix="prefix1"

/* Request all of the optional features so we can check that they
 * get renamed properly too. */
%option yy_scan_string
%option yy_scan_bytes
%option yy_scan_buffer
%option yy_read_character
%option yy_unread_character

%%

prefix        printf("prefix1");
