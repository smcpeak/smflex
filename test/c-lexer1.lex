/* c-lexer1.lex */
/* Copied and modified from elkhound/c/lexer1.lex, which carries
 * this copyright:

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

/* In the context of smflex, this file is being used as a performance
 * test.  The actions have been cut down to not do very much so that
 * the scanner engine is the dominant infuence on speed. */

/******************/
/* C declarations */
/******************/

%{


#include "perftest.h"                  /* runPerftest */


/* type of each L1 token */
enum Lexer1TokenType {
  L1_IDENTIFIER,           /* 0 */
  L1_INT_LITERAL,          /* 1 */
  L1_FLOAT_LITERAL,        /* 2 */
  L1_STRING_LITERAL,       /* 3 */
  L1_UDEF_QUAL,            /* 4 */     /* dsw: user-defined qualifier */
  L1_CHAR_LITERAL,         /* 5 */
  L1_OPERATOR,             /* 6 */
  L1_PREPROCESSOR,         /* 7 */
  L1_WHITESPACE,           /* 8 */
  L1_COMMENT,              /* 9 */
  L1_ILLEGAL,             /* 10 */
  NUM_L1_TOKENS
};

/* Record a token that has been seen. */
static void lexer_emit(enum Lexer1TokenType, char const *text, int len);

/* Record a token fragment. */
static void collector_append(char const *text, int len);

#define COLLECTOR yytext, yyleng

%}

/****************/
/* flex options */
/****************/

%option noyywrap
%option nounput


/***************/
/* sub-regexps */
/***************/

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

/* sequence of decimal digits */
DIGITS        ({DIGIT}+)

/* sign of a number */
SIGN          ("+"|"-")

/* integer suffix */
/* added 'LL' option for GNU long long compatibility.. */
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


/********************/
/* start conditions */
/********************/

%x ST_C_COMMENT
%x ST_STRING


/**************************/
/* token definition rules */
/**************************/
%%

  /* identifier: e.g. foo */
{LETTER}{ALNUM}* {
  lexer_emit(L1_IDENTIFIER, yytext, yyleng);
}

  /* integer literal; dec, oct, or hex */
[1-9][0-9]*{INT_SUFFIX}?           |
[0][0-7]*{INT_SUFFIX}?             |
[0][xX][0-9A-Fa-f]+{INT_SUFFIX}?   {
  lexer_emit(L1_INT_LITERAL, yytext, yyleng);
}

  /* floating literal */
{DIGITS}"."{DIGITS}?([eE]{SIGN}?{DIGITS})?{FLOAT_SUFFIX}?   |
{DIGITS}"."?([eE]{SIGN}?{DIGITS})?{FLOAT_SUFFIX}?	    |
"."{DIGITS}([eE]{SIGN}?{DIGITS})?{FLOAT_SUFFIX}?	    {
  lexer_emit(L1_FLOAT_LITERAL, yytext, yyleng);
}

  /* ----- string literal ------- */
  /* intial */
"L"?{QUOTE}   {
  collector_append(yytext, yyleng);
  BEGIN(ST_STRING);
}

  /* continuation */
<ST_STRING>({STRCHAR}|{ESCAPE})*   {
  collector_append(yytext, yyleng);
}

  /* final */
<ST_STRING>{QUOTE} {
  collector_append(yytext, yyleng);
  lexer_emit(L1_STRING_LITERAL, COLLECTOR);
  BEGIN(INITIAL);
}

  /* dsw: user-defined qualifier; example: $tainted */
\${ALNUM}+ {
  lexer_emit(L1_UDEF_QUAL, yytext, yyleng);
}

  /* final, error */
<ST_STRING>{EOL}   |
<ST_STRING><<EOF>> {
  if (yytext[0] == '\n') {
    collector_append(yytext, yyleng);
  }
  else {
    /* when matching <<EOF>>, yytext[0]=0 and yyleng=1 (possibly
     * a bug in flex; its man page doesn't specify what it does), so we
     * get an extra NUL in the collected token, which I don't want */
  }

  //lexer.error("unterminated string literal");
  lexer_emit(L1_STRING_LITERAL, COLLECTOR);
  BEGIN(INITIAL);

  if (yytext[0] != '\n') {
    yyterminate();     	  /* flex man page says to do this for <<EOF>> */
  }
}


  /* character literal */
