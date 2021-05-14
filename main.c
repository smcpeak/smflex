/* smflex - tool to generate fast lexical analyzers */

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

#ifndef lint
char copyright[] =
  "@(#) Copyright (c) 1990 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#include "main.h"                      /* this module */

#include "dfa.h"                       /* ntod */
#include "ecs.h"                       /* cre8cs, ccl2ecl */
#include "gen.h"                       /* make_tables, emit_header_file, etc. */
#include "input-parse.h"               /* warn, yyparse, etc. */
#include "input-scan.h"                /* set_input_file */
#include "misc.h"                      /* flexerror, outn, etc. */
#include "sym.h"                       /* scinstal */
#include "version.h"                   /* SMFLEX_VERSION */

#include <ctype.h>                     /* toupper */
#include <stdio.h>                     /* remove */
#include <string.h>                    /* strcpy, etc. */
#include <stdlib.h>                    /* exit */


/* Size of input alphabet - should be size of ASCII set. */
#ifndef DEFAULT_CSIZE
#  define DEFAULT_CSIZE 128
#endif

#ifdef VMS
#  ifndef __VMS_POSIX
#    define SHORT_FILE_NAMES
#  endif
#endif

/* Maximum number of NFA states that can comprise a DFA state.  It's real
 * big because if there's a lot of rules, the initial state will have a
 * huge epsilon closure.
 */
#define INITIAL_MAX_DFA_SIZE 750


/* Number that, if used to subscript an array, has a good chance of producing
 * an error; should be small enough to fit into a short.
 */
#define BAD_SUBSCRIPT -32767


static char smflex_version[] = SMFLEX_VERSION;


/* ----------- BEGIN: big block of globals ---------------- */
/* These globals are all declared and documented in main.h. */

int printstats, syntaxerror, eofseen, option_debug, trace, nowarn, spprdflt;
int interactive, caseins, do_yylineno, useecs, fulltbl, usemecs;
int jacobson, gen_line_dirs, performance_report, backing_up_report;
int cpp_interface, long_align, use_read, csize;
int yymore_used, reject_used, real_reject, continued_action, in_rule;
int yymore_really_used, reject_really_used;

int option_stack;

int option_main = false;
int option_yy_read_character = false;
int option_yy_scan_string = false;
int option_yy_scan_bytes = false;
int option_yy_scan_buffer = false;
int option_yy_top_state = false;
int option_yy_unread_character = false;

int option_flex_compat = false;

int datapos, dataline, linenum, out_linenum;
int scanner_skl_ind = 0;
FILE *backing_up_file;
char *infilename = NULL, *outfilename = NULL;
int did_outfilename;
char const *prefix;
char const *all_caps_prefix;
char *yyclass;

/* Make sure program_name is initialized so we don't crash if writing
 * out an error message before getting the program name from argv[0].
 */
char *program_name = "smflex";

char *action_array;
int action_size;
int defs1_offset, prolog_offset, action_offset, action_index;

FILE *scanner_c_file = NULL;

int write_native_line_endings;

int onestate[ONE_STACK_SIZE], onesym[ONE_STACK_SIZE];
int onenext[ONE_STACK_SIZE], onedef[ONE_STACK_SIZE], onesp;

int current_mns, current_max_rules;
int num_rules, num_eof_rules, default_rule, lastnfa;
int *firstst, *lastst, *finalst, *transchar, *trans1, *trans2;
int *accptnum, *assoc_rule, *state_type;
int *rule_type, *rule_linenum, *rule_useful;

int current_state_type;

int variable_trailing_context_rules;

int numtemps, numprots, protprev[MSP], protnext[MSP], prottbl[MSP];
int protcomst[MSP], firstprot, lastprot, protsave[PROT_SAVE_SIZE];

int numecs, nextecm[CSIZE + 1], ecgroup[CSIZE + 1], nummecs;
int tecfwd[CSIZE + 1];
int tecbck[CSIZE + 1];

int lastsc, *scset, *scbol, *scxclu, *sceof;
int current_max_scs;
char **scname;

