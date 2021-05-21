/* input-scan.lex.h */
/* Scanner interface definition generated by smflex. */
/* DO NOT EDIT MANUALLY. */

/* This file was automatically created by smflex.  It is not subject to
 * copyright restrictions. */

#ifndef INPUT_SCAN_LEX_H
#define INPUT_SCAN_LEX_H

#include <stdio.h>                     /* FILE */

/* Opaque types defined in the generated scanner code. */
typedef struct input_scan_buffer_state_struct input_scan_buffer_state_t;

/* Types of input and output streams.  The scanner core treats these
 * as opaque; only 'yy_read_input_function' and
 * 'yy_write_output_function' use them in a non-opaque way, and those
 * can be replaced by the client.
 *
 * Thus, the actual types used here are effectively default
 * suggestions, not requirements. */
typedef FILE input_scan_input_stream_t;
typedef FILE input_scan_output_stream_t;


/* Error codes that can be reported to 'yy_error_function'. */
typedef enum input_scan_error_code_enum {
  /* No error has occurred.  This will not be reported as an error code,
   * rather it is used simply to indicate the absence of an error. */
  input_scan_err_no_error = 0,

  /* Something went wrong with the scanner internal logic.  Either there
   * is a bug in smflex or the API was misused (but that was not
   * detected directly). */
  input_scan_err_internal_error,

  /* The API was misused by passing data that violates its constraints
   * or invoking functions in an invalid sequence. */
  input_scan_err_api_misuse,

  /* An attempt to read from the input source failed. */
  input_scan_err_input_error,

  /* A single token was larger than the size of the input buffer, and
   * the buffer cannot be expanded due to how the scanner was
   * configured. */
  yy_err_input_buffer_cannot_expand,

  /* A single token was larger than the size of the input buffer, and
   * it cannot be expanded because the new size would not be
   * representable with 'int'. */
  yy_err_input_buffer_at_max_size,

  /* The default rule has been suppressed, and none of the
   * user-specified rules matches the scanned input.  'flex' refers to
   * this condition as "scanner jammed". */
  yy_err_no_rule_matches,

  /* The scanner failed to allocate more memory when needed. */
  input_scan_err_out_of_memory,

  /* The 'input_scan_unread_character' function tried to add a character, but
   * the buffer is not large enough to hold it and the current token. */
  input_scan_err_unread_overflow,

  /* The start condition stack has hit its maximum size.  The exact size
   * depends on configuration parameters, but is usually at least one
   * billion (INT_MAX/2). */
  input_scan_err_condition_stack_overflow,

  /* The number of error codes, starting with 0. */
  input_scan_err_num_error_codes
} input_scan_error_code_t;


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
struct input_scan_lexer_struct {
  /* -------- Public members -------- */
  /* Application-specific data.  The client of this interface is free
   * to use this value however they want.  'input_scan_construct' sets it to
   * NULL. */
  void *yy_client_data;

  /* Output sink.  This is initially a FILE*, namely 'stdout'.  Since
   * there is no buffering of output within 'smflex', this can be
   * directly reassigned at any time. */
  input_scan_output_stream_t *yy_output_stream;

  /* Read up to 'size' bytes into 'dest'.  The presumed source of the
   * data is 'yy_input_stream', but that is up to the client.  Return
   * the number of bytes read, or 0 for end of file, or -1 for a read
   * error.
   */
  /* The initial value is '&input_scan_read_input_with_fread'. */
  int (*yy_read_input_function)(struct input_scan_lexer_struct *yy_lexer,
    void *dest, int size);

  /* Write 'size' bytes from 'src'.  The presumed destination is
   * 'yy_output_stream'.  Return 'size' for success or -1 for error.
   *
   * The initial value is '&input_scan_write_output_with_fwrite'. */
  int (*yy_write_output_function)(struct input_scan_lexer_struct *yy_lexer,
    void const *src, int size);

