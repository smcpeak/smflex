/* input-parse.y - parser for smflex input */

%token CHAR NUMBER SECTEND SCDECL XSCDECL NAME PREVCCL EOF_OP
%token OPTION_OP OPT_OUTFILE OPT_PREFIX OPT_YYCLASS
%token OPT_YY_LEX_NAME
%token OPT_YY_LEX_PARAMETERS

%token CCE_ALNUM CCE_ALPHA CCE_BLANK CCE_CNTRL CCE_DIGIT CCE_GRAPH
%token CCE_LOWER CCE_PRINT CCE_PUNCT CCE_SPACE CCE_UPPER CCE_XDIGIT

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


#include "input-parse.h"               /* this module */

#include "ccl.h"                       /* cclinit, cclnegate, ccladd */
#include "ecs.h"                       /* mkeccl */
#include "flexchar.h"                  /* smflex_isascii */
#include "main.h"                      /* check_options */
#include "misc.h"                      /* add_action, copy_string, etc. */
#include "nfa.h"                       /* mkstate, mkbranch, etc. */
#include "sym.h"                       /* scinstal, sclookup */
#include "yylex.h"                     /* yylex */

#include <ctype.h>                     /* isalnum, isalpha, etc. */


int pat, scnum, eps, headcnt, trailcnt, anyccl, lastchar, i, rulelen;
int trlcontxt, xcluflg, currccl, cclsorted, varlength, variable_trail_rule;

int *scon_stk;
int scon_stk_ptr;

static int madeany = false;  /* whether we've made the '.' character class */
int previous_continued_action;  /* whether the previous rule's action was '|' */

/* Expand a POSIX character class expression. */
#define CCL_EXPR(func) \
  { \
  int c; \
  for ( c = 0; c < csize; ++c ) \
    if ( smflex_isascii(c) && func(c) ) \
      ccladd( currccl, c ); \
  }

/* While POSIX defines isblank(), it's not ANSI C. */
#define IS_BLANK(c) ((c) == ' ' || (c) == '\t')

/* On some over-ambitious machines, such as DEC Alpha's, the default
 * token type is "long" instead of "int"; this leads to problems with
 * declaring yylval in flexdef.h.  But so far, all the yacc's I've seen
 * wrap their definitions of YYSTYPE with "#ifndef YYSTYPE"'s, so the
 * following should ensure that the default token type is "int".
 */
#define YYSTYPE int

%}

%%
goal            :  sect1 sect1end sect2 initforrule
                        { /* add default rule */
                          int def_rule;

                          pat = cclinit();
                          cclnegate(pat);

                          def_rule = mkstate(-pat);

                          /* Remember the number of the default rule so we
                           * don't generate "can't match" warnings for it.
                           */
                          default_rule = num_rules;

                          finish_rule(def_rule, false, 0, 0);

                          for (i = 1; i <= lastsc; ++i)
                            scset[i] = mkbranch(scset[i], def_rule);

                          add_action(yy_output_file_line_directive);
                          if (option_suppress_default_rule) {
                            add_action("  YY_ERROR(yy_err_no_rule_matches, NULL /*detail*/);\n"
                                       "  return 0;\n");
                          }
                          else {
                            add_action("  YY_ECHO;\n"
                                       "  YY_BREAK\n");
                          }
                        }
                ;

sect1           :  sect1 startconddecl namelist1
                |  sect1 options
                |
                |  error
                        { synerr("unknown error processing section 1"); }
                ;

sect1end        :  SECTEND
                        {
                          check_options();
                          scon_stk = allocate_integer_array(lastsc + 1);
                          scon_stk_ptr = 0;
                        }
                ;

startconddecl   :  SCDECL
                        { xcluflg = false; }

                |  XSCDECL
                        { xcluflg = true; }
                ;

namelist1       :  namelist1 NAME
                        { scinstal(nmstr, xcluflg); }

                |  NAME
                        { scinstal(nmstr, xcluflg); }

                |  error
                        { synerr("bad start condition list"); }
                ;

options         :  OPTION_OP optionlist
                ;

optionlist      :  optionlist option
                |
                ;

/* The grammar process options that take arguments.  Other options
 * are handled directly in the lexer. */
