/* elsa-lexer.lex
 * C/C++ lexer from Elsa.
 *
 * This file is copied from cc.lex in Elsa, which carries this license:

  The software in this directory is
  Copyright (c) 2002, Regents of the University of California
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

      * Redistributions in binary form must reproduce the above
        copyright notice, this list of conditions and the following
        disclaimer in the documentation and/or other materials provided
        with the distribution.

      * Neither the name of the University of California, Berkeley nor
        the names of its contributors may be used to endorse or promote
        products derived from this software without specific prior
        written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* In smflex, this file is used primarily for performance testing.
 * The actions from the original cc.lex have been replaced with stubs
 * that mimic the real lexer but focus on scanner engine execution. */


/* ----------------------- C definitions ---------------------- */
%{

#include "perftest.h"                  /* runPerftest */

/* This set of token types comes from cc_tokens.h in Elsa. */
enum TokenType {
  TOK_EOF,

  /* non-keyword name */
  TOK_NAME,

  /* classified name (for e.g. cdecl2) */
  TOK_TYPE_NAME,
  TOK_VARIABLE_NAME,

  /* literals */
  TOK_INT_LITERAL,
  TOK_FLOAT_LITERAL,
  TOK_STRING_LITERAL,
  TOK_CHAR_LITERAL,

  /* keywords */
  TOK_ASM,
  TOK_AUTO,
  TOK_BREAK,
  TOK_BOOL,
  TOK_CASE,
  TOK_CATCH,
  TOK_CDECL,
  TOK_CHAR,
  TOK_CLASS,
  TOK_CONST,
  TOK_CONST_CAST,
  TOK_CONTINUE,
  TOK_DEFAULT,
  TOK_DELETE,
  TOK_DO,
  TOK_DOUBLE,
  TOK_DYNAMIC_CAST,
  TOK_ELSE,
  TOK_ENUM,
  TOK_EXPLICIT,
  TOK_EXPORT,
  TOK_EXTERN,
  TOK_FALSE,
  TOK_FLOAT,
  TOK_FOR,
  TOK_FRIEND,
  TOK_GOTO,
  TOK_IF,
  TOK_INLINE,
  TOK_INT,
  TOK_LONG,
  TOK_MUTABLE,
  TOK_NAMESPACE,
  TOK_NEW,
  TOK_OPERATOR,
  TOK_PASCAL,
  TOK_PRIVATE,
  TOK_PROTECTED,
  TOK_PUBLIC,
  TOK_REGISTER,
  TOK_REINTERPRET_CAST,
  TOK_RETURN,
  TOK_SHORT,
  TOK_SIGNED,
  TOK_SIZEOF,
  TOK_STATIC,
  TOK_STATIC_CAST,
  TOK_STRUCT,
  TOK_SWITCH,
  TOK_TEMPLATE,
  TOK_THIS,
  TOK_THROW,
  TOK_TRUE,
  TOK_TRY,
  TOK_TYPEDEF,
  TOK_TYPEID,
  TOK_TYPENAME,
  TOK_UNION,
  TOK_UNSIGNED,
  TOK_USING,
  TOK_VIRTUAL,
  TOK_VOID,
  TOK_VOLATILE,
  TOK_WCHAR_T,
  TOK_WHILE,

  /* operators (I don't identify C++ operators because in C they're not identifiers) */
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LBRACKET,
  TOK_RBRACKET,
  TOK_ARROW,
  TOK_COLONCOLON,
  TOK_DOT,
  TOK_BANG,
  TOK_TILDE,
  TOK_PLUS,
  TOK_MINUS,
  TOK_PLUSPLUS,
  TOK_MINUSMINUS,
  TOK_AND,
  TOK_STAR,
  TOK_DOTSTAR,
  TOK_ARROWSTAR,
  TOK_SLASH,
  TOK_PERCENT,
  TOK_LEFTSHIFT,
  TOK_RIGHTSHIFT,
  TOK_LESSTHAN,
  TOK_LESSEQ,
  TOK_GREATERTHAN,
  TOK_GREATEREQ,
  TOK_EQUALEQUAL,
  TOK_NOTEQUAL,
  TOK_XOR,
  TOK_OR,
  TOK_ANDAND,
  TOK_OROR,
  TOK_QUESTION,
  TOK_COLON,
  TOK_EQUAL,
  TOK_STAREQUAL,
  TOK_SLASHEQUAL,
  TOK_PERCENTEQUAL,
  TOK_PLUSEQUAL,
  TOK_MINUSEQUAL,
  TOK_ANDEQUAL,
  TOK_XOREQUAL,
  TOK_OREQUAL,
  TOK_LEFTSHIFTEQUAL,
  TOK_RIGHTSHIFTEQUAL,
  TOK_COMMA,
  TOK_ELLIPSIS,
  TOK_SEMICOLON,
  TOK_LBRACE,
  TOK_RBRACE,

  /* dummy terminals used for precedence games */
  TOK_PREFER_REDUCE,
  TOK_PREFER_SHIFT,

  /* GNU extensions */
  TOK_BUILTIN_CONSTANT_P,
  TOK___ALIGNOF__,
  TOK___OFFSETOF__,
  TOK___BUILTIN_OFFSETOF,
  TOK___ATTRIBUTE__,
  TOK___FUNCTION__,
  TOK___LABEL__,
  TOK___PRETTY_FUNCTION__,
  TOK___TYPEOF__,
  TOK___EXTENSION__,
  TOK___BUILTIN_EXPECT,

  /* varargs; dsw: I think that we should make all of these their own
   * AST node, I just don't want to deal with the parsing ambiguity
   * with E_funCall right now */
  TOK___BUILTIN_VA_ARG,

  TOK_MIN_OP,
  TOK_MAX_OP,
  TOK_REAL,
  TOK_IMAG,

  /* sm: these are C99 but I'm putting them here anyway.. */
  TOK_RESTRICT,
  TOK_COMPLEX,
  TOK_IMAGINARY,
  NUM_TOKEN_TYPES

};  /* enum TokenType */


/* Functions invoked by actions. */
static enum TokenType tokFunc(enum TokenType t, char const *text, int leng);
static enum TokenType svalTokFunc(enum TokenType t, char const *text, int leng);
static enum TokenType alternateKeyword_tokFunc(enum TokenType t, char const *text, int leng);
static void err(char const *);
static void whitespace();

#define tok(t) tokFunc(t, YY_TEXT, YY_LENG)
#define svalTok(t) svalTokFunc(t, YY_TEXT, YY_LENG)
#define alternateKeyword_tok(t) alternateKeyword_tokFunc(t, YY_TEXT, YY_LENG)


%}