int current_max_dfa_size, current_max_xpairs;
int current_max_template_xpairs, current_max_dfas;
int lastdfa, *nxt, *chk, *tnxt;
int *base, *def, *nultrans, NUL_ec, tblend, firstfree, **dss, *dfasiz;
union dfaacc_union *dfaacc;
int *accsiz, *dhash, numas;
int numsnpairs, jambase, jamstate;
int end_of_buffer_state;

int lastccl, *cclmap, *ccllen, *cclng, cclreuse;
int current_maxccls, current_max_ccl_tbl_size;
Char *ccltbl;

char nmstr[MAXLINE];
int sectnum, nummt, hshcol, dfaeql, numeps, eps2, num_reallocs;
int tmpuses, totnst, peakpairs, numuniq, numdup, hshsave;
int num_backing_up, bol_needed;

/* ----------- END: big block of globals ---------------- */


#ifndef SHORT_FILE_NAMES
static char *outfile_template = "lex.%s.%s";
static char *backing_name = "lex.backup";
#else
static char *outfile_template = "lex%s.%s";
static char *backing_name = "lex.bck";
#endif

static char outfile_path[MAXLINE];
static int outfile_created = 0;

char header_file_name[MAXLINE];


int main(int argc, char **argv)
{
  int i;

  flexinit(argc, argv);

  readin();

  ntod();

  for (i = 1; i <= num_rules; ++i)
    if (!rule_useful[i] && i != default_rule)
      line_warning(_("rule cannot be matched"), rule_linenum[i]);

  if (spprdflt && !reject_used && rule_useful[default_rule])
    line_warning(_("-s option given but default rule can be matched"),
                 rule_linenum[default_rule]);

  /* Generate the C state transition tables from the DFA.  Write the
   * generated scanner code to the primary output file. */
  make_tables();

  /* Emit the header file with interface definitions. */
  emit_header_file(header_file_name);

  /* Note, flexend does not return.  It exits with its argument
   * as status.
   */
  flexend(0);

  return 0;                     /* keep compilers/lint happy */
}


/* check_options - check user-specified options */
void check_options()
{
  int i;

  if (do_yylineno)
    /* This should really be "maintain_backup_tables = true" */
    reject_really_used = true;

  if (csize == unspecified) {
    if ((fulltbl || jacobson) && !useecs)
      csize = DEFAULT_CSIZE;
    else
      csize = CSIZE;
  }

  if (fulltbl || jacobson) {
    if (usemecs)
      flexerror(_("-Cf/-CJ and -Cm don't make sense together"));

    if (interactive)
      flexerror(_("-Cf/-CJ and -I are incompatible"));

    if (do_yylineno)
      flexerror(_("-Cf/-CJ and %option yylineno are incompatible"));

    if (fulltbl && jacobson)
      flexerror(_("-Cf and -CJ are mutually exclusive"));
  }

  if (useecs) {                 /* Set up doubly-linked equivalence classes. */

    /* We loop all the way up to csize, since ecgroup[csize] is
     * the position used for NUL characters.
     */
    ecgroup[1] = NIL;

    for (i = 2; i <= csize; ++i) {
      ecgroup[i] = i - 1;
      nextecm[i - 1] = i;
    }

    nextecm[csize] = NIL;
  }

  else {
    /* Put everything in its own equivalence class. */
    for (i = 1; i <= csize; ++i) {
      ecgroup[i] = i;
      nextecm[i] = BAD_SUBSCRIPT;       /* to catch errors */
    }
  }

  /* Open the C output file. */
  {
    if (!did_outfilename) {
      char *suffix;

      if (cpp_interface)
        suffix = "cc";
      else
        suffix = "c";

      sprintf(outfile_path, outfile_template, prefix, suffix);

      outfilename = outfile_path;
    }

    scanner_c_file = fopen(outfilename,
                           write_native_line_endings ? "w" : "wb");

    if (scanner_c_file == NULL)
      lerrsf(_("could not create %s"), outfilename);

    outfile_created = 1;
  }
}


/* flexend - terminate smflex
 *
 * note
 *    This routine does not return.
 */