option          :  OPT_OUTFILE '=' NAME
                        {
                          /* Only record the specified file name if we
                           * do not already have one.  This way, the -o
                           * command line option takes precedence. */
                          if (!did_outfilename) {
                            outfilename = copy_string(nmstr);
                            did_outfilename = 1;
                          }
                        }
                |  OPT_PREFIX '=' NAME
                        { set_prefix(copy_string(nmstr)); }
                |  OPT_YYCLASS '=' NAME
                        { option_yyclass = copy_string(nmstr); }
                |  OPT_YY_LEX_NAME '=' NAME
                        { option_yy_lex_name = copy_string(nmstr); }
                |  OPT_YY_LEX_PARAMETERS '=' NAME
                        { option_yy_lex_parameters = copy_string(nmstr); }
                ;

sect2           :  sect2 scon initforrule flexrule '\n'
                        { scon_stk_ptr = $2; }
                |  sect2 scon '{' sect2 '}'
                        { scon_stk_ptr = $2; }
                |
                ;

initforrule     :
                        {
                          /* Initialize for a parse of one rule. */
                          trlcontxt = variable_trail_rule = varlength = false;
                          trailcnt = headcnt = rulelen = 0;
                          current_state_type = STATE_NORMAL;
                          previous_continued_action = continued_action;
                          in_rule = true;

                          new_rule();
                        }
                ;

flexrule        :  '^' rule
                        {
                          pat = $2;
                          finish_rule(pat, variable_trail_rule, headcnt, trailcnt);

                          if (scon_stk_ptr > 0) {
                            for (i = 1; i <= scon_stk_ptr; ++i)
                              scbol[scon_stk[i]] = mkbranch(scbol[scon_stk[i]], pat);
                          }

                          else {
                            /* Add to all non-exclusive start conditions,
                             * including the default (0) start condition.
                             */
                            for (i = 1; i <= lastsc; ++i)
                              if (!scxclu[i])
                                scbol[i] = mkbranch(scbol[i], pat);
                          }

                          if (!bol_needed) {
                            bol_needed = true;

                            if (performance_report > 1)
                              pinpoint_message("'^' operator results in sub-optimal performance");
                          }
                        }

                |  rule
                        {
                          pat = $1;
                          finish_rule(pat, variable_trail_rule, headcnt, trailcnt);

                          if (scon_stk_ptr > 0) {
                            for (i = 1; i <= scon_stk_ptr; ++i)
                              scset[scon_stk[i]] = mkbranch(scset[scon_stk[i]], pat);
                          }

                          else {
                            for (i = 1; i <= lastsc; ++i)
                              if (!scxclu[i])
                                scset[i] = mkbranch(scset[i], pat);
                          }
                        }

                |  EOF_OP
                        {
                          if (scon_stk_ptr > 0)
                            build_eof_action();

                          else {
                            /* This EOF applies to all start conditions
                             * which don't already have EOF actions.
                             */
                            for (i = 1; i <= lastsc; ++i)
                              if (!sceof[i])
                                scon_stk[++scon_stk_ptr] = i;

                            if (scon_stk_ptr == 0)
                              warn("all start conditions already have <<EOF>> rules");

                            else
                              build_eof_action();
                          }
                        }

                |  error
                        { synerr("unrecognized rule"); }
                ;

scon_stk_ptr    :
                        { $$ = scon_stk_ptr; }
                ;

scon            :  '<' scon_stk_ptr namelist2 '>'
                        { $$ = $2; }

                |  '<' '*' '>'
                        {
                          $$ = scon_stk_ptr;

                          for (i = 1; i <= lastsc; ++i) {
                            int j;

                            for (j = 1; j <= scon_stk_ptr; ++j)
                              if (scon_stk[j] == i)
                                break;

                            if (j > scon_stk_ptr)
                              scon_stk[++scon_stk_ptr] = i;
                          }
                        }

                |
                        { $$ = scon_stk_ptr; }
                ;

namelist2       :  namelist2 ',' sconname

                |  sconname

                |  error
                        { synerr("bad start condition list"); }
                ;