/* -------------------- flex options ------------------ */
/* don't use the default-echo rules */
%option nodefault

/* start conditions */
%x BUGGY_STRING_LIT

/* ------------------- definitions -------------------- */
/* newline */
NL            "\n"

/* anything but newline */
NOTNL         .

/* any of 256 source characters */
ANY           ({NOTNL}|{NL})

/* backslash */
BACKSL        "\\"

/* beginnging of line (must be start of a pattern) */
BOL           ^

/* end of line (would like EOF to qualify also, but flex doesn't allow it */
EOL           {NL}

/* letter or underscore */
LETTER        [A-Za-z_]

/* letter or underscore or digit */
ALNUM         [A-Za-z_0-9]

/* decimal digit */
DIGIT         [0-9]
HEXDIGIT      [0-9A-Fa-f]

/* sequence of decimal digits */
DIGITS        ({DIGIT}+)
/* sequence of hex digits */
HEXDIGITS     ({HEXDIGIT}+)

/* sign of a number */
SIGN          ("+"|"-")

/* integer suffix */
/* added 'LL' option for gcc/c99 long long compatibility */
ELL_SUFFIX    [lL]([lL]?)
INT_SUFFIX    ([uU]{ELL_SUFFIX}?|{ELL_SUFFIX}[uU]?)

