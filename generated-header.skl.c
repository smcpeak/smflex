/* File created from generated-header.skl via encode.sh */

/* NULL-terminated array of NUL-terminated lines, *without* line
 * terminators. */
const char *header_skl_contents[] = {
  "%# generated-header.skl: Skeleton of generated header.",
  "",
  "/* Copyright (c) 1993 The Regents of the University of California.",
  " * All rights reserved.",
  " *",
  " * This code is derived from software contributed to Berkeley by",
  " * Kent Williams and Tom Epperly.",
  " *",
  " * 2021-04-30: It has (much later) been heavily modified by",
  " * Scott McPeak.",
  " *",
  " * Redistribution and use in source and binary forms, with or without modification, are permitted provided",
  " * that: (1) source distributions retain this entire copyright notice and",
  " * comment, and (2) distributions including binaries display the following",
  " * acknowledgement:  ``This product includes software developed by the",
  " * University of California, Berkeley and its contributors'' in the",
  " * documentation or other materials provided with the distribution and in",
  " * all advertising materials mentioning features or use of this software.",
  " * Neither the name of the University nor the names of its contributors may",
  " * be used to endorse or promote products derived from this software without",
  " * specific prior written permission.",
  " * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED",
  " * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF",
  " * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. */",
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
  "%# In the skeleton file, we use YY_INPUT_STREAM_TYPE and",
  "%# YY_OUTPUT_STREAM_TYPE.  These are substituted for std::istream,",
  "%# std::ostream, or FILE, as appropriate, during generation.",
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
  "typedef struct yy_buffer_state_struct yy_buffer_state;",
  "%if jacobson",
  "typedef struct yy_trans_info_struct yy_trans_info;",
  "%endif",
  "",
  "/* This is the type of 'yy_current_state' (among other things), which",
  " * is the current state within the finite state automaton that is",
  " * responsible for recognizing tokens. */",
  "%if jacobson",
  "typedef yy_trans_info const *yy_state_type;",
  "%else",
  "typedef int yy_state_type;",
  "%endif",
  "",
  "%if cpp_interface",
  "class yyFlexLexer;           /* Defined below. */",
  "%endif",
  "",
  "/* This struct encapsulates the scanner state. */",
  "struct yy_lexer_state_struct {",
  "%if !cpp_interface",
  "  /* Application-specific data.  The client of this interface is free",
  "   * to use this value however they want.  'yy_construct' sets it to",
  "   * NULL. */",
  "  void *yy_client_data;",
  "%else",
  "  /* Pointer to the C++ wrapper object. */",
  "  yyFlexLexer *yy_wrapper_object;",
  "%endif",
  "",
  "  /* After a rule pattern has been matched, this is set to point at the",
  "   * matched text within 'yy_current_buffer', temporarily",
  "   * NUL-terminated.",
  "   *",
  "   * NOTE: Within section 2 actions, there is also a 'yytext' (without",
  "   * an underscore) macro that expands to 'yy_lexer->yy_text'. */",
  "  char *yy_text;",
  "",
  "  /* After a rule pattern has been matched, this is set to the length",
  "   * of the matched text, in bytes.",
  "   *",
  "   * NOTE: As with 'yy_text', this field has an associated 'yyleng'",
  "   * macro active in section 2 actions. */",
  "  int yy_leng;",
  "",
  "%if do_yylineno",
  "  /* Line number within the currently scanned file.  TODO: Does this",
  "   * work after 'yy_restart', etc.? */",
  "  int yy_lineno;",
  "",
  "%endif",
  "%if option_debug",
  "  /* When true, print to stdout lines indicating which rules' actions",
  "   * are running.  Initially true. */",
  "  int yy_flex_debug;",
  "",
  "%endif",
  "  /* Input source for default YY_INPUT. */",
  "  YY_INPUT_STREAM_TYPE *yy_input_stream;",
  "",
  "  /* Output sink for default ECHO. */",
  "  YY_OUTPUT_STREAM_TYPE *yy_output_stream;",
  "",
  "  /* The input source we are currently reading from, and a buffer",
  "   * in front of it. */",
  "  yy_buffer_state *yy_current_buffer;",
  "",
  "  /* Holds the character overwritten by a NUL when 'yy_text' is formed. */",
  "  char yy_hold_char;",
  "",
  "  /* Number of characters read into yy_ch_buf. */",
  "  int yy_n_chars;",
  "",
  "  /* Points to current character in buffer. */",
  "  char *yy_c_buf_p;",
  "",
  "  /* True if we need to initialize. */",
  "  /* TODO: Can I remove this now that I have 'yy_construct'? */",
  "  int yy_init;",
  "",
  "  /* Current start state number. */",
  "  int yy_start;",
  "",
  "  /* Flag which is used to allow yy_wrap()'s to do buffer switches",
  "   * instead of setting up a fresh 'yy_input_stream'.",
  "   * A bit of a hack ... */",
  "  int yy_did_buffer_switch_on_eof;",
  "",
  "  /* The following are not always needed, but may be depending",
  "   * on use of certain smflex features (like REJECT or yymore()). */",
  "",
  "%if num_backing_up > 0 && !reject_used",
  "  /* These are related to backing up when REJECT is *not* used. */",
  "  yy_state_type yy_last_accepting_state;",
  "  char *yy_last_accepting_cpos;",
  "",
  "%endif",
  "%if reject_used",
  "  /* These are related to REJECT. */",
  "  yy_state_type *yy_state_buf;",
  "  yy_state_type *yy_state_ptr;",
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
  "  /* TODO: I do not think either of these is ever used. */",
  "  int yy_more_offset;",
  "  int yy_prev_more_offset;",
  "",
  "  /* TODO: Create a variable to track when \"%option stack\" is used,",
  "   * and only define these in that case. */",
  "  int yy_start_stack_ptr;",
  "  int yy_start_stack_depth;",
  "  int *yy_start_stack;",
  "};",
  "",
  "typedef struct yy_lexer_state_struct yy_lexer_t;",
  "",
  "",
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
  " * scanning 'input_file'. */",
  "void yy_restart(yy_lexer_t *yy_lexer, YY_INPUT_STREAM_TYPE *input_file);",
  "",
  "/* Pointer to an opaque type that represents an input source along",
  " * with a read buffer for that source.  See the manual section",
  " * \"Multiple Input Buffers\" for more details. */",
  "typedef yy_buffer_state *YY_BUFFER_STATE;",
  "",
  "/* Create a new buffer for use with 'yy_lexer' that reads from 'file'.",
  " * The 'size' is the size of the read buffer; 8192 is a reasonable",
  " * value to use. */",
  "/* TODO: The manual advises using YY_BUF_SIZE, but that value is not",
  " * currently exported. */",
  "YY_BUFFER_STATE yy_create_buffer(yy_lexer_t *yy_lexer, YY_INPUT_STREAM_TYPE *file,",
  "                                 int size);",
  "",
  "/* Make 'new_buffer' the active input source for 'yy_lexer'. */",
  "void yy_switch_to_buffer(yy_lexer_t *yy_lexer, YY_BUFFER_STATE new_buffer);",
  "",
  "/* Deallocate 'b' and release any resources associated with it. */",
  "void yy_delete_buffer(yy_lexer_t *yy_lexer, YY_BUFFER_STATE b);",
  "",
  "/* Discard any already-read data from the input source associated with",
  " * 'b'.  Future attempts to read will start by reading new data from",
  " * that source. */",
  "void yy_flush_buffer(yy_lexer_t *yy_lexer, YY_BUFFER_STATE b);",
  "",
  "/* Allocate a new buffer (which must be deallocated with",
  " * 'yy_delete_buffer') to scan the contents of NUL-terminated 'yy_str'.",
  " * Switch to that buffer. */",
  "YY_BUFFER_STATE yy_scan_string(yy_lexer_t *yy_lexer, const char *yy_str);",
  "",
  "/* Like 'yy_scan_string', but without the presumption of NUL",
  " * termination. */",
  "YY_BUFFER_STATE yy_scan_bytes(yy_lexer_t *yy_lexer, const char *bytes, int len);",
  "",
  "/* Whereas the preceding two functions make a copy of the source data,",
  " * this one scans it without making a copy.  The last two bytes *must*",
  " * be 0.  See the description in the manual. */",
  "YY_BUFFER_STATE yy_scan_buffer(yy_lexer_t *yy_lexer, char *base, size_t size);",
  "",
  "/* Set the start state of 'yy_lexer' to 'state'.  This function must be",
  " * used instead of BEGIN when not within a rule action. */",
  "void yy_begin(yy_lexer_t *yy_lexer, int state);",
  "",
  "/* Set the 'interactive' flag on the given 'yy_lexer'.  An interactive",
  " * yy_lexer reads its input one character at a time. */",
  "void yy_set_interactive(yy_lexer_t *yy_lexer, int is_interactive);",
  "",
  "/* Set whether 'yy_lexer' will regard itself as being at the beginning",
  " * of a line (BOL), which is where \"^\" patterns can match. */",
  "void yy_set_bol(yy_lexer_t *yy_lexer, int at_bol);",
  "",
  "%if do_yywrap",
  "%if cpp_interface",
  "/* This just delegates to the C++ class 'yym_wrap' method. */",
  "%else",
  "/* The 'smflex' user must define this function.  It can return 0 after",
  " * calling 'yy_restart' to begin processing another file, or return 1 to",
  " * indicate there are no more files to process. */",
  "%endif",
  "int yy_wrap(yy_lexer_t *yy_lexer);",
  "%endif",
  "",
  "%if !cpp_interface",
  "#ifdef __cplusplus",
  "}",
  "#endif",
  "%endif",
  "",
  "",
  "%if cpp_interface",
  "/* Indicate that the C++ interface is in use. */",
  "#define yyFlexLexer_CLASS_DEFINED",
  "",
  "",
  "/* Wrapper class that provides a more convenient interface for C++.",
  " *",
  " * Clients can retrieve one token at a time using 'yym_lex()' and",
  " * inspect the text of each token using 'yym_text()' and 'yym_leng()'.",
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
  "public:      /* methods */",
  "  /* arg_yyin and arg_yyout default to the cin and cout, but we",
  "   * only make that assignment when initializing in yy_lex(). */",
  "  yyFlexLexer(YY_INPUT_STREAM_TYPE *arg_yyin = NULL,",
  "              YY_OUTPUT_STREAM_TYPE *arg_yyout = NULL);",
  "",
  "  virtual ~yyFlexLexer();",
  "",
  "%if yyclass",
  "  /* There is no 'yym_lex' method in this class.  Instead, the user of",
  "   * 'smflex' must declare it in their derived class, yyclass_name. */",
  "%else",
  "  /* Get the next token.  Return 0 on EOF. */",
  "  int yym_lex();",
  "%endif",
  "",
  "  /* Switch to new input/output streams.  A NULL stream pointer",
  "   * indicates \"keep the current one\".",
  "   *",
  "   * The scanner core does *not* delete these objects; the client is",
  "   * responsible for deallocation if needed. */",
  "  /* TODO: I don't think this is a good interface. */",
  "  void yym_switch_streams(YY_INPUT_STREAM_TYPE *new_in = NULL,",
  "                          YY_OUTPUT_STREAM_TYPE *new_out = NULL);",
  "",
  "%if do_yywrap",
  "%if yyclass",
  "  /* The derived class, yyclass_name, must define this. */",
  "  virtual int yym_wrap() = 0;",
  "%else",
  "  /* The 'smflex' user must define this method.  It is called by",
  "   * 'yym_lex()' on EOF to possibly supply new input.  It must either call",
  "   * 'switch_streams' and return 0, or else return 1, the latter meaning",
  "   * there is no more input. */",
  "  virtual int yym_wrap();",
  "%endif",
  "%endif",
  "",
  "  /* Low-level buffer manipulation.  See manual for details. */",
  "  void yym_switch_to_buffer(yy_buffer_state *new_buffer);",
  "  YY_BUFFER_STATE yym_create_buffer(YY_INPUT_STREAM_TYPE *file, int size);",
  "  void yym_delete_buffer(YY_BUFFER_STATE b);",
  "  void yym_restart(YY_INPUT_STREAM_TYPE *s);",
  "",
  "  /* Last-matched token text (NUL-terminated) and its length. */",
  "  const char* yym_text()          { return yym_lexer_state.yy_text; }",
  "  int yym_leng()                  { return yym_lexer_state.yy_leng; }",
  "",
  "%if do_yylineno",
  "  int yym_lineno() const          { return yym_lexer_state.yy_lineno; }",
  "",
  "%endif",
  "%if option_debug",
  "  int yym_debug() const           { return yym_lexer_state.yy_flex_debug; }",
  "  void yym_set_debug( int flag )  { yym_lexer_state.yy_flex_debug = flag; }",
  "",
  "%endif",
  "  /* These methods may be overridden to provide an alternate source and",
  "   * sink.  The default implementations read and write",
  "   * 'yy_input_stream' and 'yy_output_stream' in 'yym_lexer_state'. */",
  "  virtual int yym_LexerInput(char *buf, int max_size);",
  "  virtual void yym_LexerOutput(const char *buf, int size);",
  "",
  "  /* This can be overridden to customize how errors are handled. */",
  "  virtual void yym_LexerError(const char *msg);",
  "",
  "protected:   /* methods */",
  "  /* TODO: Do I need all of these wrapper functions? */",
  "  void yym_flush_buffer(yy_buffer_state *b);",
  "",
  "  void yym_push_state(int new_state);",
  "  void yym_pop_state();",
  "  int yym_top_state();",
  "",
  "protected:   /* data */",
  "  /* Underlying lexer state, around which this class is a wrapper. */",
  "  yy_lexer_t yym_lexer_state;",
  "};",
  "%endif",
  "",
  "#endif /* yy_header_include_guard_name */",
  0
};