sconname        :  NAME
                        {
                          if ((scnum = sclookup(nmstr)) == 0)
                            format_pinpoint_message("undeclared start condition %s", nmstr);
                          else {
                            for (i = 1; i <= scon_stk_ptr; ++i)
                              if (scon_stk[i] == scnum) {
                                format_warn("<%s> specified twice", scname[scnum]);
                                break;
                              }

                            if (i > scon_stk_ptr)
                              scon_stk[++scon_stk_ptr] = scnum;
                          }
                        }
                ;

rule            :  re2 re
                        {
                          if (transchar[lastst[$2]] != SYM_EPSILON)
                            /* Provide final transition \now/ so it
                             * will be marked as a trailing context
                             * state.
                             */
                            $2 = link_machines($2, mkstate(SYM_EPSILON));

                          mark_beginning_as_normal($2);
                          current_state_type = STATE_NORMAL;

                          if (previous_continued_action) {
                            /* We need to treat this as variable trailing
                             * context so that the backup does not happen
                             * in the action but before the action switch
                             * statement.  If the backup happens in the
                             * action, then the rules "falling into" this
                             * one's action will *also* do the backup,
                             * erroneously.
                             */
                            if (!varlength || headcnt != 0)
                              warn("trailing context made variable due to preceding '|' action");

                            /* Mark as variable. */
                            varlength = true;
                            headcnt = 0;
                          }

                          /* variable trailing context rule */
                          if (varlength && headcnt == 0) {
                            /* Mark the first part of the rule as the
                             * accepting "head" part of a trailing
                             * context rule.
                             *
                             * By the way, we didn't do this at the
                             * beginning of this production because back
                             * then current_state_type was set up for a
                             * trail rule, and add_accept() can create
                             * a new state ...
                             */
                            add_accept($1, num_rules | YY_TRAILING_HEAD_MASK);
                            variable_trail_rule = true;
                          }

                          else
                            trailcnt = rulelen;

                          $$ = link_machines($1, $2);
                        }

                |  re2 re '$'
                        { synerr("trailing context used twice"); }

                |  re '$'
                        {
                          headcnt = 0;
                          trailcnt = 1;
                          rulelen = 1;
                          varlength = false;

                          current_state_type = STATE_TRAILING_CONTEXT;

                          if (trlcontxt) {
                            synerr("trailing context used twice");
                            $$ = mkstate(SYM_EPSILON);
                          }

                          else if (previous_continued_action) {
                            /* See the comment in the rule for "re2 re"
                             * above.
                             */
                            warn("trailing context made variable due to preceding '|' action");

                            varlength = true;
                          }

                          if (varlength) {
                            /* Again, see the comment in the rule for
                             * "re2 re" above.
                             */
                            add_accept($1, num_rules | YY_TRAILING_HEAD_MASK);
                            variable_trail_rule = true;
                          }

                          trlcontxt = true;

                          eps = mkstate(SYM_EPSILON);
                          $$ = link_machines($1, link_machines(eps, mkstate('\n')));
                        }

                |  re
                        {
                          $$ = $1;

                          if (trlcontxt) {
                            if (varlength && headcnt == 0)
                              /* Both head and trail are
                               * variable-length.
                               */
                              variable_trail_rule = true;
                            else
                              trailcnt = rulelen;
                          }
                        }
                ;


re              :  re '|' series
                        {
                          varlength = true;
                          $$ = mkor($1, $3);
                        }

                |  series
                        { $$ = $1; }
                ;


re2             :  re '/'
                        {
                          /* This rule is written separately so the
                           * reduction will occur before the trailing
                           * series is parsed.
                           */

                          if (trlcontxt)
                            synerr("trailing context used twice");
                          else
                            trlcontxt = true;

                          if (varlength)
                            /* We hope the trailing context is
                             * fixed-length.
                             */
                            varlength = false;
                          else
                            headcnt = rulelen;

                          rulelen = 0;

                          current_state_type = STATE_TRAILING_CONTEXT;
                          $$ = $1;
                        }
                ;

series          :  series singleton
                        {
                          /* This is where concatenation of adjacent patterns
                           * gets done.
                           */
                          $$ = link_machines($1, $2);
                        }

                |  singleton
                        { $$ = $1; }
                ;

