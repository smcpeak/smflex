/* input-scan.lex - scanner for smflex input */

%{
/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Vern Paxson.
 *
 * The United States Government has rights in this work pursuant
 * to contract no. DE-AC03-76SF00098 between the United States
 * Department of Energy and the University of California.
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
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "input-scan.h"                /* this module */

#include "input-parse.h"               /* synerr, format_synerr, yylval */
#include "input-parse.y.h"             /* token codes */
#include "main.h"                      /* sectnum, nmstr */
#include "misc.h"                      /* add_action, myctoi, etc. */
#include "sym.h"                       /* ndinstal, ccllookup, cclinstal */

#include <string.h>                    /* strcpy, strlen, etc. */


#define ACTION_ECHO add_action(yytext)
#define ACTION_IFDEF(def, should_define) \
  { \
    if (should_define) \
      action_define(def, 1); \
  }

/* Add a newline to the current action text.  This is done when we
 * consume a line of source text but do not otherwise emit anything,
 * since we need to stay synchronized with the #line scope.
 *
 * In general, every time we increment 'linenum', we must either echo
 * the newline we matched, call ADD_ACTION_NL(), or emit a new #line
 * directive. */
#define ADD_ACTION_NL() add_action("\n")

#define MARK_END_OF_PROLOG mark_prolog();

#define RETURNCHAR \
  yylval = (unsigned char) yytext[0]; \
  return CHAR;

#define RETURNNAME \
  strcpy( nmstr, yytext ); \
  return NAME;

#define PUT_BACK_STRING(str, start) \
  for (i = strlen(str) - 1; i >= start; --i) \
    unput((str)[i])

#define CHECK_REJECT(str) \
  if (all_upper(str)) \
    reject_used = true;

#define CHECK_YYMORE(str) \
  if (all_lower(str)) \
    yymore_used = true;
%}

%option caseless nodefault outfile="input-scan.lex.c" stack noyy_top_state
%option nostdinit yywrap
%option prefix="input_scan"

%x SECT2 SECT2PROLOG SECT3 CODEBLOCK PICKUPDEF SC CARETISBOL NUM QUOTE
%x FIRSTCCL CCL ACTION RECOVER COMMENT ACTION_STRING PERCENT_BRACE_ACTION
%x OPTION LINEDIR

WS              [[:blank:]]+
OPTWS           [[:blank:]]*
NOT_WS          [^[:blank:]\n]

NL              \r?\n

NAME            ([[:alpha:]_][[:alnum:]_-]*)
NOT_NAME        [^[:alpha:]_*\n]+

SCNAME          {NAME}

ESCSEQ          (\\([^\n]|[0-7]{1,3}|x[[:xdigit:]]{1,2}))

FIRST_CCL_CHAR  ([^\\\n]|{ESCSEQ})
CCL_CHAR        ([^\\\n\]]|{ESCSEQ})
CCL_EXPR        ("[:"[[:alpha:]]+":]")

LEXOPT          [aceknopr]

%%
  static int bracelevel, didadef, indented_code;
  static int doing_rule_action = false;
  static int option_sense;
  static int doing_start_conditions = false;

  int doing_codeblock = false;
  int i;
  Char nmdef[MAXLINE], myesc();