void flexend(int exit_status)
{
  int tblsiz;

  if (exit_status != 0 && outfile_created) {
    if (ferror(scanner_c_file))
      lerrsf(_("error writing output file %s"), outfilename);

    else if (fclose(scanner_c_file))
      lerrsf(_("error closing output file %s"), outfilename);

    else if (remove(outfilename))
      lerrsf(_("error deleting output file %s"), outfilename);
  }

  if (backing_up_report && backing_up_file) {
    if (num_backing_up == 0)
      fprintf(backing_up_file, _("No backing up.\n"));
    else if (jacobson || fulltbl)
      fprintf(backing_up_file,
              _("%d backing up (non-accepting) states.\n"), num_backing_up);
    else
      fprintf(backing_up_file, _("Compressed tables always back up.\n"));

    if (ferror(backing_up_file))
      lerrsf(_("error writing backup file %s"), backing_name);

    else if (fclose(backing_up_file))
      lerrsf(_("error closing backup file %s"), backing_name);
  }

  if (printstats) {
    fprintf(stderr, _("%s version %s usage statistics:\n"),
            program_name, smflex_version);

    fprintf(stderr, _("  scanner options: -"));

    if (cpp_interface)
      putc('+', stderr);
    if (backing_up_report)
      putc('b', stderr);
    if (option_debug)
      putc('d', stderr);
    if (caseins)
      putc('i', stderr);
    if (performance_report > 0)
      putc('p', stderr);
    if (performance_report > 1)
      putc('p', stderr);
    if (spprdflt)
      putc('s', stderr);
    if (printstats)
      putc('v', stderr);        /* always true! */
    if (nowarn)
      putc('w', stderr);
    if (interactive == false)
      putc('B', stderr);
    if (interactive == true)
      putc('I', stderr);
    if (!gen_line_dirs)
      putc('L', stderr);
    if (trace)
      putc('T', stderr);

    if (csize == unspecified)
      /* We encountered an error fairly early on, so csize
       * never got specified.  Define it now, to prevent
       * bogus table sizes being written out below.
       */
      csize = 256;

    if (csize == 128)
      putc('7', stderr);
    else
      putc('8', stderr);

    fprintf(stderr, " -C");

    if (long_align)
      putc('a', stderr);
    if (fulltbl)
      putc('f', stderr);
    if (jacobson)
      putc('J', stderr);
    if (useecs)
      putc('e', stderr);
    if (usemecs)
      putc('m', stderr);
    if (use_read)
      putc('r', stderr);

    if (did_outfilename)
      fprintf(stderr, " -o%s", outfilename);

    if (!str_eq(prefix, "yy"))
      fprintf(stderr, " -P%s", prefix);

    putc('\n', stderr);

    fprintf(stderr, _("  %d/%d NFA states\n"), lastnfa, current_mns);
    fprintf(stderr, _("  %d/%d DFA states (%d words)\n"),
            lastdfa, current_max_dfas, totnst);
    fprintf(stderr, _("  %d rules\n"),
            num_rules + num_eof_rules - 1 /* - 1 for def. rule */ );

    if (num_backing_up == 0)
      fprintf(stderr, _("  No backing up\n"));
    else if (jacobson || fulltbl)
      fprintf(stderr,
              _("  %d backing-up (non-accepting) states\n"), num_backing_up);
    else
      fprintf(stderr, _("  Compressed tables always back-up\n"));

    if (bol_needed)
      fprintf(stderr, _("  Beginning-of-line patterns used\n"));

    fprintf(stderr, _("  %d/%d start conditions\n"), lastsc, current_max_scs);
    fprintf(stderr,
            _("  %d epsilon states, %d double epsilon states\n"),
            numeps, eps2);

    if (lastccl == 0)
      fprintf(stderr, _("  no character classes\n"));
    else
      fprintf(stderr, _("  %d/%d character classes needed %d/%d words of storage, %d reused\n"),
              lastccl, current_maxccls, cclmap[lastccl] + ccllen[lastccl],
              current_max_ccl_tbl_size, cclreuse);

    fprintf(stderr, _("  %d state/nextstate pairs created\n"), numsnpairs);
    fprintf(stderr, _("  %d/%d unique/duplicate transitions\n"),
            numuniq, numdup);

    if (fulltbl) {
      tblsiz = lastdfa * numecs;
      fprintf(stderr, _("  %d table entries\n"), tblsiz);
    }

    else {
      tblsiz = 2 * (lastdfa + numtemps) + 2 * tblend;

      fprintf(stderr,
              _("  %d/%d base-def entries created\n"),
              lastdfa + numtemps, current_max_dfas);
      fprintf(stderr,
              _("  %d/%d (peak %d) nxt-chk entries created\n"),
              tblend, current_max_xpairs, peakpairs);
      fprintf(stderr,
              _("  %d/%d (peak %d) template nxt-chk entries created\n"),
              numtemps * nummecs,
              current_max_template_xpairs, numtemps * numecs);
      fprintf(stderr, _("  %d empty table entries\n"), nummt);
      fprintf(stderr, _("  %d protos created\n"), numprots);
      fprintf(stderr,
              _("  %d templates created, %d uses\n"), numtemps, tmpuses);
    }

    if (useecs) {
      tblsiz = tblsiz + csize;
      fprintf(stderr,
              _("  %d/%d equivalence classes created\n"), numecs, csize);
    }

    if (usemecs) {
      tblsiz = tblsiz + numecs;
      fprintf(stderr,
              _("  %d/%d meta-equivalence classes created\n"),
              nummecs, csize);
    }

    fprintf(stderr,
            _("  %d (%d saved) hash collisions, %d DFAs equal\n"),
            hshcol, hshsave, dfaeql);
    fprintf(stderr, _("  %d sets of reallocations needed\n"), num_reallocs);
    fprintf(stderr, _("  %d total table entries needed\n"), tblsiz);
  }

  exit(exit_status);
}