  /* When the end of the current input is read, the scanner calls this
   * to determine what to do next.  It can return true, meaning is no
   * more input, and the scanner will terminate.  It can instead return
   * false (0) after setting up a new input source, meaning the scanner
   * will continue scanning with that new input.
   *
   * The initial value is '&input_scan_wrap_return_1'.
   *
   * The type of the first parameter should be thought of as
   * 'input_scan_lexer_t*'. */
  int (*yy_wrap_function)(struct input_scan_lexer_struct *yy_lexer);

  /* Invoked when an error occurs during scanning.  The function may
   * return, in which case the scanner will try to recover by returning
   * control to the client (typically by pretending it hit the end of
   * the input).  In C++, the function may also throw an exception.
   *
   * After this function has been called, the scanner object should be
   * regarded as broken after that point, and hence the client should
   * stop trying to scan and call 'input_scan_destroy' to clean up instead.
   *
   * The 'yy_lexer' parameter is a pointer to const because errors are
   * sometimes reported from within functions that are not supposed to
   * modify the lexer object.  Nothing catastrophic should happen if
   * constness is cast away, but clients are encouraged to try to
   * adhere to its restrictions.
   *
   * If not NULL, 'detail' is some additional bit of detail that may be
   * of use to a human, but is not useful for error recovery.
   *
   * The initial value is '&input_scan_error_print_and_exit'. */
  void (*yy_error_function)(struct input_scan_lexer_struct const *yy_lexer,
    input_scan_error_code_t code, char const *detail);

  /* -------- Semi-public members -------- */
  /* Input source.  This is initially a FILE*, namely 'stdin'. */
  input_scan_input_stream_t *yy_input_stream;

  /* After a rule pattern has been matched, this is set to point at the
   * matched text within 'yy_current_buffer', temporarily
   * NUL-terminated.
   *
   * This pointer is only valid while an action is executing, or between
   * when input_scan_lex() returns a non-zero value and the next invocation of a
   * input_scan_lex(), or any function that modifies the read buffer except for
   * input_scan_less_text().
   *
   * NOTE: Within section 2 actions, there is also a 'YY_TEXT' macro
   * that expands to 'yy_lexer->yy_text'. */
  char const *yy_text;

  /* After a rule pattern has been matched, this is set to the length
   * of the matched text, in bytes.
   *
   * NOTE: As with 'yy_text', this field has an associated 'YY_LENG'
   * macro active in section 2 actions. */
  int yy_leng;

  /* The input source we are currently reading from, and a buffer
   * in front of it.  Initially NULL.  If it remains NULL when input_scan_lex()
   * is first called, it will be created then. */
  input_scan_buffer_state_t *yy_current_buffer;

  /* -------- Private members -------- */
  /* The error code of the last error reported, or
   * 'input_scan_err_no_error' if none has. */
  input_scan_error_code_t yy_error_code;

  /* Holds the character overwritten by a NUL when 'yy_text' is formed.
   *
   * Generally, when we are not in the middle of the matching algorithm,
   * 'yy_hold_char' contains the value that belongs in '*yy_buf_cur_pos'. */
  char yy_hold_char;

  /* Number of characters in 'yy_current_buffer->yy_buffer' that need to
   * be scanned, not including the two extra EOB markers.
   *
   * Invariant: 0 <= yy_buf_data_len <= yy_current_buffer->yy_buf_alloc_size
   * Invariant: yy_current_buffer->yy_buffer[yy_buf_data_len+0] == YY_END_OF_BUFFER_CHAR
   * Invariant: yy_current_buffer->yy_buffer[yy_buf_data_len+1] == YY_END_OF_BUFFER_CHAR
   */
  int yy_buf_data_len;

  /* Points to current character in buffer, i.e., the next character
   * to scan.
   *
   * Invariant: yy_current_buffer->yy_buffer <= yy_buf_cur_pos
   * Invariant: yy_buf_cur_pos <= yy_current_buffer->yy_buffer + yy_buf_data_len + 2
   *
   * I'm not sure, but I think it is possible for yy_buf_cur_pos to reach
   * the one-past-the-end location, i.e., yy_buf_data_len + 2. */
  char *yy_buf_cur_pos;