<INITIAL>{
        ^{WS}           indented_code = true; BEGIN(CODEBLOCK);
        ^"/*"           ACTION_ECHO; yy_push_state(yy_lexer, COMMENT);
        ^#{OPTWS}line{WS}       yy_push_state(yy_lexer, LINEDIR);
        ^"%s"{NAME}?    {
                          /* About to emit SC #defines, which must be
                           * in output file context. */
                          add_action(yy_output_file_line_directive);
                          doing_start_conditions = true;
                          return SCDECL;
                        }
        ^"%x"{NAME}?    {
                          add_action(yy_output_file_line_directive);
                          doing_start_conditions = true;
                          return XSCDECL;
                        }
        ^"%{".*{NL}     {
                          ++ linenum;
                          line_directive_out_src();
                          indented_code = false;
                          BEGIN(CODEBLOCK);
                        }

        {WS}            /* discard */

        ^"%%".*         {
                          sectnum = 2;
                          bracelevel = 0;
                          mark_defs1();
                          line_directive_out_src();
                          BEGIN(SECT2PROLOG);
                          return SECTEND;
                        }

        ^"%option"      {
                          /* We could emit lots or nothing while
                           * processing options.  It has to be treated
                           * as non-source. */
                          add_action(yy_output_file_line_directive);
                          BEGIN(OPTION);
                          return OPTION_OP;
                        }

        ^"%"{LEXOPT}{OPTWS}[[:digit:]]*{OPTWS}{NL}    ++linenum; ADD_ACTION_NL(); /* ignore */
        ^"%"{LEXOPT}{WS}.*{NL}                        ++linenum; ADD_ACTION_NL(); /* ignore */

        ^"%"[^sxaceknopr{}].*   synerr( _( "unrecognized '%' directive" ) );

        ^{NAME}         {
                          strcpy(nmstr, yytext);
                          didadef = false;
                          BEGIN(PICKUPDEF);
                        }

        {SCNAME}        RETURNNAME;
        {OPTWS}{NL}     {
                          ++linenum;
                          if (doing_start_conditions) {
                            /* We switched to output #line scope when we
                             * started processing these.  Return to
                             * input file scope. */
                            line_directive_out_src();
                            doing_start_conditions = false;
                          }
                          else {
                            ACTION_ECHO;
                          }
                        }
}


<COMMENT>{
        "*/"            ACTION_ECHO; yy_pop_state(yy_lexer);
        "*"             ACTION_ECHO;
        [^*\n]+         ACTION_ECHO;
        [^*\n]*{NL}     ++linenum; ACTION_ECHO;
}

<LINEDIR>{
        \n              yy_pop_state(yy_lexer);
        [[:digit:]]+    linenum = myctoi( yytext );

        \"[^"\n]*\"     {
                          flex_free((void *) infilename);
                          infilename = copy_string(yytext + 1);
                          infilename[strlen(infilename) - 1] = '\0';
                        }
        .               /* ignore spurious characters */
}

<CODEBLOCK>{
        ^"%}".*{NL}     {
                          ++linenum;
                          ADD_ACTION_NL();
                          BEGIN(INITIAL);
                        }

        {NAME}|{NOT_NAME}|.     ACTION_ECHO;

        {NL}            {
                          ++linenum;
                          ACTION_ECHO;
                          if (indented_code)
                            BEGIN(INITIAL);
                        }
}


<PICKUPDEF>{
        {WS}            /* separates name and definition */

        {NOT_WS}.*      {
                          strcpy((char *) nmdef, yytext);

                          /* Skip trailing whitespace. */
                          for (i = strlen((char *) nmdef) - 1;
                               i >= 0 && (nmdef[i] == ' ' || nmdef[i] == '\t'); --i);

                          nmdef[i + 1] = '\0';

                          ndinstal(nmstr, nmdef);
                          didadef = true;
                        }

        {NL}            {
                          if (!didadef)
                            synerr(_("incomplete name definition"));
                          BEGIN(INITIAL);
                          ++linenum;
                          ADD_ACTION_NL();
                        }
}


