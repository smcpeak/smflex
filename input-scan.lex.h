/* input-scan.lex.h */
/* Scanner interface definition generated by smflex. */
/* DO NOT EDIT MANUALLY. */

/* Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Kent Williams and Tom Epperly.
 *
 * 2021-04-30: It has (much later) been heavily modified by
 * Scott McPeak.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef input_scan_lexer_t_DEFINED
#define input_scan_lexer_t_DEFINED

#include <stddef.h>                    /* size_t */
#include <stdio.h>                     /* FILE */

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque types defined in the generated scanner code. */
typedef struct input_scan_buffer_state_struct input_scan_buffer_state;

/* This is the type of 'yy_current_state' (among other things), which
 * is the current state within the finite state automaton that is
 * responsible for recognizing tokens. */
typedef int input_scan_state_type;


/* This struct encapsulates the scanner state. */
struct input_scan_lexer_state_struct {
  /* Application-specific data.  The client of this interface is free
   * to use this value however they want.  'input_scan_construct' sets it to
   * NULL. */
  void *yy_client_data;

  /* After a rule pattern has been matched, this is set to point at the
   * matched text within 'yy_current_buffer', temporarily
   * NUL-terminated.
   *
   * NOTE: Within section 2 actions, there is also a 'yytext' (without
   * an underscore) macro that expands to 'yy_lexer->yy_text'. */
  char *yy_text;

  /* After a rule pattern has been matched, this is set to the length
   * of the matched text, in bytes.
   *
   * NOTE: As with 'yy_text', this field has an associated 'yyleng'
   * macro active in section 2 actions. */
  int yy_leng;


  /* TODO: Only needed if: ddebug */
  int yy_flex_debug;      /* only has effect with -d or "%option debug" */

  /* Input source for default YY_INPUT. */
  FILE *yy_input_stream;

  /* Output sink for default ECHO. */
  FILE *yy_output_stream;

  /* The input source we are currently reading from, and a buffer
   * in front of it. */
  input_scan_buffer_state *yy_current_buffer;

  /* Holds the character overwritten by a NUL when 'yy_text' is formed. */
  char yy_hold_char;

  /* Number of characters read into yy_ch_buf. */
  int yy_n_chars;

  /* Points to current character in buffer. */
  char *yy_c_buf_p;

  /* True if we need to initialize. */
  /* TODO: Can I remove this now that I have 'input_scan_construct'? */
  int yy_init;

  /* Current start state number. */
  int yy_start;

  /* Flag which is used to allow input_scan_wrap()'s to do buffer switches
   * instead of setting up a fresh 'yy_input_stream'.
   * A bit of a hack ... */
  int yy_did_buffer_switch_on_eof;

  /* The following are not always needed, but may be depending
   * on use of certain smflex features (like REJECT or yymore()). */

  /* TODO: Only needed when: num_backing_up > 0 && !reject_used */
  input_scan_state_type yy_last_accepting_state;
  char *yy_last_accepting_cpos;

  /* TODO: Only needed when: reject_used */
  input_scan_state_type *yy_state_buf;
  input_scan_state_type *yy_state_ptr;
  char *yy_full_match;
  int yy_lp;

  /* TODO: Only needed when: reject_used && variable_trailing_context_rules */
  int yy_looking_for_trail_begin;
  int yy_full_lp;
  int *yy_full_state;

  /* TODO: Only needed when: yymore_used */
  int yy_more_flag;
  int yy_more_len;

  /* TODO: I do not think either of these is ever used. */
  int yy_more_offset;
  int yy_prev_more_offset;

  /* TODO: Create a variable to track when "%option stack" is used,
   * and only define these in that case. */
  int yy_start_stack_ptr;
  int yy_start_stack_depth;
  int *yy_start_stack;
};

typedef struct input_scan_lexer_state_struct input_scan_lexer_t;


/* Initialize 'yy_lexer'.  This begins the lifecycle of a lexer
 * object. */
void input_scan_construct(input_scan_lexer_t *yy_lexer);