/* flexinit - initialize smflex */
void flexinit(int argc, char **argv)
{
  int i, sawcmpflag;
  char *arg;

  printstats = syntaxerror = trace = spprdflt = caseins = false;
  cpp_interface = backing_up_report = option_debug = fulltbl = false;
  jacobson = long_align = nowarn = yymore_used = continued_action = false;
  do_yylineno = in_rule = reject_used = false;
  yymore_really_used = reject_really_used = unspecified;
  option_stack = false;
  csize = unspecified;
  interactive = false;
  gen_line_dirs = usemecs = useecs = true;
  performance_report = 0;
  did_outfilename = 0;
  write_native_line_endings = 0;
  prefix = "yy";
  all_caps_prefix = "YY";
  yyclass = NULL;
  use_read = false;

  sawcmpflag = false;

  /* Initialize dynamic array for holding the rule actions. */
  action_size = 2048;           /* default size of action array in bytes */
  action_array = allocate_character_array(action_size);
  defs1_offset = prolog_offset = action_offset = action_index = 0;
  action_array[0] = '\0';

  program_name = argv[0];

  /* Iterate over all program arguments. */
  for (--argc, ++argv; argc; --argc, ++argv) {
    arg = argv[0];

    if (arg[0] != '-' || arg[1] == '\0')
      break;

    if (arg[1] == '-') {        /* --option */
      if (str_eq(arg, "--flex-compat")) {
        option_flex_compat = true;
        continue;
      }

      else if (str_eq(arg, "--help"))
        arg = "-h";

      else if (str_eq(arg, "--version"))
        arg = "-V";

      else if (str_eq(arg, "--")) {    /* end of options */
        --argc;
        ++argv;
        break;
      }

      else {
        flexerror_s(_("unknown option: %s"), arg);
      }
    }

    /* Iterate over the one-letter flags that come after '-'. */
    for (i = 1; arg[i] != '\0'; ++i)
      switch (arg[i]) {
        case '+':
          cpp_interface = true;
          break;

        case 'B':
          interactive = false;
          break;

        case 'b':
          backing_up_report = true;
          break;

        case 'C':
          if (i != 1)
            flexerror(_("-C flag must be given separately"));

          if (!sawcmpflag) {
            useecs = false;
            usemecs = false;
            fulltbl = false;
            sawcmpflag = true;
          }

          for (++i; arg[i] != '\0'; ++i)
            switch (arg[i]) {
              case 'a':
                long_align = true;
                break;

              case 'e':
                useecs = true;
                break;

              case 'F':
                flexerror(_("-CF has been renamed to -CJ."));
                break;

              case 'J':
                jacobson = true;
                break;

              case 'f':
                fulltbl = true;
                break;

              case 'm':
                usemecs = true;
                break;

              case 'r':
                use_read = true;
                break;

              default:
                lerrif(_("unknown -C option '%c'"), (int) arg[i]);
                break;
            }

          goto get_next_arg;

        case 'd':
          option_debug = true;
          break;

        case 'f':
          flexerror(_("The -f flag has been removed.  Use -Cfr instead."));
          break;

        case 'F':
          flexerror(_("The -F flag has been removed.  Use -CJr instead."));
          break;

        case '?':
        case 'h':
          usage();
          exit(0);

        case 'I':
          interactive = true;
          break;

        case 'i':
          caseins = true;
          break;

        case 'L':
          gen_line_dirs = false;
          break;

        case 'n':
          write_native_line_endings = true;
          break;

        case 'o':
          if (i != 1)
            flexerror(_("-o flag must be given separately"));

          outfilename = arg + i + 1;
          did_outfilename = 1;
          goto get_next_arg;

        case 'P':
          if (i != 1)
            flexerror(_("-P flag must be given separately"));

          set_prefix(arg + i + 1);
          goto get_next_arg;

        case 'p':
          ++performance_report;
          break;

        case 's':
          spprdflt = true;
          break;

        case 'T':
          trace = true;
          break;

        case 'v':
          printstats = true;
          break;

        case 'V':
          printf(_("%s version %s\n"), program_name, smflex_version);
          exit(0);

        case 'w':
          nowarn = true;
          break;

        case '7':
          csize = 128;
          break;

        case '8':
          csize = CSIZE;
          break;

        default:
          fprintf(stderr,
                  _("%s: unknown flag '%c'.  For usage, try\n\t%s --help\n"),
                  program_name, (int) arg[i], program_name);
          exit(1);
      }

    /* Used by -C, -S, -o, and -P flags in lieu of a "continue 2"
     * control.
     */
  get_next_arg:;
  }

  /* Do this before setting the input file.  We never destroy this. */
  input_scan_construct(&input_lexer);

  if (argc != 1) {
    flexerror(_("smflex requires exactly one input file"));
  }
  set_input_file(argv[0]);

  lastccl = lastsc = lastdfa = lastnfa = 0;
  num_rules = num_eof_rules = default_rule = 0;
  numas = numsnpairs = tmpuses = 0;
  numecs = numeps = eps2 = num_reallocs = hshcol = dfaeql = totnst = 0;
  numuniq = numdup = hshsave = eofseen = datapos = dataline = 0;
  num_backing_up = onesp = numprots = 0;
  variable_trailing_context_rules = bol_needed = false;

  out_linenum = linenum = sectnum = 1;
  firstprot = NIL;

  /* Used in mkprot() so that the first proto goes in slot 1
   * of the proto queue.
   */
  lastprot = 1;

  set_up_initial_allocations();
}