<OPTION>{
        {NL}            {
                          ++linenum;

                          /* Return to source context after processing
                           * a line of options. */
                          line_directive_out_src();
                          BEGIN(INITIAL);
                        }
        {WS}            option_sense = true;

        "="             return '=';

        no              option_sense = ! option_sense;

        7bit            csize = option_sense ? 128 : 256;
        8bit            csize = option_sense ? 256 : 128;

        align           long_align = option_sense;
        backup          backing_up_report = option_sense;
        batch           interactive = ! option_sense;
        "c++"           cpp_interface = option_sense;
        caseful|case-sensitive          caseins = ! option_sense;
        caseless|case-insensitive       caseins = option_sense;
        debug           option_debug = option_sense;
        default         spprdflt = ! option_sense;
        ecs             useecs = option_sense;
        fast            synerr(_("\"fast\" has been renamed to \"jacobson\""));
        full            {
                          useecs = usemecs = false;
                          fulltbl = true;
                        }
        input           ACTION_IFDEF("YY_NO_INPUT", ! option_sense);
        interactive     interactive = option_sense;
        jacobson        {
                          useecs = usemecs = false;
                          jacobson = true;
                        }
        main            {
                          action_define("YY_MAIN", option_sense);
                        }
        meta-ecs        usemecs = option_sense;
        perf-report     performance_report += option_sense ? 1 : -1;
        read            use_read = option_sense;
        reject          reject_really_used = option_sense;
        stack           option_stack = option_sense;
        stdinit         do_stdinit = option_sense;
        unput           ACTION_IFDEF("YY_NO_UNPUT", ! option_sense);
        verbose         printstats = option_sense;
        warn            nowarn = ! option_sense;
        yylineno        do_yylineno = option_sense;
        yymore          yymore_really_used = option_sense;
        yywrap          do_yywrap = option_sense;

        yy_push_state   ACTION_IFDEF("YY_NO_PUSH_STATE", !option_sense);
        yy_pop_state    ACTION_IFDEF("YY_NO_POP_STATE", !option_sense);
        yy_top_state    ACTION_IFDEF("YY_NO_TOP_STATE", !option_sense);

        yy_scan_buffer  ACTION_IFDEF("YY_NO_SCAN_BUFFER", !option_sense);
        yy_scan_bytes   ACTION_IFDEF("YY_NO_SCAN_BYTES", !option_sense);
        yy_scan_string  ACTION_IFDEF("YY_NO_SCAN_STRING", !option_sense);

        outfile         return OPT_OUTFILE;
        prefix          return OPT_PREFIX;
        yyclass         return OPT_YYCLASS;

        \"[^"\n]*\"     {
                          strcpy(nmstr, yytext + 1);
                          nmstr[strlen(nmstr) - 1] = '\0';
                          return NAME;
                        }

        (([a-mo-z]|n[a-np-z])[[:alpha:]\-+]*)|. {
                          format_synerr(_("unrecognized %%option: %s"), yytext);
                          BEGIN(RECOVER);
                        }
}

<RECOVER>.*{NL}         ++linenum; ADD_ACTION_NL(); BEGIN(INITIAL);


<SECT2PROLOG>{
        ^"%{".* ++bracelevel; yyless( 2 );      /* eat only %{ */
        ^"%}".* --bracelevel; yyless( 2 );      /* eat only %} */

        ^{WS}.* ACTION_ECHO;    /* indented code in prolog */

        ^{NOT_WS}.*     {       /* non-indented code */
                          if (bracelevel <= 0) {   /* not in %{ ... %} */
                            yyless(0);             /* put it all back */
                            yy_set_bol(yy_lexer, 1);
                            mark_prolog();
                            BEGIN(SECT2);
                          }
                          else
                            ACTION_ECHO;
                        }

        .*              ACTION_ECHO;
        {NL}            ++linenum; ACTION_ECHO;

        <<EOF>>         {
                          mark_prolog();
                          sectnum = 0;
                          yyterminate(); /* to stop the parser */
                        }
}