  /* True if we need to initialize inside 'input_scan_lex()'.
   *
   * 'flex' has an undocumented (outside the NEWS file) feature where
   * YY_USER_INIT can set this back to 1 to force another initializaton
   * on a subsequent invocation of 'input_scan_lex()'.  I am very skeptical
   * that that is a good idea, but I do not have a pressing need to
   * remove that feature or this field, so for now they stay. */
  int yy_need_init;

  /* Current start state number.  This is a "start" state in the sense
   * that it indicates the state in which the recognizer begins when
   * 'input_scan_lex()' is called; the recognizer then transitions from there
   * depending on what characters are seen.
   *
   * Beware that this is distinct from the "start condition", which is
   * the number exposed to the user in the API (they differ by some
   * arithmetic).*/
  int yy_start_state;

  /* These are related to backing up when YY_REJECT is *not* used. */
  input_scan_state_type_t yy_last_accepting_state;
  char *yy_last_accepting_cpos;

  /* These variables are used to implement "%option stack". */
  int yy_start_stack_cur_size;         /* Number of elements in stack. */
  int yy_start_stack_alloc_size;       /* Allocated size of stack. */
  int *yy_start_stack_array;           /* The stack.  [0] is bottom. */
};

typedef struct input_scan_lexer_struct input_scan_lexer_t;