/* Construct the name of the header file from the output file name
 * by replacing the extension with "h".  Put it in 'header_file_name'. */
static void compute_header_file_name()
{
  char *last_dot;

  strcpy(header_file_name, outfilename);
  last_dot = strrchr(header_file_name, '.');
  if (last_dot == NULL) {
    /* No dot, so add one. */
    last_dot = strrchr(header_file_name, '\0');
    *last_dot = '.';
  }
  strcpy(last_dot + 1, "h");
}


/* readin - read in the rules section of the input file(s) */
void readin()
{
  /* Create default DFA start condition. */
  scinstal("INITIAL", false);

  /* This prepares the first #line directive referring to the source
   * file to be emitted.  But because the output file is NULL, the
   * directive is put in 'action_array' for the moment. */
  line_directive_out_src();

  if (yyparse()) {
    pinpoint_message(_("fatal parse error"));
    flexend(1);
  }

  if (syntaxerror)
    flexend(1);

  if (backing_up_report) {
    backing_up_file = fopen(backing_name, "w");
    if (backing_up_file == NULL)
      lerrsf(_("could not create backing-up info file %s"), backing_name);
  }

  else
    backing_up_file = NULL;

  if (yymore_really_used == true)
    yymore_used = true;
  else if (yymore_really_used == false)
    yymore_used = false;

  if (reject_really_used == true)
    reject_used = true;
  else if (reject_really_used == false)
    reject_used = false;

  if (performance_report > 0) {
    if (interactive)
      fprintf(stderr,
              _("-I (interactive) entails a large (10x) performance penalty\n"));

    if (do_yylineno) {
      fprintf(stderr,
              _("%%option yylineno entails a large performance penalty (maybe; see manual)\n"));
    }

    if (performance_report > 1) {
      if (yymore_used)
        fprintf(stderr, _("yymore() entails a minor performance penalty\n"));
    }

    if (reject_used)
      fprintf(stderr, _("REJECT entails a large performance penalty (maybe; see manual)\n"));

    if (variable_trailing_context_rules)
      fprintf(stderr, _("Variable trailing context rules entail a large performance penalty\n"));
  }

  if (reject_used)
    real_reject = true;

  if (variable_trailing_context_rules)
    reject_used = true;

  if ((fulltbl || jacobson) && reject_used) {
    if (real_reject)
      flexerror(_("REJECT cannot be used with -Cf or -CJ"));
    else if (do_yylineno)
      flexerror(_("%option yylineno cannot be used with -Cf or -CJ"));
    else
      flexerror(_("variable trailing context rules cannot be used with -Cf or -CJ"));
  }

  /* Begin writing the primary output file. */
  if (did_outfilename)
    line_directive_out(scanner_c_file, 0);

  /* Set 'header_file_name' so it can be used by the skeleton.
   *
   * This is not the best place to do this, but the logic around setting
   * 'outfilename' is strangely complicated, so I am not sure where the
   * best place is. */
  compute_header_file_name();

  /* Copy the first chunk from the skeleton.  From here on, extending
   * into 'make_tables()', we alternate between inserting fragments of
   * code and calling 'skelout_upto()' to copy successive chunks.  The logic
   * here is tightly synchronized with the skeleton file organization. */
  skelout_upto("yy_text_def");

  if (option_debug)
    outn("\n#define SMFLEX_DEBUG");

  if (csize == 256)
    outn("typedef unsigned char YY_CHAR;");
  else
    outn("typedef char YY_CHAR;");

  if (yyclass && !cpp_interface) {
    flexerror(_("%option yyclass only meaningful for C++-interface scanners"));
  }

  if (useecs)
    numecs = cre8ecs(nextecm, ecgroup, csize);
  else
    numecs = csize;

  /* Now map the equivalence class for NUL to its expected place. */
  ecgroup[0] = ecgroup[csize];
  NUL_ec = ABS(ecgroup[0]);

  if (useecs)
    ccl2ecl();
}


