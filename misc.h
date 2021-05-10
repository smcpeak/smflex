/* misc.h */
/* Declarations for misc.c. */
/* See COPYING for license terms. */

#ifndef MISC_H
#define MISC_H

#include "flexchar.h"                  /* Char */

#include <stddef.h>                    /* size_t */
#include <stdio.h>                     /* FILE */


/* This is a special line directive that gets inserted into the
 * in-memory action text at a location where we want to substitute
 * a #line directive pointing at the output file.
 *
 * The use of a marker like this is ugly, but it is difficult to do
 * better given the current design of the code that accumulates the
 * action text to emit. */
extern char const yy_output_file_line_directive[];


/* See definitions for documentation. */
void action_define(char const *defname, int value);
void add_action(char const *new_text);
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
void flexerror_ss(char const *fmt, char const *s1, char const *s2);
void flexfatal(const char msg[]);
void flexfatal_s(char const *fmt, char const *s);
int htoi(Char str[]);
void lerrif(const char msg[], int arg);
void lerrsf(const char msg[], const char arg[]);
void line_directive_out(FILE *output_file, int do_infile);
void line_directive_out_src(void);
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
void out_with_limit(char const *str, size_t length);
char *readable_form(int c);
void *reallocate_array(void *array, int size, size_t element_size);
void skelout_upto(char const *expectedLabel);
int starts_with(char const *str, char const *prefix);
int str_eq(char const *s1, char const *s2);
void transition_struct_out(int element_v, int element_n);
void *yy_flex_xmalloc(int size);
void zero_out(char *region_ptr, size_t size_in_bytes);


/* Next, we have a bunch of convenience wrappers around the
 * allocation functions. */

#define allocate_integer_array(size) \
        (int *) allocate_array( size, sizeof( int ) )

#define reallocate_integer_array(array,size) \
        (int *) reallocate_array( (void *) array, size, sizeof( int ) )

#define allocate_int_ptr_array(size) \
        (int **) allocate_array( size, sizeof( int * ) )

#define allocate_char_ptr_array(size) \
        (char **) allocate_array( size, sizeof( char * ) )

#define allocate_dfaacc_union(size) \
        (union dfaacc_union *) \
                allocate_array( size, sizeof( union dfaacc_union ) )

#define reallocate_int_ptr_array(array,size) \
        (int **) reallocate_array( (void *) array, size, sizeof( int * ) )

#define reallocate_char_ptr_array(array,size) \
        (char **) reallocate_array( (void *) array, size, sizeof( char * ) )

#define reallocate_dfaacc_union(array, size) \
        (union dfaacc_union *) \
        reallocate_array( (void *) array, size, sizeof( union dfaacc_union ) )

#define allocate_character_array(size) \
        (char *) allocate_array( size, sizeof( char ) )

#define reallocate_character_array(array,size) \
        (char *) reallocate_array( (void *) array, size, sizeof( char ) )

#define allocate_Character_array(size) \
        (Char *) allocate_array( size, sizeof( Char ) )

#define reallocate_Character_array(array,size) \
        (Char *) reallocate_array( (void *) array, size, sizeof( Char ) )


#endif /* MISC_H */