singleton       :  singleton '*'
                        {
                          varlength = true;

                          $$ = mkclos($1);
                        }

                |  singleton '+'
                        {
                          varlength = true;
                          $$ = mkposcl($1);
                        }

                |  singleton '?'
                        {
                          varlength = true;
                          $$ = mkopt($1);
                        }

                |  singleton '{' NUMBER ',' NUMBER '}'
                        {
                          varlength = true;

                          if ($3 > $5 || $3 < 0) {
                            synerr("bad iteration values");
                            $$ = $1;
                          }
                          else {
                            if ($3 == 0) {
                              if ($5 <= 0) {
                                synerr("bad iteration values");
                                $$ = $1;
                              }
                              else
                                $$ = mkopt(mkrep($1, 1, $5));
                            }
                            else
                              $$ = mkrep($1, $3, $5);
                          }
                        }

                |  singleton '{' NUMBER ',' '}'
                        {
                          varlength = true;

                          if ($3 <= 0) {
                            synerr("iteration value must be positive");
                            $$ = $1;
                          }

                          else
                            $$ = mkrep($1, $3, INFINITY);
                        }

                |  singleton '{' NUMBER '}'
                        {
                          /* The singleton could be something like "(foo)",
                           * in which case we have no idea what its length
                           * is, so we punt here.
                           */
                          varlength = true;

                          if ($3 <= 0) {
                            synerr("iteration value must be positive");
                            $$ = $1;
                          }

                          else
                            $$ = link_machines($1, copysingl($1, $3 - 1));
                        }

                |  '.'
                        {
                          if (!madeany) {
                            /* Create the '.' character class. */
                            anyccl = cclinit();
                            ccladd(anyccl, '\n');
                            cclnegate(anyccl);

                            if (useecs)
                              mkeccl(ccltbl + cclmap[anyccl],
                                     ccllen[anyccl], nextecm, ecgroup, csize, csize);

                            madeany = true;
                          }

                          ++rulelen;

                          $$ = mkstate(-anyccl);
                        }

                |  fullccl
                        {
                          if (!cclsorted)
                            /* Sort characters for fast searching.  We
                             * use a shell sort since this list could
                             * be large.
                             */
                            cshell(ccltbl + cclmap[$1], ccllen[$1], true);

                          if (useecs)
                            mkeccl(ccltbl + cclmap[$1], ccllen[$1], nextecm, ecgroup, csize, csize);

                          ++rulelen;

                          $$ = mkstate(-$1);
                        }

                |  PREVCCL
                        {
                          ++ rulelen;

                          $$ = mkstate(-$1);
                        }

                |  '"' string '"'
                        { $$ = $2; }

                |  '(' re ')'
                        { $$ = $2; }

                |  CHAR
                        {
                          ++rulelen;

                          if (caseins && $1 >= 'A' && $1 <= 'Z')
                            $1 = clower($1);

                          $$ = mkstate($1);
                        }
                ;

fullccl         :  '[' ccl ']'
                        { $$ = $2; }

                |  '[' '^' ccl ']'
                        {
                          cclnegate($3);
                          $$ = $3;
                        }
                ;

ccl             :  ccl CHAR '-' CHAR
                        {
                          if (caseins) {
                            if ($2 >= 'A' && $2 <= 'Z')
                              $2 = clower($2);
                            if ($4 >= 'A' && $4 <= 'Z')
                              $4 = clower($4);
                          }

                          if ($2 > $4)
                            synerr("negative range in character class");

                          else {
                            for (i = $2; i <= $4; ++i)
                              ccladd($1, i);

                            /* Keep track if this ccl is staying in
                             * alphabetical order.
                             */
                            cclsorted = cclsorted && ($2 > lastchar);
                            lastchar = $4;
                          }

                          $$ = $1;
                        }

                |  ccl CHAR
                        {
                          if (caseins && $2 >= 'A' && $2 <= 'Z')
                            $2 = clower($2);

                          ccladd($1, $2);
                          cclsorted = cclsorted && ($2 > lastchar);
                          lastchar = $2;
                          $$ = $1;
                        }

                |  ccl ccl_expr
                        {
                          /* Too hard to properly maintain cclsorted. */
                          cclsorted = false;
                          $$ = $1;
                        }

                |
                        {
                          cclsorted = true;
                          lastchar = 0;
                          currccl = $$ = cclinit();
                        }
                ;