/* Like 'copy_string' but uppercase the contents. */
static char *copy_string_toupper(char const *src)
{
  char *ret = copy_string(src);
  char *p = ret;
  for (; *p; p++) {
    *p = toupper(*p);
  }
  return ret;
}


/* Set 'prefix' to 'new_prefix', and set 'all_caps_prefix' to the
 * all-caps version. */
void set_prefix(char const *new_prefix)
{
  prefix = new_prefix;
  if (strlen(prefix) > MAX_PREFIX_LEN) {
    flexerror(_("-P argument too big"));
  }

  all_caps_prefix = copy_string_toupper(prefix);
}


/* set_up_initial_allocations - allocate memory for internal tables */
void set_up_initial_allocations()
{
  current_mns = INITIAL_MNS;
  firstst = allocate_integer_array(current_mns);
  lastst = allocate_integer_array(current_mns);
  finalst = allocate_integer_array(current_mns);
  transchar = allocate_integer_array(current_mns);
  trans1 = allocate_integer_array(current_mns);
  trans2 = allocate_integer_array(current_mns);
  accptnum = allocate_integer_array(current_mns);
  assoc_rule = allocate_integer_array(current_mns);
  state_type = allocate_integer_array(current_mns);

  current_max_rules = INITIAL_MAX_RULES;
  rule_type = allocate_integer_array(current_max_rules);
  rule_linenum = allocate_integer_array(current_max_rules);
  rule_useful = allocate_integer_array(current_max_rules);

  current_max_scs = INITIAL_MAX_SCS;
  scset = allocate_integer_array(current_max_scs);
  scbol = allocate_integer_array(current_max_scs);
  scxclu = allocate_integer_array(current_max_scs);
  sceof = allocate_integer_array(current_max_scs);
  scname = allocate_char_ptr_array(current_max_scs);

  current_maxccls = INITIAL_MAX_CCLS;
  cclmap = allocate_integer_array(current_maxccls);
  ccllen = allocate_integer_array(current_maxccls);
  cclng = allocate_integer_array(current_maxccls);

  current_max_ccl_tbl_size = INITIAL_MAX_CCL_TBL_SIZE;
  ccltbl = allocate_Character_array(current_max_ccl_tbl_size);

  current_max_dfa_size = INITIAL_MAX_DFA_SIZE;

  current_max_xpairs = INITIAL_MAX_XPAIRS;
  nxt = allocate_integer_array(current_max_xpairs);
  chk = allocate_integer_array(current_max_xpairs);

  current_max_template_xpairs = INITIAL_MAX_TEMPLATE_XPAIRS;
  tnxt = allocate_integer_array(current_max_template_xpairs);

  current_max_dfas = INITIAL_MAX_DFAS;
  base = allocate_integer_array(current_max_dfas);
  def = allocate_integer_array(current_max_dfas);
  dfasiz = allocate_integer_array(current_max_dfas);
  accsiz = allocate_integer_array(current_max_dfas);
  dhash = allocate_integer_array(current_max_dfas);
  dss = allocate_int_ptr_array(current_max_dfas);
  dfaacc = allocate_dfaacc_union(current_max_dfas);

  nultrans = (int *) 0;
}