<SECT2>{
        ^{OPTWS}{NL}    {       /* allow blank lines in section 2 */
                          ++linenum;
                          ADD_ACTION_NL();
                        }

        ^{OPTWS}"%{"    {
                          indented_code = false;
                          doing_codeblock = true;
                          bracelevel = 1;
                          BEGIN(PERCENT_BRACE_ACTION);
                        }

        ^{OPTWS}"<"     BEGIN(SC); return '<';
        ^{OPTWS}"^"     return '^';
        \"              BEGIN(QUOTE); return '"';
        "{"/[[:digit:]] BEGIN(NUM); return '{';
        "$"/([[:blank:]]|{NL})  return '$';

        {WS}"%{"        {
                          bracelevel = 1;
                          BEGIN(PERCENT_BRACE_ACTION);

                          if (in_rule) {
                            doing_rule_action = true;
                            in_rule = false;
                            return '\n';
                          }
                        }
        {WS}"|".*{NL}   {
                          /* There is a problem with #line numbers and
                           * continued actions, at least when the
                           * pattern is <<EOF>>, because we emit a line
                           * directive pointing at the source file
                           * before we realize the action is continued,
                           * and hence will not have any action code.
                           * So, emit another #line directive pointing
                           * back at the output file to cancel the
                           * effect of the first one.
                           *
                           * Test case: test/continued-action2.lex */
                          add_action(yy_output_file_line_directive);

                          continued_action = true;
                          ++linenum;
                          return '\n';
                        }

        ^{WS}"/*"       {
                          yyless(yyleng - 2); /* put back '/', '*' */
                          bracelevel = 0;
                          continued_action = false;
                          BEGIN(ACTION);
                        }

        ^{WS}           /* allow indented rules */

        {WS}            {
                          /* This rule is separate from the one below because
                           * otherwise we get variable trailing context, so
                           * we can't build the scanner using -C{f,J}.
                           */
                          bracelevel = 0;
                          continued_action = false;
                          BEGIN(ACTION);

                          if (in_rule) {
                            doing_rule_action = true;
                            in_rule = false;
                            return '\n';
                          }
                        }

        {OPTWS}{NL}     {
                          bracelevel = 0;
                          continued_action = false;
                          BEGIN(ACTION);
                          unput('\n');      /* so <ACTION> sees it */

                          if (in_rule) {
                            doing_rule_action = true;
                            in_rule = false;
                            return '\n';
                          }
                        }

        ^{OPTWS}"<<EOF>>"       |
        "<<EOF>>"       return EOF_OP;

        ^"%%".*         {
                          sectnum = 3;
                          BEGIN(SECT3);
                          yyterminate(); /* to stop the parser */
                        }

        "["({FIRST_CCL_CHAR}|{CCL_EXPR})({CCL_CHAR}|{CCL_EXPR})*        {
                          int cclval;

                          strcpy(nmstr, yytext);

                          /* Check to see if we've already encountered this
                           * ccl.
                           */
                          if ((cclval = ccllookup((Char *) nmstr)) != 0) {
                            if (yyinput() != ']')
                              synerr(_("bad character class"));

                            yylval = cclval;
                            ++cclreuse;
                            return PREVCCL;
                          }
                          else {
                            /* We fudge a bit.  We know that this ccl will
                             * soon be numbered as lastccl + 1 by cclinit.
                             */
                            cclinstal((Char *) nmstr, lastccl + 1);

                            /* Push back everything but the leading bracket
                             * so the ccl can be rescanned.
                             */
                            yyless(1);

                            BEGIN(FIRSTCCL);
                            return '[';
                          }
                        }

        "{"{NAME}"}"    {
                          Char *nmdefptr;
                          Char *ndlookup();

                          strcpy(nmstr, yytext + 1);
                          nmstr[yyleng - 2] = '\0'; /* chop trailing brace */

                          if ((nmdefptr = ndlookup(nmstr)) == 0)
                            format_synerr(_("undefined definition {%s}"), nmstr);

                          else {
                            /* push back name surrounded by ()'s */
                            int len = strlen((char *) nmdefptr);

                            /* don't use ()'s after all */
                            if (nmdefptr[0] == '^' ||
                                (len > 0 && nmdefptr[len - 1] == '$')) {
                              PUT_BACK_STRING((char *) nmdefptr, 0);

                              if (nmdefptr[0] == '^')
                                BEGIN(CARETISBOL);
                            }

                            else {
                              unput(')');
                              PUT_BACK_STRING((char *) nmdefptr, 0);
                              unput('(');
                            }
                          }
                        }

        [/|*+?.(){}]    return (unsigned char) yytext[0];
        .               RETURNCHAR;
}