ccl_expr:          CCE_ALNUM    { CCL_EXPR(isalnum) }
                |  CCE_ALPHA    { CCL_EXPR(isalpha) }
                |  CCE_BLANK    { CCL_EXPR(IS_BLANK) }
                |  CCE_CNTRL    { CCL_EXPR(iscntrl) }
                |  CCE_DIGIT    { CCL_EXPR(isdigit) }
                |  CCE_GRAPH    { CCL_EXPR(isgraph) }
                |  CCE_LOWER    { CCL_EXPR(islower) }
                |  CCE_PRINT    { CCL_EXPR(isprint) }
                |  CCE_PUNCT    { CCL_EXPR(ispunct) }
                |  CCE_SPACE    { CCL_EXPR(isspace) }
                |  CCE_UPPER    {
                                  if (caseins)
                                    CCL_EXPR(islower)
                                  else
                                    CCL_EXPR(isupper)
                                }
                |  CCE_XDIGIT   { CCL_EXPR(isxdigit) }
                ;

string          :  string CHAR
                        {
                          if (caseins && $2 >= 'A' && $2 <= 'Z')
                            $2 = clower($2);

                          ++rulelen;

                          $$ = link_machines($1, mkstate($2));
                        }

                |
                        { $$ = mkstate(SYM_EPSILON); }
                ;

%%


/* build_eof_action - build the "<<EOF>>" action for the active start
 *                    conditions
 */
void build_eof_action()
{
  int i;
  char action_text[MAXLINE];

  for (i = 1; i <= scon_stk_ptr; ++i) {
    if (sceof[scon_stk[i]])
      format_pinpoint_message("multiple <<EOF>> rules for start condition %s",
                              scname[scon_stk[i]]);

    else {
      sceof[scon_stk[i]] = true;
      sprintf(action_text, "case YY_STATE_EOF(%s):\n", scname[scon_stk[i]]);
      add_action(action_text);
    }
  }

  line_directive_out_src();

  /* This isn't a normal rule after all - don't count it as
   * such, so we don't have any holes in the rule numbering
   * (which make generating "rule can never match" warnings
   * more difficult.
   */
  --num_rules;
  ++num_eof_rules;
}


/* format_synerr - write out formatted syntax error */
void format_synerr(char const *msg, char const *arg)
{
  char errmsg[MAXLINE];

  (void) sprintf(errmsg, msg, arg);
  synerr(errmsg);
}


/* synerr - report a syntax error */
void synerr(char str[])
{
  syntaxerror = true;
  pinpoint_message(str);
}


/* Report a syntax error where 'fmt' has two integer placeholders. */
void synerr_ii(char const *fmt, int n1, int n2)
{
  char errmsg[MAXLINE];
  sprintf(errmsg, fmt, n1, n2);
  synerr(errmsg);
}


/* format_warn - write out formatted warning */
void format_warn(char msg[], char arg[])
{
  char warn_msg[MAXLINE];

  (void) sprintf(warn_msg, msg, arg);
  warn(warn_msg);
}


/* warn - report a warning, unless -w was given */
void warn(char str[])
{
  line_warning(str, linenum);
}

/* format_pinpoint_message - write out a message formatted with one string,
 *                           pinpointing its location
 */
void format_pinpoint_message(char const *msg, char const *arg)
{
  char errmsg[MAXLINE];

  (void) sprintf(errmsg, msg, arg);
  pinpoint_message(errmsg);
}


/* pinpoint_message - write out a message, pinpointing its location */
void pinpoint_message(char str[])
{
  line_pinpoint(str, linenum);
}


/* line_warning - report a warning at a given line, unless -w was given */
void line_warning(char str[], int line)
{
  char warning[MAXLINE];

  if (!nowarn) {
    sprintf(warning, "warning, %s", str);
    line_pinpoint(warning, line);
  }
}


/* line_pinpoint - write out a message, pinpointing it at the given line */
void line_pinpoint(char str[], int line)
{
  fprintf(stderr, "\"%s\", line %d: %s\n", infilename, line, str);
}


/* yyerror - eat up an error message from the parser;
 *           currently, messages are ignore
 */
void yyerror(char msg[])
{
}
