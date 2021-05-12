/* input-scan.h */
/* Declarations for functions defined in input-scan.lex. */
/* See COPYING for license terms. */

#ifndef INPUT_SCAN_H
#define INPUT_SCAN_H

#include "input-scan.lex.h"            /* input_scan_lexer_t */

#include <stddef.h>                    /* size_t */

/* The state for our input scanner. */
extern input_scan_lexer_t input_lexer;

/* Invoke the smflex-generated input scanner. */
int flexscan(void);

/* See definitions in input-scan.lex for documentation. */
void set_input_file(char *file);
void *flex_alloc(size_t size);
void *flex_realloc(void *, size_t size);
void flex_free(void *ptr);

#endif /* INPUT_SCAN_H */