"L"?{TICK}({CCCHAR}|{ESCAPE})*{TICK}   {
  lexer_emit(L1_CHAR_LITERAL, yytext, yyleng);
}


  /* operator */
  /* extensions for theorem prover: "==>" */
"("|")"|"["|"]"|"->"|"::"|"."|"!"|"~"|"+"|"-"|"++"|"--"|"&"|"*"  |
".*"|"->*"|"/"|"%"|"<<"|">>"|"<"|"<="|">"|">="     	         |
"=="|"!="|"^"|"|"|"&&"|"||"|"?"|":"|"="|"*="|"/="|"%="|"+="      |
"-="|"&="|"^="|"|="|"<<="|">>="|","|"..."|";"|"{"|"}"|"==>"      {
  lexer_emit(L1_OPERATOR, yytext, yyleng);
}

  /* preprocessor */
  /* technically, if this isn't at the start of a line (possibly after
   * some whitespace, it should be an error.. I'm not sure right now how
   * I want to deal with that (I originally was using '^', but that
   * interacts badly with the whitespace rule) */
"#"{PPCHAR}*({BACKSL}{NL}{PPCHAR}*)*   {
  lexer_emit(L1_PREPROCESSOR, yytext, yyleng);
}

  /* whitespace */
  /* 10/20/02: added '\r' to accomodate files coming from Windows */
[ \t\n\f\v\r]+  {
  lexer_emit(L1_WHITESPACE, yytext, yyleng);
}

  /* C++ comment */
  /* we don't match the \n because that way this works at EOF */
"//"{NOTNL}*    {
  lexer_emit(L1_COMMENT, yytext, yyleng);
}

  /* ------- C comment --------- */
  /* initial */
"/""*"     {
  collector_append(yytext, yyleng);
  BEGIN(ST_C_COMMENT);
}

  /* continuation */
<ST_C_COMMENT>([^*]|"*"[^/])*   {
  collector_append(yytext, yyleng);
}

  /* final */
<ST_C_COMMENT>"*/"     {
  collector_append(yytext, yyleng);
  lexer_emit(L1_COMMENT, COLLECTOR);
  BEGIN(INITIAL);
}

  /* final, error */
<ST_C_COMMENT><<EOF>>     {
  //lexer.error("unterminated /**/ comment");
  lexer_emit(L1_COMMENT, COLLECTOR);
  BEGIN(INITIAL);
}

  /* illegal */
.  {
  lexer_emit(L1_ILLEGAL, yytext, yyleng);
}


%%
/**************/
/* extra code */
/**************/


static int tokenCount[NUM_L1_TOKENS] = {0};

static char lastTokenText[5];
static int lastTokenLen = 0;

static char lastCollectedText[5];
static int lastCollectedLen = 0;


static void lexer_emit(enum Lexer1TokenType type, char const *text, int len)
{
  tokenCount[type]++;

  /* Copy some text and then length just the prevent the compiler from *
   * optimizing them away completely. */
  my_strncpy(lastTokenText, text, sizeof(lastTokenText));
  lastTokenLen = len;
}


static void collector_append(char const *text, int len)
{
  my_strncpy(lastCollectedText, text, sizeof(lastCollectedText));
  lastCollectedLen = len;
}


int main(int argc, char **argv)
{
  int iters = runPerftest(argc, argv);

  if (iters > 1) {
    /* Abbreviated stats, to reduce clutter. */
    printf("identifiers: %d\n", tokenCount[L1_IDENTIFIER]);
  }
  else {
    /* Print these globals, again to prevent them from being optimized
     * away. */
    printf("last token text: \"%.*s\"\n", lastTokenLen, lastTokenText);
    printf("last collected text: \"%.*s\"\n", lastCollectedLen, lastCollectedText);

    /* Print token count stats. */
    for (int i=0; i < NUM_L1_TOKENS; i++) {
      printf("type=%d count=%d\n", i, tokenCount[i]);
    }
  }

  return 0;
}


/* EOF */
