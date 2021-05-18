/* File created from generated-header.skl via encode.sh */

/* NULL-terminated array of NUL-terminated lines, *without* line
 * terminators. */
const char *header_skl_contents[] = {
  "%# generated-header.skl: Skeleton of generated header.",
  "%#",
  "%# This skeleton file was originally written by Scott McPeak in",
  "%# May 2021, and it is hereby contributed to the public domain.",
  "%# See historical/header-file-copyright.txt.",
  "",
  "/* This file was automatically created by smflex.  It is not subject to",
  " * copyright restrictions. */",
  "",
  "#ifndef yy_header_include_guard_name",
  "#define yy_header_include_guard_name",
  "",
  "%# Note: To avoid collisions with other libraries or with names the",
  "%# user has defined, all names contributed to the global namespace",
  "%# must begin with \"yy\" or \"YY\".",
  "%#",
  "#include <stddef.h>                    /* size_t */",
  "%if cpp_interface",
  "#include <iostream>                    /* std::istream, std::ostream */",
  "%else",
  "#include <stdio.h>                     /* FILE */",
  "%endif",
  "",
  "%# If we are not generating the C++ interface, but using a C++ compiler,",
  "%# make sure all of the functions have C linkage so they can be used",
  "%# from modules compiled with a C compiler.  However, if we *are*",
  "%# generating a C++ interface, then a C++ compiler must be used to",
  "%# parse this file, and hence the functions are not callable from C",
  "%# code, so we keep the C++ linkage.",
  "%if !cpp_interface",
  "#ifdef __cplusplus",
  "extern \"C\" {",
  "#endif",
  "%endif",
  "",
  "/* Opaque types defined in the generated scanner code. */",
  "typedef struct yy_buffer_state_struct yy_buffer_state_t;",
  "%if jacobson",
  "typedef struct yy_trans_info_struct yy_trans_info_t;",
  "%endif",
  "",
  "%# TODO: Arrange to use these definitions depending on options.",
  "%# typedef struct yy_input_stream_generic  yy_input_stream_t;",
  "%# typedef struct yy_output_stream_generic yy_output_stream_t;",
  "%#",
  "/* Types of input and output streams.  The scanner core treats these",
  " * as opaque; only 'yy_read_input_function' and",
  " * 'yy_write_output_function' use them in a non-opaque way, and those",
  " * can be replaced by the client.",
  " *",
  " * Thus, the actual types used here are effectively default",
  " * suggestions, not requirements. */",
  "%if !cpp_interface",
  "typedef FILE yy_input_stream_t;",
  "typedef FILE yy_output_stream_t;",
  "%else",
  "typedef std::istream yy_input_stream_t;",
  "typedef std::ostream yy_output_stream_t;",
  "%endif",
  "",
  "",
  "/* Error codes that can be reported to 'yy_error_function'. */",
  "typedef enum yy_error_code_enum {",
  "  /* No error has occurred.  This will not be reported as an error code,",
  "   * rather it is used simply to indicate the absence of an error. */",
  "  yy_err_no_error = 0,",
  "",
  "  /* Something went wrong with the scanner internal logic.  Either there",
  "   * is a bug in smflex or the API was misused (but that was not",
  "   * detected directly). */",
  "  yy_err_internal_error,",
  "",
  "  /* The API was misused by passing data that violates its constraints",
  "   * or invoking functions in an invalid sequence. */",
  "  yy_err_api_misuse,",
  "",
  "  /* An attempt to read from the input source failed. */",
  "  yy_err_input_error,",
  "",
  "  /* A single token was larger than the size of the input buffer, and",
  "   * the buffer cannot be expanded due to how the scanner was",
  "   * configured. */",
  "  yy_err_input_buffer_cannot_expand,",
  "",
  "  /* A single token was larger than the size of the input buffer, and",
  "   * it cannot be expanded because the new size would not be",
  "   * representable with 'size_t'. */",
  "  yy_err_input_buffer_at_max_size,",
  "",
  "  /* The default rule has been suppressed, and none of the",
  "   * user-specified rules matches the scanned input.  'flex' refers to",
  "   * this condition as \"scanner jammed\". */",
  "  yy_err_no_rule_matches,",
  "",
  "  /* The scanner failed to allocate more memory when needed. */",
  "  yy_err_out_of_memory,",
  "",
  "%if option_yy_unread_character",
  "  /* The 'yy_unread_character' function tried to add a character, but",
  "   * the buffer is not large enough to hold it and the current token. */",
  "  yy_err_unread_overflow,",
  "",
  "%endif",
  "%if option_stack",
  "  /* The start-state stack has hit its maximum size.  The exact size",
  "   * depends on configuration parameters, but is usually at least one",
  "   * billion (INT_MAX/2). */",
  "  yy_err_state_stack_overflow,",
  "",
  "%endif",
  "  /* The number of error codes, starting with 0. */",
  "  yy_err_num_error_codes",
  "} yy_error_code_t;",
  "",
  "",
  "/* This is the type of 'yy_current_state' (among other things), which",
  " * is the current state within the finite state automaton that is",
  " * responsible for recognizing tokens. */",
  "%if jacobson",
  "typedef yy_trans_info_t const *yy_state_type_t;",
  "%else",
  "typedef int yy_state_type_t;",
  "%endif",
  "",
  "%if cpp_interface",
  "class yyFlexLexer;           /* Defined below. */",
  "%endif",
  "",
  "/* This structure encapsulates the scanner state.",
  " *",
  " * There are three kinds of members:",
  " *",
  " * - Public members can be directly read and written.",
  " *",
  " * - Semi-public members can be directly read, but not written.",
  " *",
  " * - Private members cannot be directly accessed.",
  " *",
  " * In addition to the allowed forms of member access, the API functions,",
  " * below, can be used to manipulate this structure.",
  " *",
  " * However, when the C++ interface is in use, it is considered to be the",
  " * exclusive client of the C interface, and therefore the the client of",
  " * the C++ interface should not access any members directly.",
  " */",
  "struct yy_lexer_struct {",
  "  /* -------- Public members -------- */",
  "  /* Application-specific data.  The client of this interface is free",
  "   * to use this value however they want.  'yy_construct' sets it to",
  "   * NULL. */",
  "  void *yy_client_data;",
  "",
  "  /* After a rule pattern has been matched, this is set to point at the",
  "   * matched text within 'yy_current_buffer', temporarily",
  "   * NUL-terminated.",
  "   *",
  "   * This pointer is only valid while an action is executing, or between",
  "   * when yy_lex() returns a non-zero value and the next invocation of a",
  "   * yy_lex(), or any function that modifies the read buffer except for",
  "   * yy_less_text().",
  "   *",
  "   * NOTE: Within section 2 actions, there is also a 'YY_TEXT' macro",
  "   * that expands to 'yy_lexer->yy_text'. */",
  "  char *yy_text;",
  "",
  "  /* After a rule pattern has been matched, this is set to the length",
  "   * of the matched text, in bytes.",
  "   *",
  "   * NOTE: As with 'yy_text', this field has an associated 'YY_LENG'",
  "   * macro active in section 2 actions. */",
  "  int yy_leng;",
  "",
  "  /* Input source.  This is initially a FILE*, namely 'stdin'. */",
  "  yy_input_stream_t *yy_input_stream;",
  "",
  "  /* Output sink.  This is initially a FILE*, namely 'stdout'.  Since",
  "   * there is no buffering of output within 'smflex', this can be",
  "   * directly reassigned at any time. */",
  "  yy_output_stream_t *yy_output_stream;",
  "",
  "  /* Read up to 'size' bytes into 'dest'.  The presumed source of the",
  "   * data is 'yy_input_stream', but that is up to the client.  Return",
  "   * the number of bytes read, or 0 for end of file, or -1 for a read",
  "   * error.",
  "   */",
  "%if use_read",
  "  /* The initial value is '&yy_read_input_with_read'. */",
  "%else",
  "  /* The initial value is '&yy_read_input_with_fread'. */",
  "%endif",
  "  int (*yy_read_input_function)(struct yy_lexer_struct *yy_lexer,",
  "    void *dest, int size);",
  "",
  "  /* Write 'size' bytes from 'src'.  The presumed destination is",
  "   * 'yy_output_stream'.  Return 'size' for success or -1 for error.",
  "   *",
  "   * The initial value is '&yy_write_output_with_fwrite'. */",
  "  int (*yy_write_output_function)(struct yy_lexer_struct *yy_lexer,",
  "    void const *src, int size);",
  "",
  "  /* When the end of the current input is read, the scanner calls this",
  "   * to determine what to do next.  It can return true, meaning is no",
  "   * more input, and the scanner will terminate.  It can instead return",
  "   * false (0) after setting up a new input source, meaning the scanner",
  "   * will continue scanning with that new input.",
  "   *",
  "   * The initial value is '&yy_wrap_return_1'.",
  "   *",
  "   * The type of the first parameter should be thought of as",
  "   * 'yy_lexer_t*'. */",
  "  int (*yy_wrap_function)(struct yy_lexer_struct *yy_lexer);",
  "",
  "  /* Invoked when an error occurs during scanning.  The function may",
  "   * return, in which case the scanner will try to recover by returning",
  "   * control to the client (typically by pretending it hit the end of",
  "   * the input).  In C++, the function may also throw an exception.",
  "   *",
  "   * After this function has been called, the scanner object should be",
  "   * regarded as broken after that point, and hence the client should",
  "   * stop trying to scan and call 'yy_destroy' to clean up instead.",
  "   *",
  "   * The 'yy_lexer' parameter is a pointer to const because errors are",
  "   * sometimes reported from within functions that are not supposed to",
  "   * modify the lexer object.  Nothing catastrophic should happen if",
  "   * constness is cast away, but clients are encouraged to try to",
  "   * adhere to its restrictions.",
  "   *",
  "   * If not NULL, 'detail' is some additional bit of detail that may be",
  "   * of use to a human, but is not useful for error recovery.",
  "   *",
  "   * The initial value is '&yy_error_print_and_exit'. */",
  "  void (*yy_error_function)(struct yy_lexer_struct const *yy_lexer,",
  "    yy_error_code_t code, char const *detail);",
  "",
  "%if do_yylineno",
  "  /* Current line number in the input being scanned.  See the",
  "   * description in the manual for additional details. */",
  "  int yy_lineno;",
  "",
  "%endif",
  "%if option_debug",
  "  /* When true, print to stdout lines indicating which rules' actions",
  "   * are running.  Initially true. */",
  "  int yy_flex_debug;",
  "",
  "%endif",
  "  /* -------- Semi-public members -------- */",
  "  /* The input source we are currently reading from, and a buffer",
  "   * in front of it. */",
  "  yy_buffer_state_t *yy_current_buffer;",
  "",
  "  /* -------- Private members -------- */",
  "  /* The error code of the last error reported, or",
  "   * 'yy_err_no_error' if none has. */",
  "  yy_error_code_t yy_error_code;",
  "",
  "  /* Holds the character overwritten by a NUL when 'yy_text' is formed.",
  "   *",
  "   * Generally, when we are not in the middle of the matching algorithm,",
  "   * 'yy_hold_char' contains the value that belongs in '*yy_c_buf_p'. */",
  "  char yy_hold_char;",
  "",
  "  /* Number of characters in 'yy_current_buffer->yy_ch_buf' that need to",
  "   * be scanned, not including the two extra EOB markers.",
  "   *",
  "   * Invariant: 0 <= yy_n_chars <= yy_current_buffer->yy_buf_size",
  "   * Invariant: yy_current_buffer->yy_ch_buf[yy_n_chars+0] == 0",
  "   * Invariant: yy_current_buffer->yy_ch_buf[yy_n_chars+1] == 0",
  "   */",
  "  int yy_n_chars;",
  "",
  "  /* Points to current character in buffer, i.e., the next character",
  "   * to scan. */",
  "  char *yy_c_buf_p;",
  "",
  "  /* True if we need to initialize inside 'yy_lex()'.",
  "   *",
  "   * 'flex' has an undocumented (outside the NEWS file) feature where",
  "   * YY_USER_INIT can set this back to 1 to force another initializaton",
  "   * on a subsequent invocation of 'yy_lex()'.  I am very skeptical",
  "   * that that is a good idea, but I do not have a pressing need to",
  "   * remove that feature or this field, so for now they stay. */",
  "  int yy_init;",
  "",
  "  /* Current start state number.  This is a \"start\" state in the sense",
  "   * that it indicates the state in which the recognizer begins when",
  "   * 'yy_lex()' is called; the recognizer then transitions from there",
  "   * depending on what characters are seen.  Beware that the value of",
  "   * this field is different from what 'yy_set_start_state' and",
  "   * 'yy_get_start_state' use (they differ by some arithmetic).*/",
  "  int yy_start_state;",
  "",
  "  /* Flag which is used to allow yy_wrap()'s to do buffer switches",
  "   * instead of setting up a fresh 'yy_input_stream'.",
  "   * A bit of a hack ... */",
  "  int yy_did_buffer_switch_on_eof;",
  "",
  "%if num_backing_up > 0 && !reject_used",
  "  /* These are related to backing up when REJECT is *not* used. */",
  "  yy_state_type_t yy_last_accepting_state;",
  "  char *yy_last_accepting_cpos;",
  "",
  "%endif",
  "%if reject_used",
  "  /* These are related to REJECT. */",
  "  yy_state_type_t *yy_state_buf;",
  "  yy_state_type_t *yy_state_ptr;",
  "  char *yy_full_match;",
  "  int yy_lp;",
  "",
  "%endif",
  "%# The following conditional checks 'reject_used', but that is always",
  "%# set when 'variable_trailing_context_rules' is set.  I am leaving the",
  "%# redundancy just because the old code had it too and I do not want",
  "%# to poke this particular bear.",
  "%if reject_used && variable_trailing_context_rules",
  "  /* These are related to variable trailing context. */",
  "  int yy_looking_for_trail_begin;",
  "  int yy_full_lp;",
  "  int *yy_full_state;",
  "",
  "%endif",
  "%if yymore_used",
  "  /* These are related to 'yymore()'. */",
  "  int yy_more_flag;",
  "  int yy_more_len;",
  "",
  "%endif",
  "%if option_stack",
  "  /* These variables are used to implement \"%option stack\". */",
  "  int yy_start_stack_cur_size;         /* Number of elements in stack. */",
  "  int yy_start_stack_alloc_size;       /* Allocated size of stack. */",
  "  int *yy_start_stack_array;           /* The stack.  [0] is bottom. */",
  "%endif",
  "};",
  "",
  "typedef struct yy_lexer_struct yy_lexer_t;",
  "",
  "",
  "/* -------- Essentials -------- */",
  "/* Initialize 'yy_lexer'.  This begins the lifecycle of a lexer",
  " * object. */",
  "void yy_construct(yy_lexer_t *yy_lexer);",
  "",
  "/* Destroy the contents of 'yy_lexer'.  This deallocates any",
  " * dynamically-allocated memory acquired by the scanner engine.  The",
  " * object itself is not deallocated; that is the client's",
  " * responsibility. */",
  "void yy_destroy(yy_lexer_t *yy_lexer);",
  "",
  "%if cpp_interface",
  "/* This scanner is meant to be using the C++ wrapper class below.",
  " * Consequently, there is no 'yy_lex' global function.  Instead,",
  " * the there is a 'yym_lex' method on yyFlexLexer. */",
  "%else",
  "/* Given a lexer object initialized with 'yy_construct', and possibly",
  " * with its input specified with 'yy_restart', begin scanning it.",
  " * If this returns 0, it means the end of the input was reached.",
  " * Otherwise, it returns an integer whose meaning is determined by",
  " * the scanner rules, but typically indicates the kind of token that",
  " * was found (e.g., identifier, number, string, etc.). */",
  "int yy_lex(yy_lexer_t *yy_lexer);",
  "%endif",
  "",
  "/* Abandon whatever input (if any) 'yy_lexer' was scanning, and start",
  " * scanning 'input_file'.  This is how to choose what to scan;",
  " * otherwise, the scanner reads standard input. */",
  "void yy_restart(yy_lexer_t *yy_lexer, yy_input_stream_t *input_file);",
  "",
  "/* -------- Manipulating matched text -------- */",
  "/* Put a suffix of the matched text back into the read buffer so it will",
  " * be scanned next.  This function can only be called when",
  " * 'yy_lexer->yy_text' could be accessed.  'new_yy_leng' must be in",
  " * [0,yy_leng], and it determines the new value of 'yy_lexer->yy_leng'.",
  " * The amount of text put back is the difference between the old and",
  " * new lengths. */",
  "void yy_less_text(yy_lexer_t *yy_lexer, int new_yy_leng);",
  "",
  "/* -------- Scanning multiple sources (e.g., #includes) -------- */",
  "%if option_flex_compat",
  "/* Provided for flex compatibility. */",
  "typedef yy_buffer_state_t *YY_BUFFER_STATE;",
  "",
  "%endif",
  "/* Create a new buffer for use with 'yy_lexer' that reads from 'file'.",
  " * The 'size' is the size of the read buffer; a size of 0 means to use",
  " * the default size smflex uses for its own buffers.",
  " *",
  " * If out of memory, returns NULL after calling 'yy_error_function'. */",
  "yy_buffer_state_t *yy_create_buffer(yy_lexer_t *yy_lexer, yy_input_stream_t *file,",
  "                                    int size);",
  "",
  "/* Make 'new_buffer' the active input source for 'yy_lexer'. */",
  "void yy_switch_to_buffer(yy_lexer_t *yy_lexer, yy_buffer_state_t *new_buffer);",
  "",
  "/* Deallocate 'b' and release any resources associated with it. */",
  "void yy_delete_buffer(yy_lexer_t *yy_lexer, yy_buffer_state_t *b);",
  "",
  "/* Discard any already-read data from the input source associated with",
  " * 'b'.  Future attempts to read will start by reading new data from",
  " * that source. */",
  "void yy_flush_buffer(yy_lexer_t *yy_lexer, yy_buffer_state_t *b);",
  "",
  "%if option_yy_scan_string || option_yy_scan_bytes || option_yy_scan_buffer",
  "/* -------- Scanning in-memory data -------- */",
  "%endif",
  "%if option_yy_scan_string",
  "/* Allocate a new buffer (which must be deallocated with",
  " * 'yy_delete_buffer') to scan the contents of NUL-terminated 'yy_str'.",
  " * Switch to that buffer.",
  " *",
  " * If out of memory, returns NULL after calling 'yy_error_function'. */",
  "yy_buffer_state_t *yy_scan_string(yy_lexer_t *yy_lexer, const char *yy_str);",
  "",
  "%endif",
  "%if option_yy_scan_bytes",
  "/* Allocate a new buffer (which must be deallocated with",
  " * 'yy_delete_buffer') to scan 'len' bytes at 'bytes'.",
  " * Switch to that buffer.",
  " *",
  " * If out of memory, returns NULL after calling 'yy_error_function'. */",
  "yy_buffer_state_t *yy_scan_bytes(yy_lexer_t *yy_lexer, const char *bytes, int len);",
  "",
  "%endif",
  "%if option_yy_scan_buffer",
  "/* Allocate a new buffer (which must be deallocated with",
  " * 'yy_delete_buffer') to scan 'size' bytes at 'base'.",
  " * Switch to that buffer.",
  " *",
  " * The new buffer does not make a copy of 'base'; instead it uses it",
  " * in-place and destructively modifies it.  The last two bytes (at",
  " * [size-2] and [size-1]) *must* be 0, but are not treated as part of",
  " * the input to scan.  See the description in the manual.",
  " *",
  " * If out of memory, returns NULL after calling 'yy_error_function'. */",
  "yy_buffer_state_t *yy_scan_buffer(yy_lexer_t *yy_lexer, char *base, size_t size);",
  "",
  "%endif",
  "/* -------- Manipulating the start state -------- */",
  "/* Set the start state of 'yy_lexer' to 'state'. */",
  "void yy_set_start_state(yy_lexer_t *yy_lexer, int state);",
  "",
  "/* Get the current start state. */",
  "int yy_get_start_state(yy_lexer_t const *yy_lexer);",
  "",
  "%if option_stack",
  "/* Set the start state to 'new_state', pushing the current start state",
  " * onto the state stack. */",
  "void yy_push_state(yy_lexer_t *yy_lexer, int new_state);",
  "",
  "/* Set the start state to the element at the top of the stack and",
  " * remove that element.  The stack must not be empty. */",
  "void yy_pop_state(yy_lexer_t *yy_lexer);",
  "",
  "%if option_yy_top_state",
  "/* Get the element at the top of the stack without changing it.  The",
  " * stack must not be empty, but there is no way to query whether it is;",
  " * you just have to know from context. */",
  "int yy_top_state(yy_lexer_t const *yy_lexer);",
  "",
  "%endif",
  "%endif",
  "/* -------- Interacting with an input stream -------- */",
  "%if option_yy_read_character",
  "/* Read a single character from the current input buffer of 'yy_lexer'.",
  " * Returns EOF (-1) on end of file. */",
  "int yy_read_character(yy_lexer_t *yy_lexer);",
  "",
  "%endif",
  "%if option_yy_unread_character",
  "/* Push a single character back into the input buffer of 'yy_lexer',",
  " * such that it will be the next one read. */",
  "void yy_unread_character(yy_lexer_t *yy_lexer, int c);",
  "",
  "%endif",
  "/* Set the 'interactive' flag on the given 'yy_lexer'.  An interactive",
  " * yy_lexer reads its input one character at a time. */",
  "void yy_set_interactive(yy_lexer_t *yy_lexer, int is_interactive);",
  "",
  "/* Get whether 'yy_lexer' will regard itself as being at the beginning",
  " * of a line (BOL), which is where \"^\" patterns can match. */",
  "int yy_get_bol(yy_lexer_t const *yy_lexer);",
  "",
  "/* Set the BOL condition. */",
  "void yy_set_bol(yy_lexer_t *yy_lexer, int at_bol);",
  "",
  "/* -------- Available methods for function pointers -------- */",
  "/* Use Standard C 'fread()'.  This assumes that 'yy_input_stream'",
  " * is a 'FILE*'. */",
  "int yy_read_input_with_fread(yy_lexer_t *yy_lexer,",
  "  void *dest, int size);",
  "",
  "%if use_read",
  "/* Use POSIX 'read()'.  This assumes that 'yy_input_stream' is a",
  " * 'FILE*'.  It uses 'fileno' to get the file descriptor. */",
  "int yy_read_input_with_read(yy_lexer_t *yy_lexer,",
  "  void *dest, int size);",
  "",
  "%endif",
  "/* Use Standard C 'fwrite()'.  This assumes that 'yy_output_stream'",
  " * is a 'FILE*'.*/",
  "int yy_write_output_with_fwrite(yy_lexer_t *yy_lexer,",
  "  void const *dest, int size);",
  "",
  "/* Return 1, meaning no more files. */",
  "int yy_wrap_return_1(yy_lexer_t *yy_lexer);",
  "",
  "/* Print an error message to stderr and exit. */",
  "void yy_error_print_and_exit(yy_lexer_t const *yy_lexer,",
  "  yy_error_code_t code, char const *detail);",
  "",
  "/* -------- Diagnostics -------- */",
  "/* Return the code of the error the scanner encountered, or",
  " * 'yy_err_no_error' if none has been.  This can be used in situations",
  " * where 'yy_error_function' returns in order to signal to the client",
  " * code that it must stop calling scanner functions. */",
  "yy_error_code_t yy_get_error(yy_lexer_t const *yy_lexer);",
  "",
  "/* Return an English string describing the given error 'code' as a",
  " * pointer to statically-allocated memory. */",
  "char const *yy_error_string(yy_error_code_t code);",
  "",
  "/* Fail an assertion if there are any objects allocated, across all",
  " * scanner instances, that have not been freed.  This can be called",
  " * after destroying all instances to check for memory leaks. */",
  "void yy_check_for_memory_leaks(void);",
  "",
  "%if !cpp_interface",
  "#ifdef __cplusplus",
  "}",
  "#endif",
  "%endif",
  "",
  "",
  "%if cpp_interface",
  "/* ---------------------- C++ interface ----------------------- */",
  "/* Indicate that the C++ interface class is available. */",
  "#define yyFlexLexer_CLASS_DEFINED",
  "",
  "",
  "/* A declaration qualifier that says the method does not throw. */",
  "#ifndef YY_NOEXCEPT",
  "#  if __cplusplus >= 201103L",
  "#    define YY_NOEXCEPT noexcept",
  "#  else",
  "#    define YY_NOEXCEPT throw()",
  "#  endif",
  "#endif",
  "",
  "",
  "/* Wrapper class that provides a more convenient interface for C++.",
  " *",
  " * If \"%option yyclass\" is used, then this is a base class for a derived",
  " * class defined by the user.  The derived class can have additional",
  " * data members that the scanner actions can use, and only it (not the",
  " * base class) should be instantiated.",
  " *",
  " * All member names begin with \"yym_\" to avoid collisions with members",
  " * the user might define in their derived class.",
  " */",
  "class yyFlexLexer {",
  "public:      /* types */",
  "  /* Exception thrown in response to 'yy_error_function'.",
  "   *",
  "   * Inheritance is virtual to account for the possibility that someone",
  "   * will multiply-inherit exceptions to combine categories. */",
  "  class LexError : virtual public std::exception {",
  "  private:     /* data */",
  "    /* This contains the string returned from 'what()' after that",
  "     * method has been called.  Its only purpose is to ensure the",
  "     * 'what()' string gets deallocated. */",
  "    mutable char *m_what;",
  "",
  "  public:      /* data */",
  "    /* Error code indicating the nature of the error. */",
  "    yy_error_code_t m_code;",
  "",
  "    /* Nullable pointer to static storage containing a human-readable",
  "     * additional detail string. */",
  "    char const *m_detail;",
  "",
  "  private:     /* methods */",
  "    void clearWhat() YY_NOEXCEPT;",
  "",
  "  public:      /* methods */",
  "    LexError(yy_error_code_t code, char const *detail) YY_NOEXCEPT;",
  "    LexError(LexError const &obj) YY_NOEXCEPT;",
  "    virtual ~LexError() YY_NOEXCEPT;",
  "",
  "    LexError& operator=(LexError const &obj) YY_NOEXCEPT;",
  "    virtual char const *what() const YY_NOEXCEPT;",
  "  };",
  "",
  "public:      /* methods */",
  "  /* If 'arg_yyin' is specified, it becomes the input stream;",
  "   * otherwise 'cin' (standard input) is used.  Similarly, if",
  "   * 'arg_yyout' is specified, it becomes the output stream; otherwise",
  "   * 'cout' is used. */",
  "  explicit yyFlexLexer(yy_input_stream_t *arg_yyin = NULL,",
  "                       yy_output_stream_t *arg_yyout = NULL);",
  "",
  "  virtual ~yyFlexLexer();",
  "",
  "%if option_yyclass",
  "  /* There is no 'yym_lex' method in this class.  Instead, the user of",
  "   * 'smflex' must declare it in their derived class, option_yyclass_name. */",
  "%else",
  "  /* Get the next token.  Return 0 on EOF. */",
  "  int yym_lex();",
  "%endif",
  "",
  "  /* Note: 'yym_lexer_state.yy_client_data' is *not* exposed.  That is",
  "   * because that field is used by the C++ wrapper class.  If you want",
  "   * to associate client data with the wrapper class, create a derived",
  "   * class.",
  "   *",
  "   * We also do not expose the function pointers because the intended",
  "   * extension mechanism for C++ is to override virtual functions. */",
  "",
  "  /* The following methods provide read-only access to the corresponding",
  "   * members of 'yy_lexer_t'.  As these correspond to direct field",
  "   * reads in the C API, the names do not contain \"get\", in order to",
  "   * keep the two sets of names as similar as possible. */",
  "  char const *yym_text() const",
  "    { return yym_lexer_state.yy_text; }",
  "  int yym_leng() const",
  "    { return yym_lexer_state.yy_leng; }",
  "  yy_input_stream_t *yym_input_stream() const",
  "    { return yym_lexer_state.yy_input_stream; }",
  "  yy_output_stream_t *yym_output_stream() const",
  "    { return yym_lexer_state.yy_output_stream; }",
  "%if do_yylineno",
  "  int yym_lineno() const",
  "    { return yym_lexer_state.yy_lineno; }",
  "%endif",
  "%if option_debug",
  "  bool yym_flex_debug() const",
  "    { return !!yym_lexer_state.yy_flex_debug; }",
  "%endif",
  "  yy_buffer_state_t *yym_current_buffer() const",
  "    { return yym_lexer_state.yy_current_buffer; }",
  "",
  "  /* Setters for 'yy_lexer_t' writeable members.  These names contain",
  "   * \"set\", even though the corresponding getters do not use \"get\". */",
  "  void yym_set_output_stream(yy_output_stream_t *new_out)",
  "    { yym_lexer_state.yy_output_stream = new_out; }",
  "%if option_debug",
  "  void yym_set_flex_debug(bool flag)",
  "    { yym_lexer_state.yy_flex_debug = flag; }",
  "%endif",
  "",
  "  /* The following \"yym_\" methods are all counterparts for the",
  "   * corresponding \"yy_\" global functions that comprise the C interface.",
  "   * See comments on their declarations, above, for details on what they",
  "   * do (and also the descriptions in the manual).  The only difference",
  "   * here is that the 'yy_lexer_t*' parameter is omitted, as it is",
  "   * implicitly carried by the object the method is invoked upon. */",
  "  void yym_restart(yy_input_stream_t *s);",
  "  yy_buffer_state_t *yym_create_buffer(yy_input_stream_t *file, int size=0);",
  "  void yym_switch_to_buffer(yy_buffer_state_t *new_buffer);",
  "  void yym_delete_buffer(yy_buffer_state_t *b);",
  "  void yym_flush_buffer(yy_buffer_state_t *b);",
  "%if option_yy_scan_string",
  "  yy_buffer_state_t *yym_scan_string(const char *yy_str);",
  "%endif",
  "%if option_yy_scan_bytes",
  "  yy_buffer_state_t *yym_scan_bytes(const char *bytes, int len);",
  "%endif",
  "%if option_yy_scan_buffer",
  "  yy_buffer_state_t *yym_scan_buffer(char *base, size_t size);",
  "%endif",
  "  void yym_set_start_state(int state);",
  "  int yym_get_start_state() const;",
  "%if option_stack",
  "  void yym_push_state(int new_state);",
  "  void yym_pop_state();",
  "%if option_yy_top_state",
  "  int yym_top_state() const;",
  "%endif",
  "%endif",
  "%if option_yy_read_character",
  "  int yym_read_character();",
  "%endif",
  "%if option_yy_unread_character",
  "  void yym_unread_character(int c);",
  "%endif",
  "  void yym_set_interactive(int is_interactive);",
  "  bool yym_get_bol() const;",
  "  void yym_set_bol(bool at_bol);",
  "",
  "  /* Provide static method versions of the functions that do not accept",
  "   * a 'yy_lexer_t*' so the entire C API is wrapped. */",
  "  static void yym_check_for_memory_leaks();",
  "",
  "  /* Read up to 'size' bytes into 'dest', returning the number of bytes",
  "   * read, 0 for EOF, and -1 for error.",
  "   *",
  "   * The default implementation reads from 'yym_input_stream()',",
  "   * treating it as 'std::istream*'. */",
  "  virtual int yym_read_input(void *dest, int size);",
  "",
  "  /* Write 'size' bytes from 'dest' to the output stream.",
  "   *",
  "   * The default implementation writes to 'yym_output_stream()',",
  "   * treating it as 'std::ostream*'. */",
  "  virtual int yym_write_output(void const *dest, int size);",
  "",
  "  /* This is called by 'yym_lex()' on EOF to possibly supply new input.",
  "   * It must either set up a new input source and return 0, or else",
  "   * return 1, the latter meaning there is no more input.  The default",
  "   * implementation returns 1. */",
  "  virtual int yym_wrap();",
  "",
  "  /* Called when an error occurs.  The default behavior is to throw",
  "   * LexError. */",
  "  virtual void yym_error(yy_error_code_t code, char const *detail) const;",
  "",
  "",
  "  /* ---------------------------------------------------------- */",
  "  /* The class interface below this point is still in a somewhat",
  "   * haphazard state. */",
  "",
  "",
  "  /* Switch to new input/output streams.  A NULL stream pointer",
  "   * indicates \"keep the current one\".",
  "   *",
  "   * The scanner core does *not* delete these objects; the client is",
  "   * responsible for deallocation if needed. */",
  "  /* TODO: I am in the process of removing this. */",
  "  void yym_switch_streams(yy_input_stream_t *new_in);",
  "",
  "protected:   /* data */",
  "  /* Underlying lexer state, around which this class is a wrapper.",
  "   * According to the principles of this API, this member should be",
  "   * private.  I am making it protected only as a hedge in case I have",
  "   * omitted some required functionality. */",
  "  yy_lexer_t yym_lexer_state;",
  "};",
  "%endif",
  "",
  "#endif /* yy_header_include_guard_name */",
  0
};