/* Text to print in response to --help. */
static const char *usage_text[] = {
  "Usage: smflex [options] input.lex [...]",
  "",
  "Options that affect what input smflex accepts:",
  "",
  "  --flex-compat    Activate aliases that provide partial flex compatibility.",
  "",
  "Options that affect what output smflex writes:",
  "",
  "  -b               Generate backing-up information.",
  "  -h, --help, -?   Print usage.",
  "  -n               Write output with platform native line endings.",
  "  -ooutput         Specify name of output file [\"lex.yy.c\" or \"lex.yy.cc\"].",
  "  -p               Write performance report to stderr.",
  "  -v               Write scanner statistics to stderr.",
  "  -w               Suppress warnings.",
  "  -L               Suppress #line directives.",
  "  -T               Enable debug tracing of the table building process.",
  "  -V, --version    Print version number and exit.",
  "",
  "Options that influence the interface (API) of the generated scanner:",
  "",
  "  -+               Generate a scanner with a C++ interface.",
  "  -Pprefix         Specify symbol name prefix in generated scanner [\"yy\"].",
  "",
  "Options that influence the behavior of the generated scanner:",
  "",
  "  -7               Generate a 7-bit scanner [if -Cf or -CJ but not -Cfe or -CJe].",
  "  -8               Generate an 8-bit scanner [opposite of -7].",
  "  -d               Generate a debug mode scanner.",
  "  -i               Generate a case-insensitive scanner.",
  "  -s               Suppress the default \"echo\" rule.",
  "  -B               Generate a batch (non-interactive) scanner [true].",
  "  -C[aefFmr]*      Generate scanner with various table compression modes [-Cem].",
  "  -I               Generate an interactive scanner.",
  0
};


void usage()
{
  char const **line = usage_text;
  for (; *line; line++) {
    printf("%s\n", *line);
  }
}
