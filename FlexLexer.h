// FlexLexer.h -- define interfaces for lexical analyzer classes generated
//                by flex

// Copyright (c) 1993 The Regents of the University of California.
// All rights reserved.
//
// This code is derived from software contributed to Berkeley by
// Kent Williams and Tom Epperly.
//
// 2021-04-30: It has (much later) been heavily modified by
// Scott McPeak.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided
// that: (1) source distributions retain this entire copyright notice and
// comment, and (2) distributions including binaries display the following
// acknowledgement:  ``This product includes software developed by the
// University of California, Berkeley and its contributors'' in the
// documentation or other materials provided with the distribution and in
// all advertising materials mentioning features or use of this software.
// Neither the name of the University nor the names of its contributors may
// be used to endorse or promote products derived from this software without
// specific prior written permission.
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef FLEX_LEXER_H
#define FLEX_LEXER_H

#include <iostream>                    // istream, ostream

// This class encapsulates the scanner state.  This primarily consists
// of knowing which tokens are candidates based on what has been seen
// so far, as well as the user-specified "start conditions".  It also
// has a pointer to 'yy_current_buffer', which buffers data from the
// actual input source.
//
// This class clients to retrieve one token at a time using 'yylex()'
// and inspect the text of each token using 'YYText()' and 'YYLeng()'.
//
// If "%option yyclass" is used, then this is a base class for a derived
// class defined by the user.  The derived class can have additional
// data members that the scanner actions can use, and only it (not the
// base class) should be instantiated.
//
class yyFlexLexer {
public:      // types
  typedef std::istream istream;
  typedef std::ostream ostream;

  // Details of the buffer state.  Defined in FlexLexer.h.
  struct yy_buffer_state_impl;

  // smcpeak: I believe this type represents a "state" in the finite
  // state automaton that is responsible for recognizing tokens.  It
  // is *not* the type used to represent "start conditions".
  //
  // TODO: I think that 'flex' can select a different type for this
  // when using the C interface, but evidently cannot for C++.  That
  // could affect table compression.  I should investigate if both
  // can be made to use the same mechanism.
  typedef int yy_state_type;

public:      // methods
  // arg_yyin and arg_yyout default to the cin and cout, but we
  // only make that assignment when initializing in yylex().
  yyFlexLexer(istream *arg_yyin = 0, ostream *arg_yyout = 0);

  virtual ~yyFlexLexer();

  // Get the next token.  Return 0 on EOF.
  int yylex();

  // Switch to new input/output streams.  A NULL stream pointer
  // indicates "keep the current one".
  void switch_streams(istream *new_in, ostream *new_out);

  // Low-level buffer manipulation.  See manual for details.
  void yy_switch_to_buffer(yy_buffer_state_impl *new_buffer);
  yy_buffer_state_impl *yy_create_buffer(istream *s, int size);
  void yy_delete_buffer(yy_buffer_state_impl *b);
  void yyrestart(istream *s);

  const char* YYText()            { return yytext; }
  int YYLeng()                    { return yyleng; }

  int lineno() const              { return yylineno; }

  int debug() const               { return yy_flex_debug; }
  void set_debug( int flag )      { yy_flex_debug = flag; }

protected:   // methods
  // As explained in the manual, these methods may be overridden by
  // a derived class to provide an alternate source and sink.
  virtual int LexerInput(char *buf, int max_size);
  virtual void LexerOutput(const char *buf, int size);

  // This can be overridden to customize how errors are handled.
  virtual void LexerError(const char *msg);

  void yyunput(int c, char *buf_ptr);
  int yyinput();

  void yy_load_buffer_state();
  void yy_init_buffer(yy_buffer_state_impl *b, istream *s);
  void yy_flush_buffer(yy_buffer_state_impl *b);

  int yy_start_stack_ptr;
  int yy_start_stack_depth;
  int *yy_start_stack;

  void yy_push_state(int new_state);
  void yy_pop_state();
  int yy_top_state();

  yy_state_type yy_get_previous_state();
  yy_state_type yy_try_NUL_trans(yy_state_type current_state);
  int yy_get_next_buffer();

protected:   // data
  char* yytext;
  int yyleng;
  int yylineno;           // only maintained if you use %option yylineno
  int yy_flex_debug;      // only has effect with -d or "%option debug"

  istream *yyin;  // input source for default LexerInput
  ostream *yyout; // output sink for default LexerOutput

  yy_buffer_state_impl *yy_current_buffer;

  // yy_hold_char holds the character lost when yytext is formed.
  char yy_hold_char;

  // Number of characters read into yy_ch_buf.
  int yy_n_chars;

  // Points to current character in buffer.
  char *yy_c_buf_p;

  int yy_init;            // whether we need to initialize
  int yy_start;           // start state number

  // Flag which is used to allow yywrap()'s to do buffer switches
  // instead of setting up a fresh yyin.  A bit of a hack ...
  int yy_did_buffer_switch_on_eof;

  // The following are not always needed, but may be depending
  // on use of certain flex features (like REJECT or yymore()).

  yy_state_type yy_last_accepting_state;
  char *yy_last_accepting_cpos;

  yy_state_type *yy_state_buf;
  yy_state_type *yy_state_ptr;

  char *yy_full_match;
  int *yy_full_state;
  int yy_full_lp;

  int yy_lp;
  int yy_looking_for_trail_begin;

  int yy_more_flag;
  int yy_more_len;
  int yy_more_offset;
  int yy_prev_more_offset;
};

#endif // FLEX_LEXER_H
