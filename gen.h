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
void genftbl(void);
void gen_next_compressed_state(char *char_map);
void gen_next_match(void);
void gen_next_state(int worry_about_NULs);
void gen_NUL_trans(void);
void gentabs(void);
void indent_put2s(char const *fmt, char const *arg);
void indent_puts(char str[]);
void make_tables(void);
void emit_with_name_substitution(FILE *fp, char const *line);
int emit_skeleton_lines_upto(
  FILE *dest, char const **skeleton_lines, int skeleton_index,
  char const *expected_label);
void emit_header_file(char const *header_file_name);

#endif /* GEN_H */
