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

#ifndef INPUT_SCAN_LEX_H
#define INPUT_SCAN_LEX_H

#include <stddef.h>                    /* size_t */
#include <stdio.h>                     /* FILE */

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque types defined in the generated scanner code. */
typedef struct input_scan_buffer_state_struct input_scan_buffer_state_t;

/* This is the type of 'yy_current_state' (among other things), which
 * is the current state within the finite state automaton that is
 * responsible for recognizing tokens. */
typedef int input_scan_state_type_t;


/* This structure encapsulates the scanner state.
 *
 * There are three kinds of members:
 *
 * - Public members can be directly read and written.
 *
 * - Semi-public members can be directly read, but not written.
 *
 * - Private members cannot be directly accessed.
 *
 * In addition to the allowed forms of member access, the API functions,
 * below, can be used to manipulate this structure.
 *
 * However, when the C++ interface is in use, it is considered to be the
 * exclusive client of the C interface, and therefore the the client of
 * the C++ interface should not access any members directly.
 */
struct input_scan_lexer_state_struct {
  /* -------- Public members -------- */
  /* Application-specific data.  The client of this interface is free
   * to use this value however they want.  'input_scan_construct' sets it to
   * NULL. */
  void *yy_client_data;

  /* After a rule pattern has been matched, this is set to point at the
   * matched text within 'yy_current_buffer', temporarily
   * NUL-terminated.  It is only valid while an action is executing.
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

  /* Input source for default YY_INPUT. */
  FILE *yy_input_stream;

  /* Output sink for default ECHO. */
  FILE *yy_output_stream;

  /* When the end of the current input is read, if it is not NULL, the
   * scanner calls this to determine what to do next.  It can return
   * true, meaning is no more input, and the scanner will terminate.  It
   * can instead return false (0) after setting up a new input source,
   * meaning the scanner will continue scanning with that new input.
   * It is initially NULL.
   *
   * The type of the first parameter should be thought of as
   * 'input_scan_lexer_t*'. */
  int (*yy_wrap_function)(struct input_scan_lexer_state_struct *yy_lexer);

  /* -------- Semi-public members -------- */
  /* The input source we are currently reading from, and a buffer
   * in front of it. */
  input_scan_buffer_state_t *yy_current_buffer;

  /* -------- Private members -------- */
  /* Holds the character overwritten by a NUL when 'yy_text' is formed. */
  char yy_hold_char;

  /* Number of characters read into yy_ch_buf. */
  int yy_n_chars;

  /* Points to current character in buffer. */
  char *yy_c_buf_p;

  /* True if we need to initialize inside 'input_scan_lex()'.
   *
   * 'flex' has an undocumented (outside the NEWS file) feature where
   * YY_USER_INIT can set this back to 1 to force another initializaton
   * on a subsequent invocation of 'input_scan_lex()'.  I am very skeptical
   * that that is a good idea, but I do not have a pressing need to
   * remove that feature or this field, so for now they stay. */
  int yy_init;

  /* Current start state number.  This is a "start" state in the sense
   * that it indicates the state in which the recognizer begins when
   * 'input_scan_lex()' is called; the recognizer then transitions from there
   * depending on what characters are seen.  Beware that the value of
   * this field is different from what BEGIN and YY_START use.*/
  int yy_start_state;

  /* Flag which is used to allow input_scan_wrap()'s to do buffer switches
   * instead of setting up a fresh 'yy_input_stream'.
   * A bit of a hack ... */
  int yy_did_buffer_switch_on_eof;

  /* These are related to backing up when REJECT is *not* used. */
  input_scan_state_type_t yy_last_accepting_state;
  char *yy_last_accepting_cpos;

  /* These variables are used to implement "%option stack". */
  int yy_start_stack_cur_size;         /* Number of elements in stack. */
  int yy_start_stack_alloc_size;       /* Allocated size of stack. */
  int *yy_start_stack_array;           /* The stack.  [0] is bottom. */
};

typedef struct input_scan_lexer_state_struct input_scan_lexer_t;


/* -------- Essentials -------- */
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
 * scanning 'input_file'.  This is how to choose what to scan;
 * otherwise, the scanner reads standard input. */
void input_scan_restart(input_scan_lexer_t *yy_lexer, FILE *input_file);

/* -------- Scanning multiple sources (e.g., #includes) -------- */
/* Create a new buffer for use with 'yy_lexer' that reads from 'file'.
 * The 'size' is the size of the read buffer; a size of 0 means to use
 * the default size smflex uses for its own buffers. */
input_scan_buffer_state_t *input_scan_create_buffer(input_scan_lexer_t *yy_lexer, FILE *file,
                                    int size);

/* Make 'new_buffer' the active input source for 'yy_lexer'. */
void input_scan_switch_to_buffer(input_scan_lexer_t *yy_lexer, input_scan_buffer_state_t *new_buffer);

/* Deallocate 'b' and release any resources associated with it. */
void input_scan_delete_buffer(input_scan_lexer_t *yy_lexer, input_scan_buffer_state_t *b);

/* Discard any already-read data from the input source associated with
 * 'b'.  Future attempts to read will start by reading new data from
 * that source. */
void input_scan_flush_buffer(input_scan_lexer_t *yy_lexer, input_scan_buffer_state_t *b);

/* -------- Manipulating the start state -------- */
/* Set the start state of 'yy_lexer' to 'state'.  This function must be
 * used instead of BEGIN when not within a rule action. */
void input_scan_set_start_state(input_scan_lexer_t *yy_lexer, int state);

/* Get the current start state. */
int input_scan_get_start_state(input_scan_lexer_t *yy_lexer);

/* Set the start state to 'new_state', pushing the current start state
 * onto the state stack. */
void yy_push_state(input_scan_lexer_t *yy_lexer, int new_state);

/* Set the start state to the element at the top of the stack and
 * remove that element.  The stack must not be empty. */
void yy_pop_state(input_scan_lexer_t *yy_lexer);

/* -------- Interacting with an input stream -------- */
/* Read a single character from the current input buffer of 'yy_lexer'.
 * Returns EOF (-1) on end of file. */
int input_scan_read_character(input_scan_lexer_t *yy_lexer);

/* Push a single character back into the input buffer of 'yy_lexer',
 * such that it will be the next one read. */
void input_scan_unread_character(input_scan_lexer_t *yy_lexer, int c);

/* Set the 'interactive' flag on the given 'yy_lexer'.  An interactive
 * yy_lexer reads its input one character at a time. */
void input_scan_set_interactive(input_scan_lexer_t *yy_lexer, int is_interactive);

/* Set whether 'yy_lexer' will regard itself as being at the beginning
 * of a line (BOL), which is where "^" patterns can match. */
void input_scan_set_bol(input_scan_lexer_t *yy_lexer, int at_bol);

/* -------- Diagnostics -------- */
/* Fail an assertion if there are any objects allocated, across all
 * scanner instances, that have not been freed.  This can be called
 * after destroying all instances to check for memory leaks. */
void input_scan_check_for_memory_leaks(void);

#ifdef __cplusplus
}
#endif



#endif /* INPUT_SCAN_LEX_H */
