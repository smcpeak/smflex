/* misc.h */
/* Declarations for misc.c. */
/* See COPYING for license terms. */

#ifndef MISC_H
#define MISC_H

#include "flexchar.h"                  /* Char */

#include <stddef.h>                    /* size_t */
#include <stdio.h>                     /* FILE */

/* See definitions for documentation. */
void action_define(char *defname, int value);
void add_action(char *new_text);
void *allocate_array(int size, size_t element_size);
int all_lower(char *str);
int all_upper(char *str);
void bubble(int v[], int n);
void check_char(int c);
Char clower(int c);
char *copy_string(const char *str);
Char *copy_unsigned_string(Char *str);
void cshell(Char v[], int n, int special_case_0);
void dataend(void);
void dataflush(void);
void flexerror(const char msg[]);
void flexfatal(const char msg[]);
int htoi(Char str[]);
void lerrif(const char msg[], int arg);
void lerrsf(const char msg[], const char arg[]);
void line_directive_out(FILE *output_file, int do_infile);
void mark_defs1(void);
void mark_prolog(void);
void mk2data(int value);
void mkdata(int value);
int myctoi(char array[]);
Char myesc(Char array[]);
int otoi(Char str[]);
void out(const char str[]);
void out_dec(const char fmt[], int n);
void out_dec2(const char fmt[], int n1, int n2);
void out_hex(const char fmt[], unsigned int x);
void out_line_count(const char str[]);
void out_str(const char fmt[], const char str[]);
void out_str3(const char fmt[], const char s1[],
              const char s2[], const char s3[]);
void out_str_dec(const char fmt[], const char str[], int n);
void outc(int c);
void outn(const char str[]);
char *readable_form(int c);
void *reallocate_array(void *array, int size, size_t element_size);
void skelout(void);
void transition_struct_out(int element_v, int element_n);
void *yy_flex_xmalloc(int size);
void zero_out(char *region_ptr, size_t size_in_bytes);

#endif /* MISC_H */
