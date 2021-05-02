/* input-scan.h */
/* Declarations for functions defined in input-scan.lex. */
/* See COPYING for license terms. */

#ifndef INPUT_SCAN_H
#define INPUT_SCAN_H

#include <stddef.h>                    /* size_t */

/* The Flex-generated scanner for flex.  Usually, this is called
 * 'yylex', but input-scan.lex uses YY_DECL to change that.  Then,
 * the file yylex.c defines 'yylex()' as a wrapper around
 * 'flexscan()' for debug purposes. */
int flexscan(void);

/* See definitions in input-scan.lex for documentation. */
int yywrap(void);
void set_input_file(char *file);
void *flex_alloc(size_t size);
void *flex_realloc(void *, size_t size);
void flex_free(void *ptr);

#endif /* INPUT_SCAN_H */