#ifdef __cplusplus
extern "C" {
#endif

/* -------- Essentials -------- */
/* Initialize 'yy_lexer'.  This begins the lifecycle of a lexer
 * object. */
void input_scan_construct(input_scan_lexer_t *yy_lexer);

/* Destroy the contents of 'yy_lexer'.  This deallocates any
 * dynamically-allocated memory acquired by the scanner engine.  The
 * object itself is not deallocated; that is the client's
 * responsibility.
 *
 * If the lexer has a current buffer, the current buffer is deallocated
 * by calling 'input_scan_delete_buffer'. */
void input_scan_destroy(input_scan_lexer_t *yy_lexer);

/* Given a lexer object initialized with 'input_scan_construct', and possibly
 * with its input specified with 'input_scan_restart', begin scanning it.
 * If this returns 0, it means the end of the input was reached.
 * Otherwise, it returns an integer whose meaning is determined by
 * the scanner rules, but typically indicates the kind of token that
 * was found (e.g., identifier, number, string, etc.). */
int input_scan_lex(input_scan_lexer_t *yy_lexer  );

/* Abandon whatever input (if any) 'yy_lexer' was scanning, and start
 * scanning 'input_file'.  This is how to choose what to scan;
 * otherwise, the scanner reads standard input.
 *
 * If there was no current buffer, a new buffer is allocated.
 * Otherwise, the current buffer is repurposed to read from
 * 'input_file', flushing that buffer in the process. */
void input_scan_restart(input_scan_lexer_t *yy_lexer, input_scan_input_stream_t *input_file);

/* -------- Manipulating matched text -------- */
/* Put a suffix of the matched text back into the read buffer so it will
 * be scanned next.  This function can only be called when
 * 'yy_lexer->yy_text' could be accessed.  'new_yy_leng' must be in
 * [0,yy_leng], and it determines the new value of 'yy_lexer->yy_leng'.
 * The amount of text put back is the difference between the old and
 * new lengths. */
void input_scan_less_text(input_scan_lexer_t *yy_lexer, int new_yy_leng);

/* -------- Scanning multiple sources (e.g., #includes) -------- */
/* Create a new buffer for use with 'yy_lexer' that reads from 'file'.
 * The 'size' is the size of the read buffer; a size of 0 means to use
 * the default size smflex uses for its own buffers.
 *
 * If out of memory, returns NULL after calling 'yy_error_function'. */
input_scan_buffer_state_t *input_scan_create_buffer(input_scan_lexer_t *yy_lexer, input_scan_input_stream_t *file,
                                    int size);

/* Make 'new_buffer' the active input source for 'yy_lexer'.  The old
 * buffer is not remembered by the lexer in any way; it is up to the
 * client to do so.  (The old buffer is also not freed.)
 *
 * This does nothing if 'new_buffer' is already the current buffer. */
void input_scan_switch_to_buffer(input_scan_lexer_t *yy_lexer, input_scan_buffer_state_t *new_buffer);

/* Deallocate 'b' and release any resources associated with it.  This
 * does nothing if 'b' is NULL.
 *
 * If 'b' is the current buffer of 'yy_lexer', then afterward the
 * current buffer is NULL. */
void input_scan_delete_buffer(input_scan_lexer_t *yy_lexer, input_scan_buffer_state_t *b);

/* Discard any already-read data from the input source associated with
 * 'b'.  Future attempts to read will start by reading new data from
 * that source. */
void input_scan_flush_buffer(input_scan_lexer_t *yy_lexer, input_scan_buffer_state_t *b);

/* -------- Manipulating the start condition -------- */
/* Set the start condition of 'yy_lexer' to 'cond'. */
void input_scan_set_start_condition(input_scan_lexer_t *yy_lexer, int cond);

/* Get the current start condition. */
int input_scan_get_start_condition(input_scan_lexer_t const *yy_lexer);

/* Set the start condition to 'new_cond', pushing the current start
 * condition onto the state stack. */
void yy_push_start_condition(input_scan_lexer_t *yy_lexer, int new_cond);

/* Set the start condition to the element at the top of the stack and
 * remove that element.  The stack must not be empty. */
void yy_pop_start_condition(input_scan_lexer_t *yy_lexer);

/* Get the element at the top of the stack without changing it.  The
 * stack must not be empty. */
int yy_top_start_condition(input_scan_lexer_t const *yy_lexer);

/* Return true if the start condition stack is empty. */
int yy_condition_stack_is_empty(input_scan_lexer_t const *yy_lexer);

/* -------- Interacting with an input stream -------- */
/* Read a single character from the current input buffer of 'yy_lexer'.
 * Returns EOF (-1) on end of file. */
int input_scan_read_character(input_scan_lexer_t *yy_lexer);

/* Push a single character back into the input buffer of 'yy_lexer',
 * such that it will be the next one read. */
void input_scan_unread_character(input_scan_lexer_t *yy_lexer, int c);

/* Set the 'interactive' flag on the given 'yy_lexer'.  An interactive
 * lexer reads its input one character at a time. */
void input_scan_set_interactive(input_scan_lexer_t *yy_lexer, int is_interactive);

/* Get whether 'yy_lexer' will regard itself as being at the beginning
 * of a line (BOL), which is where "^" patterns can match. */
int input_scan_get_bol(input_scan_lexer_t const *yy_lexer);

/* Set the BOL condition. */
void input_scan_set_bol(input_scan_lexer_t *yy_lexer, int at_bol);

/* -------- Available methods for function pointers -------- */
/* Use Standard C 'fread()'.  This assumes that 'yy_input_stream'
 * is a 'FILE*'. */
int input_scan_read_input_with_fread(input_scan_lexer_t *yy_lexer,
  void *dest, int size);

/* Use Standard C 'fwrite()'.  This assumes that 'yy_output_stream'
 * is a 'FILE*'.*/
int input_scan_write_output_with_fwrite(input_scan_lexer_t *yy_lexer,
  void const *dest, int size);

/* Return 1, meaning no more files. */
int input_scan_wrap_return_1(input_scan_lexer_t *yy_lexer);

/* Print an error message to stderr and exit. */
void input_scan_error_print_and_exit(input_scan_lexer_t const *yy_lexer,
  input_scan_error_code_t code, char const *detail);

/* -------- Diagnostics -------- */
/* Return the code of the error the scanner encountered, or
 * 'input_scan_err_no_error' if none has been.  This can be used in situations
 * where 'yy_error_function' returns in order to signal to the client
 * code that it must stop calling scanner functions. */
input_scan_error_code_t input_scan_get_error(input_scan_lexer_t const *yy_lexer);

/* Return an English string describing the given error 'code' as a
 * pointer to statically-allocated memory. */
char const *input_scan_error_string(input_scan_error_code_t code);

/* Fail an assertion if there are any objects allocated, across all
 * scanner instances, that have not been freed.  This can be called
 * after destroying all instances to check for memory leaks. */
void input_scan_check_for_memory_leaks(void);

#ifdef __cplusplus
}
#endif



#endif /* INPUT_SCAN_LEX_H */