/* Destroy the contents of 'yy_lexer'.  This deallocates any
 * dynamically-allocated memory acquired by the scanner engine.  The
 * object itself is not deallocated; that is the client's
 * responsibility. */
void input_scan_destroy(input_scan_lexer_t *yy_lexer);

/* Given a lexer object initialized with 'input_scan_construct', and possibly
 * with its input specified with 'input_scan_restart', begin scanning it.
 * If this returns 0, it means the end of the input was reached.
 * Otherwise, it returns an integer whose meaning is determined by
 * the scanner rules, but typically indicates the kind of token that
 * was found (e.g., identifier, number, string, etc.). */
int input_scan_lex(input_scan_lexer_t *yy_lexer);

/* Abandon whatever input (if any) 'yy_lexer' was scanning, and start
 * scanning 'input_file'. */
void input_scan_restart(input_scan_lexer_t *yy_lexer, FILE *input_file);

/* Pointer to an opaque type that represents an input source along
 * with a read buffer for that source.  See the manual section
 * "Multiple Input Buffers" for more details. */
typedef input_scan_buffer_state *INPUT_SCAN_BUFFER_STATE;

/* Create a new buffer for use with 'yy_lexer' that reads from 'file'.
 * The 'size' is the size of the read buffer; 8192 is a reasonable
 * value to use. */
/* TODO: The manual advises using YY_BUF_SIZE, but that value is not
 * currently exported. */
INPUT_SCAN_BUFFER_STATE input_scan_create_buffer(input_scan_lexer_t *yy_lexer, FILE *file,
                                 int size);

/* Make 'new_buffer' the active input source for 'yy_lexer'. */
void input_scan_switch_to_buffer(input_scan_lexer_t *yy_lexer, INPUT_SCAN_BUFFER_STATE new_buffer);

/* Deallocate 'b' and release any resources associated with it. */
void input_scan_delete_buffer(input_scan_lexer_t *yy_lexer, INPUT_SCAN_BUFFER_STATE b);

/* Discard any already-read data from the input source associated with
 * 'b'.  Future attempts to read will start by reading new data from
 * that source. */
void input_scan_flush_buffer(input_scan_lexer_t *yy_lexer, INPUT_SCAN_BUFFER_STATE b);

/* Allocate a new buffer (which must be deallocated with
 * 'input_scan_delete_buffer') to scan the contents of NUL-terminated 'yy_str'.
 * Switch to that buffer. */
INPUT_SCAN_BUFFER_STATE input_scan_scan_string(input_scan_lexer_t *yy_lexer, const char *yy_str);

/* Like 'input_scan_scan_string', but without the presumption of NUL
 * termination. */
INPUT_SCAN_BUFFER_STATE input_scan_scan_bytes(input_scan_lexer_t *yy_lexer, const char *bytes, int len);

/* Whereas the preceding two functions make a copy of the source data,
 * this one scans it without making a copy.  The last two bytes *must*
 * be 0.  See the description in the manual. */
INPUT_SCAN_BUFFER_STATE input_scan_scan_buffer(input_scan_lexer_t *yy_lexer, char *base, size_t size);

/* Set the start state of 'yy_lexer' to 'state'.  This function must be
 * used instead of BEGIN when not within a rule action. */
void input_scan_begin(input_scan_lexer_t *yy_lexer, int state);

/* Set the 'interactive' flag on the given 'yy_lexer'.  An interactive
 * yy_lexer reads its input one character at a time. */
void input_scan_set_interactive(input_scan_lexer_t *yy_lexer, int is_interactive);

/* Set whether 'yy_lexer' will regard itself as being at the beginning
 * of a line (BOL), which is where "^" patterns can match. */
void yy_set_bol(input_scan_lexer_t *yy_lexer, int at_bol);

/* The 'smflex' user must define this function.  It can return 0 after
 * calling 'input_scan_restart' to begin processing another file, or return 1 to
 * indicate there are no more files to process. */
int input_scan_wrap(input_scan_lexer_t *yy_lexer);

#ifdef __cplusplus
}
#endif



#endif /* input_scan_lexer_t_DEFINED */
