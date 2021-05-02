/* gen.h */
/* Declarations for gen.c. */
/* See COPYING for license terms. */

#ifndef GEN_H
#define GEN_H

#include <stdio.h>                     /* FILE */

/* See definitions for documentation. */
void do_indent(void);
void gen_backing_up(void);
void gen_bu_action(void);
void genctbl(void);
void genecs(void);
void gen_find_action(void);
void genftbl(void);
void gen_next_compressed_state(char *char_map);
void gen_next_match(void);
void gen_next_state(int worry_about_NULs);
void gen_NUL_trans(void);
void gen_start_state(void);
void gentabs(void);
void indent_put2s(char fmt[], char arg[]);
void indent_puts(char str[]);
void make_tables(void);
void emit_with_class_name_substitution(FILE *fp, char const *line);
void emit_header_file(char const *header_file_name);

#endif /* GEN_H */