<SC>{
        [,*]            return (unsigned char) yytext[0];
        ">"             BEGIN(SECT2); return '>';
        ">"/^           BEGIN(CARETISBOL); return '>';
        {SCNAME}        RETURNNAME;
        .               {
                          format_synerr(_("bad <start condition>: %s"), yytext);
                        }
}

<CARETISBOL>"^"         BEGIN(SECT2); return '^';


<QUOTE>{
        [^"\n]          RETURNCHAR;
        \"              BEGIN(SECT2); return '"';

        {NL}            {
                          synerr(_("missing quote"));
                          BEGIN(SECT2);
                          ++linenum;
                          ADD_ACTION_NL();
                          return '"';
                        }
}


<FIRSTCCL>{
        "^"/[^-\]\n]    BEGIN(CCL); return '^';
        "^"/("-"|"]")   return '^';
        .               BEGIN(CCL); RETURNCHAR;
}

<CCL>{
        -/[^\]\n]       return '-';
        [^\]\n]         RETURNCHAR;
        "]"             BEGIN(SECT2); return ']';
        .|{NL}          {
                          synerr(_("bad character class"));
                          BEGIN(SECT2);
                          return ']';
                        }
}

<FIRSTCCL,CCL>{
        "[:alnum:]"     BEGIN(CCL); return CCE_ALNUM;
        "[:alpha:]"     BEGIN(CCL); return CCE_ALPHA;
        "[:blank:]"     BEGIN(CCL); return CCE_BLANK;
        "[:cntrl:]"     BEGIN(CCL); return CCE_CNTRL;
        "[:digit:]"     BEGIN(CCL); return CCE_DIGIT;
        "[:graph:]"     BEGIN(CCL); return CCE_GRAPH;
        "[:lower:]"     BEGIN(CCL); return CCE_LOWER;
        "[:print:]"     BEGIN(CCL); return CCE_PRINT;
        "[:punct:]"     BEGIN(CCL); return CCE_PUNCT;
        "[:space:]"     BEGIN(CCL); return CCE_SPACE;
        "[:upper:]"     BEGIN(CCL); return CCE_UPPER;
        "[:xdigit:]"    BEGIN(CCL); return CCE_XDIGIT;
        {CCL_EXPR}      {
                          format_synerr(_("bad character class expression: %s"),
                                        yytext);
                          BEGIN(CCL);
                          return CCE_ALNUM;
                        }
}

<NUM>{
        [[:digit:]]+    {
                          yylval = myctoi(yytext);
                          return NUMBER;
                        }

        ","             return ',';
        "}"             BEGIN(SECT2); return '}';

        .               {
                          synerr(_("bad character inside {}'s"));
                          BEGIN(SECT2);
                          return '}';
                        }

        {NL}            {
                          synerr(_("missing }"));
                          BEGIN(SECT2);
                          ++linenum;
                          ADD_ACTION_NL();
                          return '}';
                        }
}