/* floating-point suffix letter */
FLOAT_SUFFIX  [flFL]

/* normal string character: any but quote, newline, or backslash */
STRCHAR       [^\"\n\\]

/* (start of) an escape sequence */
ESCAPE        ({BACKSL}{ANY})

/* double quote */
QUOTE         [\"]

/* normal character literal character: any but single-quote, newline, or backslash */
CCCHAR        [^\'\n\\]

/* single quote */
TICK          [\']

/* space or tab */
SPTAB         [ \t]

/* preprocessor "character" -- any but escaped newline */
PPCHAR        ([^\\\n]|{BACKSL}{NOTNL})


/* ------------- token definition rules --------------- */
%%

  /* this comment is replaced, by an external script, with whatever
   * additional rules a language extension author desires */
  /* EXTENSION RULES GO HERE */


  /* operators, punctuators and keywords: tokens with one spelling */
"asm"              return tok(TOK_ASM);
"auto"             return tok(TOK_AUTO);
"break"            return tok(TOK_BREAK);
"bool"             return tok(TOK_BOOL);
"case"             return tok(TOK_CASE);
"catch"            return tok(TOK_CATCH);
"cdecl"            return tok(TOK_CDECL);
"char"             return tok(TOK_CHAR);
"class"            return tok(TOK_CLASS);
"const"            return tok(TOK_CONST);
"const_cast"       return tok(TOK_CONST_CAST);
"continue"         return tok(TOK_CONTINUE);
"default"          return tok(TOK_DEFAULT);
"delete"           return tok(TOK_DELETE);
"do"               return tok(TOK_DO);
"double"           return tok(TOK_DOUBLE);
"dynamic_cast"     return tok(TOK_DYNAMIC_CAST);
"else"             return tok(TOK_ELSE);
"enum"             return tok(TOK_ENUM);
"explicit"         return tok(TOK_EXPLICIT);
"export"           return tok(TOK_EXPORT);
"extern"           return tok(TOK_EXTERN);
"false"            return tok(TOK_FALSE);
"float"            return tok(TOK_FLOAT);
"for"              return tok(TOK_FOR);
"friend"           return tok(TOK_FRIEND);
"goto"             return tok(TOK_GOTO);
"if"               return tok(TOK_IF);
"inline"           return tok(TOK_INLINE);
"int"              return tok(TOK_INT);
"long"             return tok(TOK_LONG);
"mutable"          return tok(TOK_MUTABLE);
"namespace"        return tok(TOK_NAMESPACE);
"new"              return tok(TOK_NEW);
"operator"         return tok(TOK_OPERATOR);
"pascal"           return tok(TOK_PASCAL);
"private"          return tok(TOK_PRIVATE);
"protected"        return tok(TOK_PROTECTED);
"public"           return tok(TOK_PUBLIC);
"register"         return tok(TOK_REGISTER);
"reinterpret_cast" return tok(TOK_REINTERPRET_CAST);
"return"           return tok(TOK_RETURN);
"short"            return tok(TOK_SHORT);
"signed"           return tok(TOK_SIGNED);
"sizeof"           return tok(TOK_SIZEOF);
"static"           return tok(TOK_STATIC);
"static_cast"      return tok(TOK_STATIC_CAST);
"struct"           return tok(TOK_STRUCT);
"switch"           return tok(TOK_SWITCH);
"template"         return tok(TOK_TEMPLATE);
"this"             return tok(TOK_THIS);
"throw"            return tok(TOK_THROW);
"true"             return tok(TOK_TRUE);
"try"              return tok(TOK_TRY);
"typedef"          return tok(TOK_TYPEDEF);
"typeid"           return tok(TOK_TYPEID);
"typename"         return tok(TOK_TYPENAME);
"union"            return tok(TOK_UNION);
"unsigned"         return tok(TOK_UNSIGNED);
"using"            return tok(TOK_USING);
"virtual"          return tok(TOK_VIRTUAL);
"void"             return tok(TOK_VOID);
"volatile"         return tok(TOK_VOLATILE);
"wchar_t"          return tok(TOK_WCHAR_T);
"while"            return tok(TOK_WHILE);

"("                return tok(TOK_LPAREN);
")"                return tok(TOK_RPAREN);
"["                return tok(TOK_LBRACKET);
"]"                return tok(TOK_RBRACKET);
"->"               return tok(TOK_ARROW);
"::"               return tok(TOK_COLONCOLON);
"."                return tok(TOK_DOT);
"!"                return tok(TOK_BANG);
"~"                return tok(TOK_TILDE);
"+"                return tok(TOK_PLUS);
"-"                return tok(TOK_MINUS);
"++"               return tok(TOK_PLUSPLUS);
"--"               return tok(TOK_MINUSMINUS);
"&"                return tok(TOK_AND);
"*"                return tok(TOK_STAR);
".*"               return tok(TOK_DOTSTAR);
"->*"              return tok(TOK_ARROWSTAR);
"/"                return tok(TOK_SLASH);
"%"                return tok(TOK_PERCENT);
"<<"               return tok(TOK_LEFTSHIFT);
">>"               return tok(TOK_RIGHTSHIFT);
"<"                return tok(TOK_LESSTHAN);
"<="               return tok(TOK_LESSEQ);
">"                return tok(TOK_GREATERTHAN);
">="               return tok(TOK_GREATEREQ);
"=="               return tok(TOK_EQUALEQUAL);
"!="               return tok(TOK_NOTEQUAL);
"^"                return tok(TOK_XOR);
"|"                return tok(TOK_OR);
"&&"               return tok(TOK_ANDAND);
"||"               return tok(TOK_OROR);
"?"                return tok(TOK_QUESTION);
":"                return tok(TOK_COLON);
"="                return tok(TOK_EQUAL);
"*="               return tok(TOK_STAREQUAL);
"/="               return tok(TOK_SLASHEQUAL);
"%="               return tok(TOK_PERCENTEQUAL);
"+="               return tok(TOK_PLUSEQUAL);
"-="               return tok(TOK_MINUSEQUAL);
"&="               return tok(TOK_ANDEQUAL);
"^="               return tok(TOK_XOREQUAL);
"|="               return tok(TOK_OREQUAL);
"<<="              return tok(TOK_LEFTSHIFTEQUAL);
">>="              return tok(TOK_RIGHTSHIFTEQUAL);
","                return tok(TOK_COMMA);
"..."              return tok(TOK_ELLIPSIS);
";"                return tok(TOK_SEMICOLON);
"{"                return tok(TOK_LBRACE);
"}"                return tok(TOK_RBRACE);

  /* "alternative tokens" of cppstd 2.5p2 */
"<%"               return alternateKeyword_tok(TOK_LBRACE);
"%>"               return alternateKeyword_tok(TOK_RBRACE);
"<:"               return alternateKeyword_tok(TOK_LBRACKET);
":>"               return alternateKeyword_tok(TOK_RBRACKET);
  /* "%:" and "%:%:" correspond to "#" and "##", which are only for
   * the preprocessor, so I will ignore them here */
"and"              return alternateKeyword_tok(TOK_ANDAND);
"bitor"            return alternateKeyword_tok(TOK_OR);
"or"               return alternateKeyword_tok(TOK_OROR);
"xor"              return alternateKeyword_tok(TOK_XOR);
"compl"            return alternateKeyword_tok(TOK_TILDE);
"bitand"           return alternateKeyword_tok(TOK_AND);
"and_eq"           return alternateKeyword_tok(TOK_ANDEQUAL);
"or_eq"            return alternateKeyword_tok(TOK_OREQUAL);
"xor_eq"           return alternateKeyword_tok(TOK_XOREQUAL);
"not"              return alternateKeyword_tok(TOK_BANG);
"not_eq"           return alternateKeyword_tok(TOK_NOTEQUAL);

  /* this rule is to avoid backing up in the lexer
   * when there are two dots but not three */
".." {
  YY_LESS_TEXT(1);     /* put back all but 1; this is inexpensive */
  return tok(TOK_DOT);
}

  /* identifier: e.g. foo */
{LETTER}{ALNUM}* {
  return svalTok(TOK_NAME);
}

  /* integer literal; dec, oct, or hex */
[1-9][0-9]*{INT_SUFFIX}?           |
[0][0-7]*{INT_SUFFIX}?             |
[0][xX][0-9A-Fa-f]+{INT_SUFFIX}?   {
  return svalTok(TOK_INT_LITERAL);
}

  /* hex literal with nothing after the 'x' */
[0][xX] {
  err("hexadecimal literal with nothing after the 'x'");
  return svalTok(TOK_INT_LITERAL);
}

  /* floating literal */
{DIGITS}"."{DIGITS}?([eE]{SIGN}?{DIGITS})?{FLOAT_SUFFIX}?   |
{DIGITS}"."?([eE]{SIGN}?{DIGITS})?{FLOAT_SUFFIX}?	    |
"."{DIGITS}([eE]{SIGN}?{DIGITS})?{FLOAT_SUFFIX}?	    {
  return svalTok(TOK_FLOAT_LITERAL);
}

  /* floating literal with no digits after the 'e' */
{DIGITS}"."{DIGITS}?[eE]{SIGN}?   |
{DIGITS}"."?[eE]{SIGN}?           |
"."{DIGITS}[eE]{SIGN}?            {
  err("floating literal with no digits after the 'e'");

  /* in recovery rules like this it's best to yield the best-guess
   * token type, instead of some TOK_ERROR, so the parser can still
   * try to make sense of the input; having reported the error is
   * sufficient to ensure that later stages won't try to interpret
   * the lexical text of this token as a floating literal */
  return svalTok(TOK_FLOAT_LITERAL);
}

  /* string literal */
"L"?{QUOTE}({STRCHAR}|{ESCAPE})*{QUOTE} {
  return svalTok(TOK_STRING_LITERAL);
}

  /* string literal missing final quote */
"L"?{QUOTE}({STRCHAR}|{ESCAPE})*{EOL}   {
  YY_SET_START_STATE(BUGGY_STRING_LIT);
  return svalTok(TOK_STRING_LITERAL);
}

  /* unterminated string literal; maximal munch causes us to prefer
   * either of the above two rules when possible; the trailing
   * optional backslash is needed so the scanner won't back up in that
   * case; NOTE: this can only happen if the file ends in the string
   * and there is no newline before the EOF */
"L"?{QUOTE}({STRCHAR}|{ESCAPE})*{BACKSL}? {
  err("unterminated string literal");
  YY_TERMINATE();
}


  /* This scanner reads in a string literal that contains unescaped
   * newlines, to support a gcc-2 bug.  The strategy is to emit a
   * sequence of TOK_STRING_LITERALs, as if the string had been
   * properly broken into multiple literals.  However, these literals
   * aren't consistently surrounded by quotes... */
<BUGGY_STRING_LIT>{
  ({STRCHAR}|{ESCAPE})*{QUOTE} {
    /* found the end */
    YY_SET_START_STATE(INITIAL);
    return svalTok(TOK_STRING_LITERAL);
  }
  ({STRCHAR}|{ESCAPE})*{EOL} {
    /* another line */
    return svalTok(TOK_STRING_LITERAL);
  }
  <<EOF>> |
  ({STRCHAR}|{ESCAPE})*{BACKSL}? {
    /* unterminated (this only matches at EOF) */
    err("at EOF, unterminated string literal; support for newlines in string "
        "literals is presently turned on, maybe the missing quote should have "
        "been much earlier in the file?");
    YY_TERMINATE();
  }
}


  /* character literal */
"L"?{TICK}({CCCHAR}|{ESCAPE})*{TICK}   {
  return svalTok(TOK_CHAR_LITERAL);
}

  /* character literal missing final tick */
"L"?{TICK}({CCCHAR}|{ESCAPE})*{EOL}    {
  err("character literal missing final \"'\"");
  return svalTok(TOK_CHAR_LITERAL);       /* error recovery */
}

  /* unterminated character literal */
"L"?{TICK}({CCCHAR}|{ESCAPE})*{BACKSL}?  {
  err("unterminated character literal");
  YY_TERMINATE();
}


  /* sm: I moved the user-defined qualifier rule into qual_ext.lex
   * in the oink tree */


  /* preprocessor */
  /* technically, if this isn't at the start of a line (possibly after
   * some whitespace), it should be an error.. I'm not sure right now how
   * I want to deal with that (I originally was using '^', but that
   * interacts badly with the whitespace rule) */

  /* #line directive: the word "line" is optional, then a space, and
   * then we accept the rest of the line; 'parseHashLine' will finish
   * parsing the directive */
"#"("line"?){SPTAB}.*{NL} {
  whitespace();       /* don't increment line count until after parseHashLine() */
}

  /* other preprocessing: ignore it */
  /* trailing optional baskslash to avoid backing up */
"#"{PPCHAR}*({BACKSL}{NL}{PPCHAR}*)*{BACKSL}?   {
  /* treat it like whitespace, ignoring it otherwise */
  whitespace();
}

  /* whitespace */
  /* 10/20/02: added '\r' to accomodate files coming from Windows; this
   * could be seen as part of the mapping from physical source file
   * characters to the basic character set (cppstd 2.1 para 1 phase 1),
   * except that it doesn't happen for chars in string/char literals... */
[ \t\n\f\v\r]+  {
  whitespace();
}

  /* C++ comment */
  /* we don't match the \n because that way this works at EOF */
"//"{NOTNL}*    {
  whitespace();
}

  /* C comment */
"/""*"([^*]|"*"*[^*/])*"*"+"/"     {
  /* the pattern is a little complicated because the naive one */
  whitespace();
}

  /* unterminated C comment */
"/""*"([^*]|"*"*[^*/])*"*"*        {
  err("unterminated /""*...*""/ comment");
  YY_TERMINATE();
}


  /* illegal */
.  {
  err("illegal character");
}

<<EOF>> {
  YY_TERMINATE();
}


%%
/**************/
/* extra code */
/**************/


static int tokenCount[NUM_TOKEN_TYPES] = {0};

static char lastTokenText[5];
static int lastTokenLen = 0;

static char lastMsg[80];

static int numErrors = 0;
static int numWhitespaces = 0;

static enum TokenType tokFunc(enum TokenType t, char const *text, int leng)
{
  tokenCount[t]++;
  my_strncpy(lastTokenText, text, sizeof(lastTokenText));
  lastTokenLen = leng;
  return t;
}

static enum TokenType svalTokFunc(enum TokenType t, char const *text, int leng)
{
  return tokFunc(t, text, leng);
}

static enum TokenType alternateKeyword_tokFunc(enum TokenType t,
  char const *text, int leng)
{
  return tokFunc(t, text, leng);
}

static void err(char const *msg)
{
  my_strncpy(lastMsg, msg, sizeof(lastMsg));
  numErrors++;
}

static void whitespace()
{
  numWhitespaces++;
}


int main(int argc, char **argv)
{
  int iters = runPerftest(argc, argv);

  if (iters > 1) {
    /* Abbreviated stats, to reduce clutter. */
    printf("identifiers: %d\n", tokenCount[TOK_NAME]);
  }
  else {
    int i;

    /* Print these globals, again to prevent them from being optimized
     * away. */
    printf("last token text: \"%.*s\"\n", lastTokenLen, lastTokenText);

    /* Print token count stats. */
    for (i=0; i < NUM_TOKEN_TYPES; i++) {
      int count = tokenCount[i];
      if (count > 0) {
        printf("type=%d count=%d\n", i, count);
      }
    }
  }

  return 0;
}


/* EOF */