<PERCENT_BRACE_ACTION>{
        {OPTWS}"%}".*           bracelevel = 0;

        <ACTION>"/*"            ACTION_ECHO; yy_push_state(yy_lexer, COMMENT);

        <CODEBLOCK,ACTION>{
                "reject"        {
                          ACTION_ECHO;
                          CHECK_REJECT(yytext);
                        }
                "yymore"        {
                          ACTION_ECHO;
                          CHECK_YYMORE(yytext);
                        }
        }

        {NAME}|{NOT_NAME}|.     ACTION_ECHO;
        {NL}            {
                          ++ linenum;
                          ACTION_ECHO;
                          if (bracelevel == 0 ||
                              (doing_codeblock && indented_code)) {
                            if (doing_rule_action) {
                              add_action(yy_output_file_line_directive);
                              add_action("  YY_BREAK\n");
                            }

                            doing_rule_action = doing_codeblock = false;
                            BEGIN(SECT2);
                          }
                        }
}


        /* The ACTION state is where the lexer accumulates the text of
         * an action in memory, via ACTION_ECHO.  During this process,
         * nothing is reported to the parser.  Once the end of the
         * action is found, we return to the SECT2 state, at which
         * point we'll resume informing the parser of what we find. */

        /* Reject and YYmore() are checked for above, in PERCENT_BRACE_ACTION */
<ACTION>{
        "{"             ACTION_ECHO; ++bracelevel;
        "}"             ACTION_ECHO; --bracelevel;
        [^[:alpha:]_{}"'/\n]+   ACTION_ECHO;
        {NAME}          ACTION_ECHO;
        "'"([^'\\\n]|\\.)*"'"   ACTION_ECHO; /* character constant */
        \"              ACTION_ECHO; BEGIN(ACTION_STRING);
        {NL}            {
                          ++ linenum;
                          ACTION_ECHO;
                          if (bracelevel == 0) {
                            if (doing_rule_action) {
                              add_action(yy_output_file_line_directive);
                              add_action("  YY_BREAK\n");
                            }

                            doing_rule_action = false;
                            BEGIN(SECT2);
                          }
                        }
        .               ACTION_ECHO;
}

<ACTION_STRING>{
        [^"\\\n]+       ACTION_ECHO;
        \\.             ACTION_ECHO;
        {NL}            ++linenum; ACTION_ECHO;
        \"              ACTION_ECHO; BEGIN(ACTION);
        .               ACTION_ECHO;
}

<COMMENT,ACTION,ACTION_STRING><<EOF>>   {
                          synerr(_("EOF encountered inside an action"));
                          yyterminate();
                        }


<SECT2,QUOTE,FIRSTCCL,CCL>{ESCSEQ}      {
                          yylval = myesc((Char *) yytext);

                          if (YY_START == FIRSTCCL)
                            BEGIN(CCL);

                          return CHAR;
                        }


<SECT3>{
        .*(\n?)         out(yytext);   /* Copy section 3 to output. */
        <<EOF>>         sectnum = 0; yyterminate();
}

<*>.|\n                 format_synerr(_("bad character: %s"), yytext);

%%


input_scan_lexer_t input_lexer;


/* Wrapup a file in the lexical analyzer and possibly move on to
 * the next file. */
int input_scan_wrap(input_scan_lexer_t *lexer)
{
  if (--num_input_files > 0) {
    set_input_file(*++input_files);
    return 0;
  }

  else
    return 1;
}


/* set_input_file - open the given file (if NULL, stdin) for scanning */
void set_input_file(char *file)
{
  if (file && strcmp(file, "-")) {
    infilename = copy_string(file);
    input_lexer.yy_input_stream = fopen(infilename, "r");

    if (input_lexer.yy_input_stream == NULL)
      lerrsf(_("can't open %s"), file);
  }

  else {
    input_lexer.yy_input_stream = stdin;
    infilename = copy_string("<stdin>");
  }

  linenum = 1;
}


int flexscan()
{
  return input_scan_lex(&input_lexer);
}


/* Wrapper routines for accessing the scanner's malloc routines. */
void *flex_alloc(size_t size)
{
  return (void *) malloc(size);
}

void *flex_realloc(void *ptr, size_t size)
{
  return (void *) realloc(ptr, size);
}

void flex_free(void *ptr)
{
  if (ptr)
    free(ptr);
}
